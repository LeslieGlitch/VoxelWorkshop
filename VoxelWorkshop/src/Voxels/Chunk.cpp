/*
 * Chunk.cpp
 *
 * Implementation file for the Chunk class
 */

#include "Chunk.h"
#include <iostream>

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
                Voxels[i][j][k]->isActive(false);
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
    std::cout << "\nBegin rendering chunk\n";
    int voxelsRendered = 0;
    int voxelsSkipped = 0;

    // iterate over each voxel
    for (int i = 0; i < Chunk::CHUNK_SIZE; ++i) {
        for (int j = 0; j < Chunk::CHUNK_SIZE; ++j) {
            for (int k = 0; k < Chunk::CHUNK_SIZE; ++k) {
                // guard clause: is voxel active?
                if (!Voxels[i][j][k]->isActive()) {
                    // skip to next voxel
                    ++voxelsSkipped;
                    continue;
                }
                ++voxelsRendered;

                // get data
                int* color = Voxels[i][j][k]->getColor();
                float r = color[0] / 255.0f;
                float g = color[1] / 255.0f;
                float b = color[2] / 255.0f;

                float x = Coords[0] * Chunk::CHUNK_SIZE + i;
                float y = Coords[1] * Chunk::CHUNK_SIZE + j;
                float z = Coords[2] * Chunk::CHUNK_SIZE + k;

                GLfloat vertices[] = {
                    x + 1, y + 1, z + 1,
                    x,     y + 1, z + 1,
                    x + 1, y + 1, z,
                    x,     y + 1, z,
                    x + 1, y,     z + 1,
                    x,     y,     z + 1,
                    x,     y,     z,
                    x + 1, y,     z,

                };

                GLuint elements[] = {
                    3, 2, 6, 7, 4, 2, 0,
                    3, 1, 6, 5, 4, 1, 0
                };
            }
        }
    }

    std::cout << "Rendered:\t" << voxelsRendered << "voxels\n"
        << "Skipped:\t" << voxelsSkipped << "voxels\n";

    // end draw call
    glEnd();
    
    return;
}
