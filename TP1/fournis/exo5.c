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

#define ROBOT_A_PRIO   		8				 // Defining Priority of each task
#define ROBOT_B_PRIO   		9
#define CONTROLLER_PRIO     22

#define QUEUE_SIZE 10
/*
*********************************************************************************************************
*                                             VARIABLES
*********************************************************************************************************
*/

OS_STK           prepRobotAStk[TASK_STK_SIZE];	//Stack of each task
OS_STK           prepRobotBStk[TASK_STK_SIZE];
OS_STK           transportStk[TASK_STK_SIZE];
OS_STK           controllerStk[TASK_STK_SIZE];

OS_EVENT *controller_to_robotA;
OS_EVENT *robotA_to_robotB;
OS_EVENT* mutex_item_count;



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


work_data liste_controller_to_robotA[10];
work_data liste_robotA_to_robotB[10];

/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

void main(void)
{
	UBYTE err;

	// A completer

	OSInit();

	mutex_item_count = OSMutexCreate(0, &err);

	err = OSTaskCreate(controller, (void*)0, &controllerStk[TASK_STK_SIZE - 1], CONTROLLER_PRIO);
	errMsg(err, "Erreur creation du Controleur !");
	err = OSTaskCreate(robotA, (void*)0, &prepRobotAStk[TASK_STK_SIZE - 1], ROBOT_A_PRIO);
	errMsg(err, "Erreur creation du RobotA!");
	err = OSTaskCreate(robotB, (void*)0, &prepRobotBStk[TASK_STK_SIZE - 1], ROBOT_B_PRIO);
	errMsg(err, "Erreur creation du RobotB !");

	controller_to_robotA = OSQCreate(&liste_controller_to_robotA[0], QUEUE_SIZE);
	robotA_to_robotB = OSQCreate(&liste_robotA_to_robotB[0], QUEUE_SIZE);

	if (controller_to_robotA != NULL && robotA_to_robotB != NULL)
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

	printf("ROBOT A @ %d : DEBUT.\n", OSTimeGet() - startTime);
	int itemCountRobotA;
	while (1)
	{
		// A completer

		work_data *work_data = OSQPend(controller_to_robotA, 0, &err);
		errMsg(err, "Erreur controller_to_robotA");

		OSQPost(robotA_to_robotB, work_data);

		itemCountRobotA = work_data->work_data_a;

		int counter = 0;
		while (counter < itemCountRobotA * 1000) { counter++; }
		printf("ROBOT A COMMANDE #%d avec %d items @ %d.\n", orderNumber, itemCountRobotA, OSTimeGet() - startTime);
		updateCurrentTotalCount(err, itemCountRobotA);

		orderNumber++;
	}
}

void robotB(void* data)
{
	INT8U err;
	int startTime = 0;
	int orderNumber = 1;
	printf("ROBOT B @ %d : DEBUT. \n", OSTimeGet() - startTime);
	int itemCountRobotB;
	while (1)
	{
		// A completer

		work_data *work_data = OSQPend(robotA_to_robotB, 0, &err);
		errMsg(err, "Erreur robotA_to_robotB");

		itemCountRobotB = work_data->work_data_b;

		int counter = 0;
		while (counter < itemCountRobotB * 1000) { counter++; }
		printf("ROBOT B COMMANDE #%d avec %d items @ %d.\n", orderNumber, itemCountRobotB, OSTimeGet() - startTime);
		updateCurrentTotalCount(err, itemCountRobotB);
		orderNumber++;
	}
}

void controller(void* data)
{
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

		printf("TACHE CONTROLLER @ %d : COMMANDE #%d. \n prep time A = %d, prep time B = %d\n", OSTimeGet() - startTime, i, workData->work_data_a, workData->work_data_b);

		// A completer

		OSQPost(controller_to_robotA, workData);

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