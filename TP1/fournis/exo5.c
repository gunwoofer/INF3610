/*
*********************************************************************************************************
*                                                 uC/OS-II
*                                          The Real-Time Kernel
*                                               PORT Windows
*
*
*            		          	Arnaud Desaulty, Frederic Fortier, Eva Terriault
*                                  Ecole Polytechnique de Montreal, Qc, CANADA
*                                                  01/2018
*
* File : exo4.c
*
*********************************************************************************************************
*/

// Main include of µC-II
#include "includes.h"
/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define TASK_STK_SIZE       16384            // Size of each task's stacks (# of WORDs)

#define ROBOT_A_PRIO_1   		8				 // Defining Priority of each task
#define ROBOT_A_PRIO_2   		9
#define ROBOT_B_PRIO_1   		10			
#define ROBOT_B_PRIO_2   		11
#define CONTROLLER_PRIO			22

#define QUEUE_SIZE 10
/*
*********************************************************************************************************
*                                             VARIABLES
*********************************************************************************************************
*/

OS_STK           prepRobotAStk_1[TASK_STK_SIZE];	//Stack of each task
OS_STK           prepRobotBStk_1[TASK_STK_SIZE];
OS_STK           prepRobotAStk_2[TASK_STK_SIZE];
OS_STK           prepRobotBStk_2[TASK_STK_SIZE];
OS_STK           transportStk[TASK_STK_SIZE];
OS_STK           controllerStk[TASK_STK_SIZE];

OS_EVENT *controller_to_robotA;
OS_EVENT *robotA_to_robotB_1;
OS_EVENT *robotA_to_robotB_2;
OS_EVENT *mutex_item_count;



/*
*********************************************************************************************************
*                                           SHARED  VARIABLES
*********************************************************************************************************
*/
volatile int total_item_count = 0;
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void    robotA(void *data);
void    robotB(void *data);
void    controller(void *data);
void    errMsg(INT8U err, char* errMSg);
int		readCurrentTotalItemCount();
void	writeCurrentTotalItemCount(int qty);
void	updateCurrentTotalCount(INT8U err, int itemCount);

/*
*********************************************************************************************************
*                                             STRUCTURES
*********************************************************************************************************
*/

typedef struct work_data {
	int work_data_a;
	int work_data_b;
} work_data;


work_data liste_controller_to_robotA[QUEUE_SIZE];
work_data liste_robotA_to_robotB_1[QUEUE_SIZE];
work_data liste_robotA_to_robotB_2[QUEUE_SIZE];

/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

void main(void)
{
	UBYTE err;

	int* numero_equipe;

	OSInit();

	mutex_item_count = OSMutexCreate(0, &err);
	errMsg(err, "Erreur mutex");

	err = OSTaskCreate(controller, (void*) 0, &controllerStk[TASK_STK_SIZE - 1], CONTROLLER_PRIO);
	errMsg(err, "Erreur creation du Controleur !");
	err = OSTaskCreate(robotA, (void*) numero_equipe = 1, &prepRobotAStk_1[TASK_STK_SIZE - 1], ROBOT_A_PRIO_1);
	errMsg(err, "Erreur creation du RobotA!");
	err = OSTaskCreate(robotB, (void*) numero_equipe = 1, &prepRobotBStk_1[TASK_STK_SIZE - 1], ROBOT_B_PRIO_1);
	errMsg(err, "Erreur creation du RobotB !");
	err = OSTaskCreate(robotA, (void*) numero_equipe = 2, &prepRobotAStk_2[TASK_STK_SIZE - 1], ROBOT_A_PRIO_2);
	errMsg(err, "Erreur creation du RobotA!");
	err = OSTaskCreate(robotB, (void*) numero_equipe = 2, &prepRobotBStk_2[TASK_STK_SIZE - 1], ROBOT_B_PRIO_2);
	errMsg(err, "Erreur creation du RobotB !");

	controller_to_robotA = OSQCreate(&liste_controller_to_robotA[0], QUEUE_SIZE);
	robotA_to_robotB_1 = OSQCreate(&liste_robotA_to_robotB_1[0], QUEUE_SIZE);
	robotA_to_robotB_2 = OSQCreate(&liste_robotA_to_robotB_2[0], QUEUE_SIZE);

	if (controller_to_robotA != NULL && robotA_to_robotB_1 != NULL && robotA_to_robotB_2 != NULL)
		OSStart();
	else
		printf("Erreur lors de la creation d'une de communication");

	return;
}

