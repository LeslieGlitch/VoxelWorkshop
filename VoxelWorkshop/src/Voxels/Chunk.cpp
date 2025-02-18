/*
 * Chunk.cpp
 *
 * Implementation file for the Chunk class
 */

#include "Chunk.h"

Chunk::Chunk() {
	m_pVoxels = new Voxel * *[Chunk::CHUNK_SIZE];
	for (int i = 0; i < Chunk::CHUNK_SIZE; ++i) {
		m_pVoxels[i] = new Voxel * [Chunk::CHUNK_SIZE];
		for (int j = 0; j < Chunk::CHUNK_SIZE; ++j) {
			m_pVoxels[i][j] = new Voxel[Chunk::CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk() {
	for (int i = 0; i < Chunk::CHUNK_SIZE; ++i) {
		for (int j = 0; j < Chunk::CHUNK_SIZE; ++j) {
			delete[] m_pVoxels[i][j];
		}
		delete[] m_pVoxels[i];
	}
	delete[] m_pVoxels;
}

void Chunk::Update(float dt) {
	return;
}

void Chunk::Render() {
	return;
}
