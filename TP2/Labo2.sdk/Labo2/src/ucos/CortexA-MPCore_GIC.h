///////////////////////////////////////////////////////////////////////////////
///\\file     CortexA-MPCore_GIC.h
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

#ifndef CORTEXA_MPCORE_GIC_H
#define CORTEXA_MPCORE_GIC_H

#include "CortexA-MPCore_SCU.h"
#include "CortexA-MPCore_GIC_Definitions.h"

#include <stdbool.h>


// ------------------------------------------------------------
// GIC
// ------------------------------------------------------------

// Typical calls to enable interrupt ID X:
// enable_irq_id(X)                 <-- Enable that ID
// set_irq_priority(X, 0)           <-- Set the priority of X to 0 (the max priority)
// set_priority_mask(0x1F)          <-- Set Core's priority mask to 0x1F (the lowest priority)
// enable_GIC()                     <-- Enable the GIC (global)
// enable_gic_processor_interface() <-- Enable the CPU interface (local to the core)
//

//  Get GIC base address
static inline unsigned long get_gic_base_addr() {
	unsigned long peripheralBaseAddress = get_base_addr();
	unsigned long gicBaseAddress = peripheralBaseAddress + GIC_OFFSET;
	return gicBaseAddress;
}

//  Global enable of the Interrupt Distributor
void enable_GIC(void);

// Global disable of the Interrupt Distributor
void disable_GIC(void);

// Enables the interrupt source number ID
void enable_irq_id(unsigned int ID);

// Disables the interrupt source number ID
void disable_irq_id(unsigned int ID);

// Sets the priority of the specifed ID
void set_irq_priority(unsigned int ID, unsigned int priority);

// Set or unset the given core as a target of the specified interrupt
void set_irq_target(unsigned int irqID, unsigned int cpuID, bool enable);

// Enables the processor interface
// Must been done one each core seperately
void enable_gic_processor_interface(void);  

// Disables the processor interface
void disable_gic_processor_interface(void);

// Sets the Priority mask register for the core run on
// The reset value masks ALL interrupts!
void set_priority_mask(unsigned int priority);

// Sets the Binary Point Register for the core run on
void set_binary_port(unsigned int priority);

//  Returns the value of the Interrupt Acknowledge Register
unsigned int read_irq_ack(void);

// Writes ID to the End Of Interrupt register
void write_end_of_irq(unsigned int ID);

// ------------------------------------------------------------
// SGI
// ------------------------------------------------------------

// Send a software generate interrupt
void send_sgi(unsigned int ID, unsigned int core_list, unsigned int filter_list);

#endif

// ------------------------------------------------------------
// End of MP_GIC.h
// ------------------------------------------------------------
