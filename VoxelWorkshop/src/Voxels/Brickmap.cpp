#define GLM_ENABLE_EXPERIMENTAL

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
std::string pathToRegions = "src/World/Brickmap/";

Brickmap::Brickmap() {
    return;
}

Brickmap::Brickmap(const Brickmap& source) {
    Brickmap::solidMask = source.solidMask;
    Brickmap::VAO.ID = source.VAO.ID;
}

Brickmap::~Brickmap() {
    // Delete the vertex array object we've created
    VAO.Delete();
}

glm::ivec3 Brickmap::indexToCoords(const int& index, const int& sideLength) {
    // Guard clause, index out of range
    if (index < 0 || index > sideLength * sideLength * sideLength) {
        return glm::vec3(-1.0, -1.0, -1.0);
    }

    return glm::vec3(
        index / (sideLength * sideLength),
        (index / sideLength) % sideLength,
        index % sideLength
    );
}

int Brickmap::coordsToIndex(const glm::ivec3& coords, const int& sideLength) {
    // Guard clauses, coords out of range
    if (coords.x < 0 || coords.x > sideLength) {
        return -1;
    }
    if (coords.y < 0 || coords.y > sideLength) {
        return -1;
    }
    if (coords.z < 0 || coords.z > sideLength) {
        return -1;
    }

    return (coords.x * sideLength * sideLength) + (coords.y * sideLength) + (coords.z);
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

unsigned int Brickmap::generateMesh(const LocationData& location, const Material& material) {
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
        0, 3, 2, // Back 1
        0, 1, 3, // Back 2
        0, 2, 6, // Left 1
        0, 6, 4, // Left 2
        0, 4, 1, // Down 1
        1, 4, 5, // Down 2
        4, 6, 5, // Front 1
        5, 6, 7, // Front 2
        1, 5, 3, // Right 1
        3, 5, 7, // Right 2
        2, 3, 7, // Up 1
        2, 7, 6  // Up 2
    };

    // Get material color
    glm::vec3 color = material.getColor();

    // Clear previous vertex/index list
    vertices = {};
    indices = {};

    for (int i = 0; i < totalVolume; ++i) {
        // Only create cubes for filled voxels
        if (Brickmap::solidMask[i]) {
            // Get the local coordinates within the brickmap
            glm::vec3 mapCoords = indexToCoords(i, BRICKMAP_SIZE);
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
                // Translate so center of object is at 0,0,0
                glm::mat4 centerOfMass = glm::translate(glm::mat4(1.0), glm::vec3(-4.0f, -4.0f, -4.0f));

                // Scale so full object is 1m^3
                glm::mat4 downScale = glm::scale(glm::mat4(1.0), glm::vec3(0.125, 0.125, 0.125));

                // Scale
                glm::mat4 scale = glm::scale(glm::mat4(1.0), location.Scale);

                // Rotate
                glm::mat4 rotation = glm::rotate(location.Rotation.w, glm::vec3(location.Rotation.x, location.Rotation.y, location.Rotation.z));

                // Translate
                glm::mat4 translation = glm::translate(glm::mat4(1.0), location.Position);

                // Apply transformations to vertex
                glm::mat4 transformation = translation * rotation * scale * downScale * centerOfMass;
                position = transformation * position;

                // Add vertex position to buffer
                vertices.push_back(position.x);
                vertices.push_back(position.y);
                vertices.push_back(position.z);

                // Add voxel color to buffer with slight offset based on relative coordinates
                vertices.push_back(color.r + (float(i % 3) - 1) / 50);
                vertices.push_back(color.g + (float(i % 3) - 1) / 50);
                vertices.push_back(color.b + (float(i % 3) - 1) / 50);
            }

            /// @TODO Cull faces that face into another voxel
            for (int j = 0; j < sizeof(baseIndices) / sizeof(unsigned int); ++j) {
                indices.push_back(baseIndices[j] + baseIndex);
            }
        }
    }
    
    return indices.size();
}

void Brickmap::render(const unsigned int& indexArraySize) {
    VAO.Bind();
    glDrawElements(GL_TRIANGLES, indexArraySize, GL_UNSIGNED_INT, 0);
}

int Brickmap::voxelCount() const {
    return solidMask.count();
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

    // Update filename
    *Brickmap::filename = fileName;

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
