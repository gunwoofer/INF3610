///////////////////////////////////////////////////////////////////////////////
//
//	CacheMem.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "CacheMem.h"


///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
CacheMem::CacheMem( sc_module_name name )
: sc_module(name)
{
	SC_THREAD(thread);
	sensitive << clk.pos();

	currBufferLine = 0;
}


///////////////////////////////////////////////////////////////////////////////
//
//	Destructeur
//
///////////////////////////////////////////////////////////////////////////////
CacheMem::~CacheMem()
{
}


///////////////////////////////////////////////////////////////////////////////
//
//	thread
//
///////////////////////////////////////////////////////////////////////////////
void CacheMem::thread(void)
{// Variable
	unsigned int uiAddress;
	unsigned int uiLength;

	// Boucle infinie
	while (1)
	{

		// Verify request port
		do {
			wait(clk->posedge_event());
		} while (!requestFromCPU.read());

		uiAddress = address.read();
		uiLength = length.read();
		unsigned int* buffer = addressData.read();

		unsigned int numBlocks = uiLength / 4; //Reader reads 4 bytes at a time
		for (unsigned int i = 0; i < numBlocks; i++)
		{
			address.write(uiAddress);
			requestToReader.write(true);

			//Wait ack from reader
			do {
				wait(clk->posedge_event());
			} while (!ackFromReader.read());

			//Recupere les donnees du reader
			buffer[i] = dataReader.read();

			requestToReader.write(false);
			
			wait(clk->posedge_event());

			uiAddress += 4;
		}

		ackToCPU.write(true);

		wait(clk->posedge_event());
		ackToCPU.write(false);
	}
}

