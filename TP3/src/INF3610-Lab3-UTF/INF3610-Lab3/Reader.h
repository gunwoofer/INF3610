///////////////////////////////////////////////////////////////////////////////
//
//	Reader.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <systemc.h>
#include "LMBIF.h"
#include "InterfaceRead.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Class Reader
//
///////////////////////////////////////////////////////////////////////////////
class Reader : public sc_channel, public InterfaceRead
{
	public:
		// Ports
		sc_port<LMBIF>			dataPortRAM;
		
		// Constructor
		Reader( sc_module_name name );

		// Destructor
		~Reader();

	private:
	
	// Read - InterfaceRead
	virtual unsigned int Read(unsigned int addr);
};
