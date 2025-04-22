/**
 *  Brickmap.h
 * 
 *  Header file for brickmap class
 *  Brickmap voxel structure invented by Thijs van Wingerden https://studenttheses.uu.nl/handle/20.500.12932/20460
 */

#ifndef BRICKMAP_H
#define BRICKMAP_H

#include <bitset>
#include <glm/glm.hpp>
#include "VisualData.h"
#include "../Voxels/Material.h"
#include "../Renderer/VAO.h"

// Size taken from brickmap paper
const int BRICKMAP_SIZE = 8;

struct LocationData {
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec4 Rotation = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
};

class Brickmap {
public:
    std::bitset<BRICKMAP_SIZE * BRICKMAP_SIZE * BRICKMAP_SIZE> solidMask;
    Brickmap();
    ~Brickmap();
    static glm::ivec3 indexToCoords(const int& index, const int& sideLength = BRICKMAP_SIZE);
    static int coordsToIndex(const glm::ivec3& coords, const int& sideLength = BRICKMAP_SIZE);
    VAO VAO;
    std::string filename = "hSphere.bm";
    unsigned int generateMesh(const LocationData &location, const Material& material);
    void linkMesh();
    int voxelCount() const;

    bool loadFromFile(const std::string &fileName);
    bool saveToFile(const std::string &fileName);
};

#endif
