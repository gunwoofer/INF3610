///////////////////////////////////////////////////////////////////////////////
//
//	main.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include <systemc.h>
#include "Sobel.h"
#include "Reader.h"
#include "Writer.h"
#include "DataRAM.h"

#define RAMSIZE 0x200000

// Global variables
bool m_bError = false;

///////////////////////////////////////////////////////////////////////////////
//
//	Main
//
///////////////////////////////////////////////////////////////////////////////
int sc_main(int arg_count, char **arg_value)
{
	// Variables
	int sim_units = 2; //SC_NS 
	
	// Components
	// TODO: Instanciation des modules 
	Reader reader("Reader");
	Writer writer("Writer");
	Sobel sobel("Sobel");
	DataRAM dataram("DataRAM", "image.mem", RAMSIZE, true); // verbose a true pour faciliter debug

	// Connexions
	//TODO: Connexions des ports des modules
	reader.dataPortRAM(dataram);
	sobel.readPort(reader);
	sobel.writePort(writer);
	writer.dataPortRAM(dataram);

	// D�marrage de l'application
	if (!m_bError)
	{
		cout << "D�marrage de la simulation." << endl;	
		sc_start( -1, sc_core::sc_time_unit(sim_units) );
		cout << endl << "Simulation s'est termin�e � " << sc_time_stamp();
	}
	// Fin du programme
	return 0;
}
