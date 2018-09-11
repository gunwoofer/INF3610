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
* File : exo3.c
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

#define ROBOT_A_PRIO   		10				 // Defining Priority of each task
#define ROBOT_B_PRIO   		9
#define CONTROLLER_PRIO     7

#define COMMANDE_READY  0x1
#define ROBOTA_TO_ROBOTB  0x2
#define ITEM_COUNT  0x4

/*
*********************************************************************************************************
*                                             VARIABLES
*********************************************************************************************************
*/

OS_STK           robotAStk[TASK_STK_SIZE];	//Stack of each task
OS_STK           robotBStk[TASK_STK_SIZE];
OS_STK           controllerStk[TASK_STK_SIZE];

OS_FLAG_GRP *FlagGroup;

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
void updateCurrentTotalCount(INT8U err, int itemCount);

/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

void main(void)
{
	UBYTE err, errFlag;

	// A completer
	OSInit();

	FlagGroup = OSFlagCreate(0x4, &errFlag);

	err = OSTaskCreate(controller, (void*)0, &controllerStk[TASK_STK_SIZE - 1], CONTROLLER_PRIO);
	errMsg(err, "Erreur !");
	err = OSTaskCreate(robotA, (void*)0, &robotAStk[TASK_STK_SIZE - 1], ROBOT_A_PRIO);
	errMsg(err, "Erreur !");
	err = OSTaskCreate(robotB, (void*)0, &robotBStk[TASK_STK_SIZE - 1], ROBOT_B_PRIO);
	errMsg(err, "Erreur !");


	if (errFlag == OS_ERR_NONE) {
		OSStart();
	}




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
	int itemCount;
	printf("ROBOT A @ %d : DEBUT.\n", OSTimeGet() - startTime);
	while (1)
	{
		itemCount = (rand() % 7 + 1) * 10;

		// A completer

		OSFlagPend(FlagGroup, COMMANDE_READY, OS_FLAG_WAIT_SET_ANY, 0, &err);
		OSFlagPost(FlagGroup, COMMANDE_READY, OS_FLAG_CLR, &err);

		OSFlagPost(FlagGroup, ROBOTA_TO_ROBOTB, OS_FLAG_SET, &err);

		int counter = 0;
		while (counter < itemCount * 1000) { counter++; }
		printf("ROBOT A COMMANDE #%d avec %d items @ %d.\n", orderNumber, itemCount, OSTimeGet() - startTime);
		updateCurrentTotalCount(err, itemCount);
		orderNumber++;
	}
}

void robotB(void* data)
{
	INT8U err;
	int startTime = 0;
	int orderNumber = 1;
	int itemCount;
	printf("ROBOT B @ %d : DEBUT. \n", OSTimeGet() - startTime);
	while (1)
	{
		itemCount = (rand() % 6 + 2) * 10;

		// A completer

		OSFlagPend(FlagGroup, ROBOTA_TO_ROBOTB, OS_FLAG_WAIT_SET_ANY, 0, &err);
		OSFlagPost(FlagGroup, ROBOTA_TO_ROBOTB, OS_FLAG_CLR, &err);

		int counter = 0;
		while (counter < itemCount * 1000) { counter++; }
		printf("ROBOT B COMMANDE #%d avec %d items @ %d.\n", orderNumber, itemCount, OSTimeGet() - startTime);
		updateCurrentTotalCount(err, itemCount);
		orderNumber++;
	}
}

void controller(void* data)
{
	INT8U err;
	int startTime = 0;
	int randomTime = 0;
	printf("CONTROLLER @ %d : DEBUT. \n", OSTimeGet() - startTime);
	for (int i = 1; i < 11; i++)
	{
		randomTime = (rand() % 9 + 5) * 10;
		OSTimeDly(randomTime);

		printf("CONTROLLER @ %d : COMMANDE #%d. \n", OSTimeGet() - startTime, i);
		OSFlagPost(FlagGroup, COMMANDE_READY, OS_FLAG_SET, &err);
		// A completer
	}
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
	OSFlagPend(FlagGroup, ITEM_COUNT, OS_FLAG_WAIT_SET_ANY, 0, &err);
	errMsg(err, "Erreur item_count");
	OSFlagPost(FlagGroup, ITEM_COUNT, OS_FLAG_CLR, &err);
	errMsg(err, "Erreur item_count");
	writeCurrentTotalItemCount(readCurrentTotalItemCount() + itemCount);
	OSFlagPost(FlagGroup, ITEM_COUNT, OS_FLAG_SET, &err);
	errMsg(err, "Erreur item_count");
}