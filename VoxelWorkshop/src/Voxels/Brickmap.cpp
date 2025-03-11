#include "Brickmap.h"

// Vertices coordinates
float vertices[] =
{//    Coordinates		/     Colors           /   L/R - U/D - F/B
	-0.5f,  0.0f, -0.5f,   0.00f, 0.00f, 0.00f, // L   - D   - B
	 0.5f,  0.0f, -0.5f,   0.99f, 0.00f, 0.00f, // R   - D   - B
	-0.5f,  1.0f, -0.5f,   0.00f, 0.99f, 0.00f, // L   - U   - B
	 0.5f,  1.0f, -0.5f,   0.99f, 0.99f, 0.00f, // R   - U   - B
	-0.5f,  0.0f,  0.5f,   0.00f, 0.00f, 0.99f, // L   - D   - F
	 0.5f,  0.0f,  0.5f,   0.99f, 0.00f, 0.99f, // R   - D   - F
	-0.5f,  1.0f,  0.5f,   0.00f, 0.99f, 0.99f, // L   - U   - F
	 0.5f,  1.0f,  0.5f,   0.99f, 0.99f, 0.99f  // R   - U   - F
};

// Indices for vertices order
unsigned int indices[] =
{
	0, 3, 2,
	0, 1, 3,
	0, 2, 6,
	0, 6, 4,
	0, 4, 1,
	1, 4, 5,
	4, 6, 5,
	5, 6, 7,
	1, 5, 3,
	3, 5, 7,
	2, 3, 7,
	2, 7, 6
};

miniMesh Brickmap::generateMesh() {
	miniMesh result{
		result.vertexArrayStart = &vertices[0],
		result.vertexArraySize = sizeof(vertices),
		result.indexArrayStart = &indices[0],
		result.indexArraySize = sizeof(indices)
	};
	
	return result;
}
