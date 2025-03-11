/**
 *  Brickmap.h
 * 
 *  Header file for brickmap class
 *  Brickmap voxel structure invented by Thijs van Wingerden https://studenttheses.uu.nl/handle/20.500.12932/20460
 */

#ifndef BRICKMAP_H
#define BRICKMAP_H

#include <bitset>
#include "VisualData.h"
#include "../Renderer/VAO.h"

// Size taken from brickmap paper
const int BRICKMAP_SIZE = 8;

struct miniMesh {
	float* vertexArrayStart;
	unsigned int vertexArraySize;
	unsigned int* indexArrayStart;
	unsigned int indexArraySize;
};

class Brickmap {
private:
	std::bitset<BRICKMAP_SIZE * BRICKMAP_SIZE * BRICKMAP_SIZE> solidMask;
	VisualData* __ptr32 shadingAttribute;
	unsigned char colorLOD[3];

	friend void generateMesh();

public:
	void linkMesh(VAO &VAO, unsigned int &indexArraySize);
};

#endif
