///////////////////////////////////////////////////////////////////////////////
//
//	main.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include <systemc.h>
#include "Sobel.h"
#include "Sobelv2.h"
#include "Reader.h"
#include "Writer.h"
#include "DataRAM.h"
#include "CacheMem.h"

#define RAMSIZE 0x200000

///////////////////////////////////////////////////////////////////////////////
//
//	Main
//
///////////////////////////////////////////////////////////////////////////////
int sc_main(int arg_count, char **arg_value)
{
	// Variables
	int sim_units = 2; //SC_NS

	// Clock
	const sc_core::sc_time_unit simTimeUnit = SC_NS;
	const int clk_freq = 4000;
	sc_clock clk("SysClock", clk_freq, simTimeUnit, 0.5);

	// Components
	Reader reader("Reader");
	DataRAM dataRAM("DataRAM", "image.mem", RAMSIZE, false);
	//TODO : Déclaration du module de l'écrivain
	Writer writer("Writer");



	// Signals
	sc_signal<unsigned int, SC_MANY_WRITERS> data;
	sc_signal<unsigned int, SC_MANY_WRITERS> address;
	sc_signal<unsigned int*> addressData;
	sc_signal<unsigned int> length;
	sc_signal<bool, SC_MANY_WRITERS> reqRead;
	sc_signal<bool, SC_MANY_WRITERS> ackReaderWriter;
	sc_signal<bool, SC_MANY_WRITERS> reqWrite;

	/* à compléter*/

	// Connexions
	reader.clk(clk);
	reader.data(data);
	reader.address(address);
	reader.request(reqRead);
	reader.ack(ackReaderWriter);
	reader.dataPortRAM(dataRAM);

	/* à compléter */
	writer.clk(clk);
	writer.data(data);
	writer.address(address);
	writer.request(reqWrite);
	writer.ack(ackReaderWriter);
	writer.dataPortRAM(dataRAM);



	const bool utiliseCacheMem = false;

	if (!utiliseCacheMem) {
		Sobel sobel("Sobel");

		sobel.clk(clk);
		/* à compléter */
		sobel.data(data);
		sobel.ack(ackReaderWriter);
		sobel.addressPort(address);
		sobel.requestRead(reqRead);
		sobel.requestWrite(reqWrite);

		// Démarrage de l'application
		cout << "Démarrage de la simulation." << endl;
		sc_start(-1, sc_core::sc_time_unit(sim_units));
		cout << endl << "Simulation s'est terminée à " << sc_time_stamp();
	} else {
		Sobelv2 sobel("Sobel");
		CacheMem cacheMem("CacheMem");

		/* à compléter*/

		// Démarrage de l'application
		cout << "Démarrage de la simulation." << endl;
		sc_start(-1, sc_core::sc_time_unit(sim_units));
		cout << endl << "Simulation s'est terminée à " << sc_time_stamp();

	}

	return 0;
}
