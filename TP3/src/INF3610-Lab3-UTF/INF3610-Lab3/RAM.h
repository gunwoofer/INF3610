///////////////////////////////////////////////////////////////////////////////
//
//	RAM.h
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <systemc.h>
#include "LMBIF.h"
//#include "Application_Definition.h"

// Variable externe
extern bool m_bError;

///////////////////////////////////////////////////////////////////////////////
//
//	Classe RAM
//
///////////////////////////////////////////////////////////////////////////////
class RAM : public sc_module, public LMBIF
{
	public: 

		// Constructeur
		RAM( sc_module_name name, const char *fileName, unsigned long RAMSize = 0, bool verbose = false );
		
		// Destructeur
		~RAM();

		// Méthode
		virtual unsigned int Read(unsigned int addr);
		virtual void Write(unsigned int addr, unsigned int data)	= 0;

	private:	

		// Variable 
		
	protected:
	
		// Variable 
		void* m_ptrRAM;
		unsigned long m_size;
		unsigned long m_RAMSize;
		bool m_verbose;
};

