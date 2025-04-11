#include "Brickmap.h"
#include "../Renderer/VAO.h"
#include "../Renderer/VBO.h"
#include "../Renderer/EBO.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <iostream>
#include <fstream>

const unsigned int totalVolume = BRICKMAP_SIZE * BRICKMAP_SIZE * BRICKMAP_SIZE;

// Vertices coordinates
std::vector<float> vertices = {};

// Indices for vertices order
std::vector <unsigned int> indices = {};

// Path to region files
std::string pathToRegions = "src/World/Region/";

Brickmap::Brickmap() {
    return;
}

Brickmap::~Brickmap() {
    // Delete the vertex array object we've created
    VAO.Delete();
}

void Brickmap::linkMesh() {
    VAO.Bind();

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

/// @TODO update mesh generation to use matrices instead of half-baked switch statement
unsigned int Brickmap::generateMesh(const LocationData& location) {
    const float baseVertices[] =
    {//    Coordinates     //   L/R - U/D - F/B
         0.0f,  0.0f,  0.0f, // L   - D   - B
         1.0f,  0.0f,  0.0f, // R   - D   - B
         0.0f,  1.0f,  0.0f, // L   - U   - B
         1.0f,  1.0f,  0.0f, // R   - U   - B
         0.0f,  0.0f,  1.0f, // L   - D   - F
         1.0f,  0.0f,  1.0f, // R   - D   - F
         0.0f,  1.0f,  1.0f, // L   - U   - F
         1.0f,  1.0f,  1.0f  // R   - U   - F
    };

    const unsigned int baseIndices[] =
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

    // Temporary static color, @TODO replace with material color
    glm::vec3 color(0.0f, 1.0f, 0.0f);

    // Clear previous vertex/index list
    vertices = {};
    indices = {};

    for (int i = 0; i < totalVolume; ++i) {
        // Only create cubes for filled voxels
        if (Brickmap::solidMask[i]) {
            // Get the local coordinates within the brickmap
            glm::vec3 mapCoords(i / (BRICKMAP_SIZE * BRICKMAP_SIZE), (i / BRICKMAP_SIZE) % BRICKMAP_SIZE, i % BRICKMAP_SIZE);
            unsigned int baseIndex = vertices.size() / 6;

            // Calculate the voxel offset based on local coords and chunk offset
            for (int j = 0; j < sizeof(baseVertices) / sizeof(float); j += 3) {
                // Relative coords
                glm::vec4 position(
                    baseVertices[j + 0] + mapCoords.x,
                    baseVertices[j + 1] + mapCoords.y,
                    baseVertices[j + 2] + mapCoords.z,
                    1.0
                );
                // Translate so center of mass is at 0,0,0
                glm::mat4 centerOfMass = glm::translate(glm::mat4(1.0), glm::vec3(-4.0f, -4.0f, -4.0f));

                // Scale
                glm::mat4 scale = glm::scale(glm::mat4(1.0), location.Scale);

                // Rotate
                glm::mat4 rotation = glm::rotate(location.Rotation.w, glm::vec3(location.Rotation.x, location.Rotation.y, location.Rotation.z));

                // Translate
                glm::mat4 translation = glm::translate(glm::mat4(1.0), location.Position);

                // Apply transformations to vertex
                glm::mat4 transformation = translation * rotation *scale* centerOfMass;
                position = transformation * position;

                vertices.push_back(position.x);
                vertices.push_back(position.y);
                vertices.push_back(position.z);
                vertices.push_back(mapCoords.x / 8);
                vertices.push_back(mapCoords.y / 8);
                vertices.push_back(mapCoords.z / 8);
            }

            for (int j = 0; j < sizeof(baseIndices) / sizeof(unsigned int); ++j) {
                indices.push_back(baseIndices[j] + baseIndex);
            }
        }
    }
    
    return indices.size();
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
    for (int i = 0; i < totalVolume / 8; ++i) {
        unsigned char bitmask;
        file >> bitmask;
        std::bitset<8> bitInt(static_cast<unsigned int>(bitmask));

        for (int j = 0; j < 8; ++j) {
            Brickmap::solidMask.set((8 * i) + j, bitInt[j]);
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
    for (int i = 0; i < totalVolume; i += 8) {
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
