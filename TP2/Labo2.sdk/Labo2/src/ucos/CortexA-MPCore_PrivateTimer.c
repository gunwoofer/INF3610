///////////////////////////////////////////////////////////////////////////////
///\\file     CortexA-MPCore_PrivateTimer.c
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

#include "CortexA-MPCore_PrivateTimer.h"
#include "CortexA-MPCore_GIC.h"

void private_timer_init(unsigned mode)
{
    unsigned long prescale = 0; // Note: scaler always assumed to be 0, therefore with division factor of 0+1 = 1
    unsigned long timerBaseAddress = get_private_timer_base_addr();

	volatile unsigned long* pulLoadReg  = (volatile unsigned long*)(timerBaseAddress + TIMER_LOAD_REGISTER_OFFSET);
	volatile unsigned long* pulCountReg = (volatile unsigned long*)(timerBaseAddress + TIMER_COUNT_REGISTER_OFFSET);
	volatile unsigned long* pulCtrlReg  = (volatile unsigned long*)(timerBaseAddress + TIMER_CONTROL_REGISTER_OFFSET);

    // Reset the timers, and clear interrupts
	*pulLoadReg = 0;
	*pulCountReg = 0;
	private_timer_clear_irq();

    switch (mode)
    {
        case AUTO_RELOAD_TIMER :
            *pulCtrlReg = (prescale << TIMER_PRESCALE_SHIFT) | TIMER_IT_ENABLE | TIMER_AUTO_RELOAD;
            break;

        case ONE_SHOT_TIMER :
            *pulCtrlReg = (prescale << TIMER_PRESCALE_SHIFT) | TIMER_IT_ENABLE;
			break;

        case DISABLE_TIMER :
        default:
            *pulCtrlReg &= ~TIMER_ENABLE;
    }

    if (mode != DISABLE_TIMER)
    {
        enable_irq_id(TIMER_INTERRUPT); // Enable this CPU's private timer's interrupt
        set_irq_priority(TIMER_INTERRUPT, 0); // Set the priority of X to 0 (the max priority)	
    }
}

void private_timer_request(unsigned long load_value)
{
    unsigned long timerBaseAddress = get_private_timer_base_addr();

	volatile unsigned long* pulLoadReg = (volatile unsigned long*)(timerBaseAddress + TIMER_LOAD_REGISTER_OFFSET);
	volatile unsigned long* pulCtrlReg = (volatile unsigned long*)(timerBaseAddress + TIMER_CONTROL_REGISTER_OFFSET);

    *pulLoadReg  = load_value;
    *pulCtrlReg |= TIMER_ENABLE;
}

bool private_timer_irq_triggered(void)
{
	unsigned long timerBaseAddress = get_private_timer_base_addr();

	volatile unsigned long* pulIntrReg  = (volatile unsigned long*)(timerBaseAddress + TIMER_INTERRUPT_REGISTER_OFFSET);
	
	return (*pulIntrReg & TIMER_INTERRUPT_TRIGGERED) != 0;
}

void private_timer_clear_irq()
{
	unsigned long timerBaseAddress = get_private_timer_base_addr();

	volatile unsigned long* pulIntrReg  = (volatile unsigned long*)(timerBaseAddress + TIMER_INTERRUPT_REGISTER_OFFSET);
	
	*pulIntrReg = TIMER_INTERRUPT_CLEAR;
}