/*
*********************************************************************************************************
*                                            TASK FUNCTIONS
*********************************************************************************************************
*/

void robotA(void* data)
{
	INT8U err;
	int startTime = 0;
	int orderNumber = 1;

	printf("ROBOT A @ %d | EQUIPE %d : DEBUT.\n", OSTimeGet() - startTime, data);
	int itemCountRobotA;
	while (1)
	{
		work_data *work_data = OSQPend(controller_to_robotA, 0, &err);
		errMsg(err, "Erreur controller_to_robotA");

		if (data == 1) {
			err = OSQPost(robotA_to_robotB_1, work_data);
			errMsg(err, "Erreur post robotA_to_robotB_1");
		}
		else if (data == 2) {
			err = OSQPost(robotA_to_robotB_2, work_data);
			errMsg(err, "Erreur post robotA_to_robotB_2");
		}
		else {
			printf("Erreur argument pData");
		}

		itemCountRobotA = work_data->work_data_a;

		int counter = 0;
		while (counter < itemCountRobotA * 1000) { counter++; }
		printf("ROBOT A | EQUIPE %d COMMANDE #%d avec %d items @ %d.\n", data, orderNumber, itemCountRobotA, OSTimeGet() - startTime);
		
		updateCurrentTotalCount(err, itemCountRobotA);

		orderNumber++;
	}
}

void robotB(void* data)
{
	INT8U err;
	int startTime = 0;
	int orderNumber = 1;
	printf("ROBOT B @ %d | EQUIPE %d : DEBUT. \n", OSTimeGet() - startTime, data);
	int itemCountRobotB;
	while (1)
	{
		work_data *work_data = NULL;

		if (data == 1) {
			work_data = OSQPend(robotA_to_robotB_1, 0, &err);
			errMsg(err, "Erreur robotA_to_robotB_1");
		}
		else if (data == 2) {
			work_data = OSQPend(robotA_to_robotB_2, 0, &err);
			errMsg(err, "Erreur robotA_to_robotB_2");
		}
		else {
			printf("Erreur argument pData");
		}

		itemCountRobotB = work_data->work_data_b;

		int counter = 0;
		while (counter < itemCountRobotB * 1000) { counter++; }
		printf("ROBOT B | EQUIPE %d COMMANDE #%d avec %d items @ %d.\n", data, orderNumber, itemCountRobotB, OSTimeGet() - startTime);
		updateCurrentTotalCount(err, itemCountRobotB);
		orderNumber++;
	}
}

void controller(void* data)
{
	INT8U err;
	int startTime = 0;
	int randomTime = 0;
	work_data* workData;
	printf("TACHE CONTROLLER @ %d : DEBUT. \n", OSTimeGet() - startTime);

	for (int i = 1; i < 11; i++)
	{
		//Création d'une commande
		workData = malloc(sizeof(work_data));

		workData->work_data_a = (rand() % 8 + 3) * 10;
		workData->work_data_b = (rand() % 8 + 6) * 10;

		printf("TACHE CONTROLLER @ %d : COMMANDE #%d. \nPrep time A = %d, Prep time B = %d\n", OSTimeGet() - startTime, i, workData->work_data_a, workData->work_data_b);

		// A completer

		err = OSQPostOpt(controller_to_robotA, workData, OS_POST_OPT_BROADCAST);
		errMsg(err, "Erreur post controller_to_robotA");

		// Délai aléatoire avant nouvelle commande
		randomTime = (rand() % 9 + 5) * 4;
		OSTimeDly(randomTime);
	}

	free(workData);
}

int	readCurrentTotalItemCount()
{
	OSTimeDly(2);
	return total_item_count;
}

void writeCurrentTotalItemCount(int newCount)
{
	OSTimeDly(2);
	total_item_count = newCount;
}

void errMsg(INT8U err, char* errMsg)
{
	if (err != OS_ERR_NONE)
	{
		printf(errMsg);
		exit(1);
	}
}

void updateCurrentTotalCount(INT8U err, int itemCount) {
	OSMutexPend(mutex_item_count, 0, &err);
	errMsg(err, "Erreur mutex_item_count");
	writeCurrentTotalItemCount(readCurrentTotalItemCount() + itemCount);
	OSMutexPost(mutex_item_count);
}