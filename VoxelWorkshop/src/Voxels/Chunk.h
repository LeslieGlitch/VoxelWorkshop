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
	Chunk();

	/// @brief Destructor for Chunk
	~Chunk();

	/// @brief Update the chunk
	/// @param dt time since last physics tick
	void Update(float dt);

	/// @brief Render the chunk
	void Render();

	static const int CHUNK_SIZE = 16;

private:
	Voxel * * * m_pVoxels;
};

#endif
