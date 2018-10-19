///////////////////////////////////////////////////////////////////////////////
///\\file     CortexA-MPCore_GIC_c.c
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
///    $Revision: 9261 $
///    $Date: 2011-07-06 11:30:49 -0400 (mer., 06 juil. 2011) $
///////////////////////////////////////////////////////////////////////////////

#include "CortexA-MPCore_GIC.h"

void set_irq_target(unsigned int irqID, unsigned int cpuID, bool enable)
{
    unsigned long word, bit_shift, temp, cpuMask;

    // There are 4 interrupt target registers per word
    word = irqID / 4;
    bit_shift = (irqID % 4) * 8;
    
    // Compute the CPU mask based on the CPU ID
    cpuMask = (1 << cpuID) << bit_shift;

	// Compute the address of the target register associated to the interrupt ID
	unsigned long gicBaseAddress = get_gic_base_addr();
	unsigned long targetRegAddress = gicBaseAddress + IPTR_OFFSET + (4*word);
	volatile unsigned long* pulTargetReg = (volatile unsigned long*)targetRegAddress;

	// Read current value of the target register
    temp = *pulTargetReg;
    
    if (enable)
    {
    	// Add the specified CPU as a target of the interrupt (other CPUs are not affected)
        temp |= cpuMask;
    }
    else
    {
    	// Remove the specified CPU as a target of the interrupt (other CPUs are not affected)    	
        temp &= ~cpuMask;
    }
    
	// Update value of the target register    
    *pulTargetReg = temp;
}
