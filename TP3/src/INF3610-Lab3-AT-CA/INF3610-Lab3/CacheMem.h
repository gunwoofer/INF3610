///////////////////////////////////////////////////////////////////////////////
//
//	CacheMem.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <systemc.h>
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////
//
//	Class CacheMem
//
///////////////////////////////////////////////////////////////////////////////
class CacheMem : public sc_module
{
	public: 
		// Ports
		sc_in_clk				clk;  
		sc_in<unsigned int*>	addressData;
		sc_in<unsigned int>  	length;
		sc_in<bool>				requestFromCPU;
		sc_out<bool>			ackToCPU;

		sc_out<unsigned int> 	address;
		sc_in<unsigned int>		dataReader;
		sc_out<bool>			requestToReader;
		sc_in<bool>				ackFromReader;
	
		// Constructor
		CacheMem( sc_module_name name );
		
		// Destructor
		~CacheMem();

	private:
		// Process SystemC
		SC_HAS_PROCESS(CacheMem);

		void thread(void);
		unsigned int currBufferLine;
};
