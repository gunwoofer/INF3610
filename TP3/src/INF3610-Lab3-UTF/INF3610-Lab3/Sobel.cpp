///////////////////////////////////////////////////////////////////////////////
//
//	Sobel.cpp
//
///////////////////////////////////////////////////////////////////////////////
#include "Sobel.h"


///////////////////////////////////////////////////////////////////////////////
//
//	Constructeur
//
///////////////////////////////////////////////////////////////////////////////
Sobel::Sobel( sc_module_name name )
: sc_module(name)
{
	SC_THREAD(thread); 
}


///////////////////////////////////////////////////////////////////////////////
//
//	Destructeur
//
///////////////////////////////////////////////////////////////////////////////
Sobel::~Sobel()
{
}


///////////////////////////////////////////////////////////////////////////////
//
//	thread
//
///////////////////////////////////////////////////////////////////////////////
void Sobel::thread(void)
{
	// Variable
	unsigned int address = 0;
	unsigned int imgWidth, imgHeight;

	while (true) {
		imgWidth = readPort->Read(address);
		address += 4;
		imgHeight = readPort->Read(address);
		unsigned int imgSize = imgWidth*imgHeight;

		//Create array
		uint8_t * image = new uint8_t[imgSize];
		uint8_t * result = new uint8_t[imgSize];
		int * imageAsInt = reinterpret_cast<int*>(image);
		int * resultAsInt = reinterpret_cast<int*>(result);

		for (unsigned int i = 0; i < imgSize/sizeof(int); i++) {
			//Request element
			address += 4;
			imageAsInt[i] = readPort->Read(address);
		}

		//For simplicity, assume that the borders don't contain edges
		for (unsigned int i = 0; i < imgWidth; ++i)
			result[i] = 0;
		for (unsigned int i = imgSize - imgWidth; i < imgSize; ++i)
			result[i] = 0;
		for (unsigned int i = 0; i < imgSize; i += imgWidth)
			result[i] = 0;
		for (unsigned int i = imgWidth - 1; i < imgSize; i += imgWidth)
			result[i] = 0;

		//Calling the operator for each pixel
		for (unsigned int i = 1; i < imgHeight - 1; ++i) {
			for (unsigned int j = 1; j < imgWidth - 1; ++j) {
				int fullIndex = i * imgWidth + j;
				result[fullIndex] = sobel_operator(fullIndex, imgWidth, image);
			}
		}

		//Write back nb. elements at the end
		address = 4;
		for (unsigned int i = 0; i < imgSize / sizeof(int); i++) {
			//Write each element
			address += 4;
			writePort->Write(address, resultAsInt[i]);
		}

		delete(image);
		delete(result);
		sc_stop();
		wait();
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//	sobel_operator
//
///////////////////////////////////////////////////////////////////////////////
static inline uint8_t getVal(int index, int xDiff, int yDiff, int img_width, uint8_t * Y) 
{ 
	return Y[index + (yDiff * img_width) + xDiff]; 
};

uint8_t Sobel::sobel_operator(const int index, const int imgWidth, uint8_t * image)
{
	int x_weight = 0;
	int y_weight = 0;

	unsigned edge_weight;
	uint8_t edge_val;

	const char x_op[3][3] = {	{ -1,0,1 },
								{ -2,0,2 },
								{ -1,0,1 } };

	const char y_op[3][3] = {	{ 1,2,1 },
								{ 0,0,0 },
								{ -1,-2,-1 } };

	//Compute approximation of the gradients in the X-Y direction
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
		// X direction gradient
		x_weight = x_weight + (getVal(index, i - 1, j - 1, imgWidth, image) * x_op[i][j]);

		// Y direction gradient
		y_weight = y_weight + (getVal(index, i - 1, j - 1, imgWidth, image) * y_op[i][j]);
		}
	}

	edge_weight = std::abs(x_weight) + std::abs(y_weight);

	edge_val = (255 - (uint8_t)(edge_weight));

	//Edge thresholding
	if (edge_val > 200)
		edge_val = 255;
	else if (edge_val < 100)
		edge_val = 0;

	return edge_val;
}
