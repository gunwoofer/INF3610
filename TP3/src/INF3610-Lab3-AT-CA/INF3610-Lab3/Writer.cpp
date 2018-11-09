#include "Writer.h"

Writer::Writer(sc_module_name nom) {

}

Writer::~Writer() {

}

void Writer::thread(void) {
	unsigned int uiAddress;
	unsigned int uiData;

	while (1) {
		do {
			wait(clk->posedge_event());
		} while (!request.read());

		uiAddress = address.read();
		uiData = data.read();
		dataPortRAM->Write(uiAddress, uiData);
		ack.write(true);

		wait(clk->posedge_event());
		ack.write(false);
		
	}
}