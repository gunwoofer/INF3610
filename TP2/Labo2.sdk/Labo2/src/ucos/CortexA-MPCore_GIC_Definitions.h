///////////////////////////////////////////////////////////////////////////////
///\\file     CortexA-MPCore_GIC_Definitions.h
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

#ifndef CORTEXA_MPCORE_GIC_DEF_H
#define CORTEXA_MPCORE_GIC_DEF_H

// Offset of the GIC base address relative to the private peripheral memory space base address
#define GIC_OFFSET 0x1000

// Offset of the IPTR register relative to the GIC base address
#define IPTR_OFFSET 0x800

// The least significant 10 bits of the IAR contain the interrupt ID
#define IAR_INTERRUPT_ID_MASK 0x000003FF

// This ID is returned when acknowledging an interrupt, if no interrupt is pending.
// This is also returned when acknowledging an interrupt that has become masked due
// to priority change but was already pending (sent to the CPU).
#define GIC_SPURIOUS_INTERRUPT 1023

#endif