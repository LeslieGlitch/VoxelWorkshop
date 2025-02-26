/*
 * Chunk.cpp
 *
 * Implementation file for the Chunk class
 */

#include "Chunk.h"

Chunk::Chunk(int ChunkCoords[3]) {
	// set chunk coordinates
	for (int i = 0; i < 3; ++i) {
		Coords[i] = ChunkCoords[i];
	}

	// Create a new voxel for each spot
	for (int i = 0; i < Chunk::CHUNK_SIZE; ++i) {
		for (int j = 0; j < Chunk::CHUNK_SIZE; ++j) {
			for (int k = 0; k < Chunk::CHUNK_SIZE; ++k) {
				Voxels[i][j][k] = new Voxel();
			}
		}
	}
}

Chunk::Chunk(const Chunk& orig, int ChunkCoords[3]) {
	// set chunk coordinates
	for (int i = 0; i < 3; ++i) {
		Coords[i] = ChunkCoords[i];
	}

	// set each voxel to the same as the copied chunk
	for (int i = 0; i < Chunk::CHUNK_SIZE; ++i) {
		for (int j = 0; j < Chunk::CHUNK_SIZE; ++j) {
			for (int k = 0; k < Chunk::CHUNK_SIZE; ++k) {
				Voxels[i][j][k] = new Voxel(*orig.getVoxel(i, j, k));
			}
		}
	}
}

Chunk::~Chunk() {
	for (int i = 0; i < Chunk::CHUNK_SIZE; ++i) {
		for (int j = 0; j < Chunk::CHUNK_SIZE; ++j) {
			for (int k = 0; k < Chunk::CHUNK_SIZE; ++k) {
				delete Voxels[i][j][k];
			}
		}
	}
}

Voxel* Chunk::getVoxel(int i, int j, int k) const {
	return Voxels[i][j][k];
}

void Chunk::setVoxel(Voxel* voxel, int i, int j, int k) {
	Voxels[i][j][k] = voxel;
	return;
}

void Chunk::Update(float dt) {
	return;
}

void Chunk::Render() {
	return;
}
