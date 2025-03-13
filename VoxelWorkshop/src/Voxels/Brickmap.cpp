#include "Brickmap.h"
#include "../Renderer/VBO.h"
#include "../Renderer/EBO.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>

// Vertices coordinates
std::vector<float> vertices = {};

// Indices for vertices order
std::vector <unsigned int> indices = {};

// Path to region files
std::string pathToRegions = "src/World/Region/";

// Friend function declaration
void generateMesh(const glm::vec3 &offset);

void Brickmap::linkMesh(VAO &VAO, const glm::vec3 &offset, unsigned int &indexArraySize) {
	VAO.Bind();

	// Generates the mesh to be bound
	generateMesh(offset);
	
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(&vertices[0], vertices.size() * sizeof(float));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(&indices[0], indices.size() * sizeof(unsigned int));

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

void generateMesh(const glm::vec3& offset) {
	float exampleVertices[] =
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

	unsigned int exampleIndices[] =
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

	for (int i = 0; i < sizeof(exampleVertices) / sizeof(float); ++i) {
		// Determine if value is X, Y, Z, or color
		float coordOffset = 0;
		switch (i % 6) {
		case 0:
			// X
			coordOffset = offset.x;
			break;
		case 1:
			// Y
			coordOffset = offset.y;
			break;
		case 2:
			// Z
			coordOffset = offset.z;
			break;
		default:
			// Color
			break;
		}

		vertices.push_back(exampleVertices[i] + coordOffset);
	}

	for (int i = 0; i < sizeof(exampleIndices) / sizeof(unsigned int); ++i) {
		indices.push_back(exampleIndices[i]);
	}

	return;
}

bool Brickmap::loadFromFile(const std::string& fileName) {
	// Open file
	std::ifstream file(pathToRegions + fileName);

	// Ensure file is open
	if (!file.is_open()) {
		std::cerr << "Error: File \"" << fileName << "\" failed to open!\n";

		if (file.bad()) {
			std::cerr << "Error: Badbit is set\n";
		}
		if (file.fail()) {
			std::cerr << std::strerror(errno) << "\n";
		}

		return true;
	}

	// Clear old solidMask data
	Brickmap::solidMask.reset();

	// Load new solidMask data
	for (int i = 0; i < BRICKMAP_SIZE * BRICKMAP_SIZE * BRICKMAP_SIZE; i += 8) {
		unsigned char bitmask;
		file >> bitmask;
		std::bitset<8> bitInt(static_cast<unsigned int>(bitmask));

		for (int j = 0; j < 8; ++j) {
			Brickmap::solidMask.set(i + j, bitInt[j]);
		}
	}

	// Cleanup
	file.close();
	return false;
}

bool Brickmap::saveToFile(const std::string &fileName) {
	// Open file
	std::ofstream file;
	file.open(pathToRegions + fileName, std::ios::binary | std::ios::trunc);

	// Ensure file is open
	if (!file.is_open()) {
		std::cerr << "Error: File \"" << fileName << "\" failed to open!\n";

		if (file.bad()) {
			std::cerr << "Error: Badbit is set\n";
		}
		if (file.fail()) {
			std::cerr << std::strerror(errno) << "\n";
		}

		return true;
	}

	// Fill with solidMask data
	for (int i = 0; i < BRICKMAP_SIZE * BRICKMAP_SIZE * BRICKMAP_SIZE; i += 8) {
		std::bitset<8> bitMask;
		for (int j = 0; j < 7; ++j) {
			bitMask.set(j, Brickmap::solidMask[i + j]);
		}
		file << static_cast<unsigned char>(bitMask.to_ulong());
	}

	// Cleanup
	file.close();
	return false;
}
