#include "Brickmap.h"
#include "../Renderer/EBO.h"

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

void Brickmap::linkMesh(VAO &VAO, unsigned int &indexArraySize) {
	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(&vertices[0], sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(&indices[0], sizeof(indices));

	// Links VBO attributes (like coords and color) to VAO
	VAO.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Clean up the VBO and EBO since they aren't needed anymore
	VBO1.Delete();
	EBO1.Delete();

	return;
}
