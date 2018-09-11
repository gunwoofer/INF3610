/*
*********************************************************************************************************
*                                                 uC/OS-II
*                                          The Real-Time Kernel
*                                      PORT LINUX CYGWIN and WINDOWS
*											MASTER INCLUDE FILE
*
*						Jerome CHEVALIER, Ecole Polytechnique de Montreal, Qc, CANADA
*                                                   09/2001
*
* File : INCLUDES.H
* By   : Jerome CHEVALIER	jerome.chevalier@polymtl.ca
* And the participation of :  Karim YAGHMOUR         yaghmourkarym@opersys.com
* Update :	20/08/2002,  Jerome CHEVALIER	jerome.chevalier@polymtl.ca   Bugs fix and new features
*			09/08/2003,  Jerome CHEVALIER	jerome.chevalier@polymtl.ca	  Cygwin and Windows port
*********************************************************************************************************
*/


#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include	<signal.h>
#include	<time.h>

#include    <setjmp.h>

#include    "os_cpu.h"
#include    "os_cfg_r.h"

#include    "ucos_ii.h"

 /*
*********************************************************************************************************
*
*                                  PORT SPECIFIC GLOBAL VARIABLE DEFINITIONS
*
*********************************************************************************************************
*/
#ifdef WIN32
	int flag_reg;
#else
	sigset_t set;
#endif

