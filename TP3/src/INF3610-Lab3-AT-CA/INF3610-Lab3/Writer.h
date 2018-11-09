#pragma once

#include <systemc.h>
#include "LMBIF.h"

class Writer: sc_module {
	public:
		sc_in_clk clk;
		sc_in<unsigned int> address;
		sc_in<unsigned int> data;
		sc_in<bool> request;
		sc_out<bool> ack;
		sc_port<LMBIF> dataPortRAM;

		Writer(sc_module_name nom);

		~Writer();

	private:
		SC_HAS_PROCESS(Writer);

		void thread(void);

};