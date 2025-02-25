/*
 * Chunk.h
 *
 * Header file for the chunk class
 */

#ifndef CHUNK_H
#define CHUNK_H

#include "Voxel.h"

class Chunk {
public:
	/// @brief Constructor for Chunk
	Chunk(int ChunkCoords[3]);

	/// @brief Copy Constructor for Chunk
	/// @param orig Chunk to be copied
	Chunk(Chunk orig, int ChunkCoords[3]);

	/// @brief Destructor for Chunk
	~Chunk();

	/// @brief Gets a pointer to a voxel within a chunk
	/// @param i X position within chunk
	/// @param j Y position within chunk
	/// @param k Z position within chunk
	/// @return Pointer to specified voxel
	Voxel* getVoxel(int i, int j, int k);

	/// @brief Replaces a voxel from the chunk with a new voxel
	/// @param voxel Pointer to voxel to be emplaced
	/// @param i X position within chunk
	/// @param j Y position within chunk
	/// @param k Z position within chunk
	void setVoxel(Voxel* voxel, int i, int j, int k);

	/// @brief Update the chunk
	/// @param dt time since last physics tick
	void Update(float dt);

	/// @brief Render the chunk
	void Render();

	static const int CHUNK_SIZE = 16;

private:
	int Coords[3];
	Voxel* Voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
};

#endif
