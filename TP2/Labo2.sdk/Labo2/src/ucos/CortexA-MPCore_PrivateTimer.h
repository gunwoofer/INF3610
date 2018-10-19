///////////////////////////////////////////////////////////////////////////////
///\\file     CortexA-MPCore_PrivateTimer.h
///\\brief    File Description
///
///                                                                        <B>
///         Space Libraries - Space Codesign (http://www.spacecodesign.com)   <BR>
///         (c) All Rights Reserved. 2006                                  <BR>
///                                                                        </B>
///         No authorisation to modify or use this file for
///         commercial purposes without prior written consentment
///         of its author(s)                                               <BR>
///
///    $Author: lmoss $
///    $Revision: 7842 $
///    $Date: 2010-10-18 18:01:33 -0400 (lun., 18 oct. 2010) $
///////////////////////////////////////////////////////////////////////////////

#ifndef CORTEXAMPCORE_PRIVATETIMER_H_
#define CORTEXAMPCORE_PRIVATETIMER_H_

#include "CortexA-MPCore_SCU.h"
#include "CortexA-MPCore_GIC.h"

#include <stdbool.h>

// Offset of the private timer base address relative to the private peripheral memory space base address
#define PRIVATE_TIMER_OFFSET 0x0600

// Register timer offsets
#define TIMER_LOAD_REGISTER_OFFSET 		0x00
#define TIMER_COUNT_REGISTER_OFFSET 	0x04
#define TIMER_CONTROL_REGISTER_OFFSET 	0x08
#define TIMER_INTERRUPT_REGISTER_OFFSET 0x0C

// Value into interrupt register that indicates whether the timer interrupt has been triggered
#define TIMER_INTERRUPT_TRIGGERED 0x1

// Value to write into interrupt register to clear interrupts
#define TIMER_INTERRUPT_CLEAR 0x1

// LSB of timer prescaler within control register
#define TIMER_PRESCALE_SHIFT  8

// Flags within the control register
#define TIMER_IT_ENABLE       4
#define TIMER_AUTO_RELOAD     2
#define TIMER_ENABLE          1

/**
 * Timer mode macro. Used to set the local timer to auto reload mode.
 * Used as a parameter for private_timer_init() when initialising the timer
 *
 */
#define AUTO_RELOAD_TIMER  0


/**
 * Timer mode macro. Used to set the local timer to single shot mode.
 * Used as a parameter for private_timer_init() when initialising the timer
 *
 */
#define ONE_SHOT_TIMER     1


/**
 * Macro used as a parameter for timer_init() to disable the timer
 * Used as a parameter for private_timer_init() when initialising the timer
 *
 */
#define DISABLE_TIMER      2

// The private timer's interrupt ID
#define TIMER_INTERRUPT 29

//  Get private timer base address
static inline unsigned long get_private_timer_base_addr() {
	unsigned long peripheralBaseAddress = get_base_addr();
	unsigned long timerBaseAddress = peripheralBaseAddress + PRIVATE_TIMER_OFFSET;
	return timerBaseAddress;
}

/**
 * Initializes one of the CPU's private timers  
 *
 * \param[in] mode    The timer mode. This can be one of the following:
 *  - AUTO_RELOAD_TIMER - sets up a periodic timer
 *  - ONE_SHOT_TIMER - sets up a one-shot timer
 *  - DISABLE_TIMER - disables the timer
 *
 * \return No value returned
 * 
 */
void private_timer_init(unsigned mode);

/**
 * Request the timer to trigger called after a number of cycles.
 * If the timer is in AUTO_RELOAD_TIMER mode, the handler will be called periodically.
 *
 * \param[in] load_value Interval at which the timer will trigger
 * 
 * \return No value returned
 * 
 */
void private_timer_request(unsigned long load_value);

/*
 * Indicates whether the timer interrupt has been interrupted since the last time it was cleared
 */
bool private_timer_irq_triggered(void);

/*
 * Clears the timer interrupt
 */
void private_timer_clear_irq();

#endif /*CORTEXAMPCORE_PRIVATETIMER_H_*/
