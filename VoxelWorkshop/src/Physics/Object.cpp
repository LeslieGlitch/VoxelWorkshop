/*
 * Object.cpp
 * 
 * Implementation class for base physics object
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include "Object.h"
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>

 // Path to object files
std::string pathToObjects = "src/World/Object/";

Object::Object() {
    Object::structure.loadFromFile("hSphere.bm");
    indexArraySize = Object::structure.generateMesh(Object::location, Object::material);
    // Link mesh to the VAO
    Object::structure.linkMesh();
}

Object::Object(std::string filename) {
    Object::structure.loadFromFile(filename);
    indexArraySize = Object::structure.generateMesh(Object::location, Object::material);
    // Link mesh to the VAO
    Object::structure.linkMesh();
}

Object::Object(Brickmap structure) {
    Object::structure = structure;
}

void Object::setTransformation(const LocationData& location) {
    Object::location = location;
    indexArraySize = Object::structure.generateMesh(Object::location, Object::material);
    // Link mesh to the VAO
    Object::structure.linkMesh();
}

void Object::setPhysics(const PhysicsData& movement) {
    Object::movement = movement;
}

void Object::setMaterial(const Material& material) {
    Object::material = material;
    indexArraySize = Object::structure.generateMesh(Object::location, Object::material);
    // Link mesh to the VAO
    Object::structure.linkMesh();
}

void Object::render() {
    Object::structure.VAO.Bind();
    // Draw primitives, number of indices, datatype of indices, index of indices
    glDrawElements(GL_TRIANGLES, indexArraySize, GL_UNSIGNED_INT, 0);
}

float Object::mass() const {
    return Object::structure.voxelCount() * Object::material.getDensity() * VOXEL_SIZE;
}

void Object::Impulse(float impulse, glm::vec3 direction, glm::vec3 offsetFromCenterOfMass) {
    return;
}

static float fileToFloat(std::ifstream& file) {
    // Read float as raw bits
    float valToReturn[1];
    file.read((char*) &valToReturn[0], sizeof(float));
    
    return valToReturn[0];
}

bool Object::loadFromFile(const std::string& fileName) {
    // Open file
    std::ifstream file(pathToObjects + fileName, std::ios::in | std::ios::binary);

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

    // Load Location Data
    LocationData readLocation;

    readLocation.Position.x = fileToFloat(file);
    readLocation.Position.y = fileToFloat(file);
    readLocation.Position.z = fileToFloat(file);

    readLocation.Scale.x = fileToFloat(file);
    readLocation.Scale.y = fileToFloat(file);
    readLocation.Scale.z = fileToFloat(file);

    readLocation.Rotation.x = fileToFloat(file);
    readLocation.Rotation.y = fileToFloat(file);
    readLocation.Rotation.z = fileToFloat(file);
    readLocation.Rotation.w = fileToFloat(file);

    Object::setTransformation(readLocation);

    // Load Physics Data
    //PhysicsData readPhysics;

    Object::movement.linearVelocity.x = fileToFloat(file);
    Object::movement.linearVelocity.y = fileToFloat(file);
    Object::movement.linearVelocity.z = fileToFloat(file);

    Object::movement.linearAcceleration.x = fileToFloat(file);
    Object::movement.linearAcceleration.y = fileToFloat(file);
    Object::movement.linearAcceleration.z = fileToFloat(file);

    Object::movement.rotationalVelocity.x = fileToFloat(file);
    Object::movement.rotationalVelocity.y = fileToFloat(file);
    Object::movement.rotationalVelocity.z = fileToFloat(file);
    Object::movement.rotationalVelocity.w = fileToFloat(file);

    Object::movement.rotationalAcceleration.x = fileToFloat(file);
    Object::movement.rotationalAcceleration.y = fileToFloat(file);
    Object::movement.rotationalAcceleration.z = fileToFloat(file);
    Object::movement.rotationalAcceleration.w = fileToFloat(file);

    //Object::setPhysics(readPhysics);

    // Load Material ID
    char matID[1];
    file.read((char*)&matID[0], 1);
    Material loadedMat(static_cast<MatList>(matID[0]));
    Object::setMaterial(loadedMat);

    // Load Brickmap from file
    char filename[15];
    file.read((char*)&filename, 15);
    std::string fileStr = (std::string)filename;
    fileStr.erase(fileStr.find(" "));
    Object::structure.loadFromFile(fileStr);
    indexArraySize = Object::structure.generateMesh(Object::location, Object::material);
    // Link mesh to the VAO
    Object::structure.linkMesh();

    // Cleanup
    file.close();
    return false;
}

static void floatToFile(float val, std::ofstream& file) {
    // Create buffer char to save floats as raw bits
    unsigned char floatToChar[sizeof(float)];

    // Save Location Data
    memcpy(floatToChar, &val, sizeof(val));
    for (int n = 0; n < sizeof(float); ++n) {
        file << floatToChar[n];
    }
}

bool Object::saveToFile(const std::string& fileName) {
    // Open file
    std::ofstream file;
    file.open(pathToObjects + fileName, std::ios::binary | std::ios::trunc);

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

    // Save Location Data
    floatToFile(Object::location.Position.x, file);
    floatToFile(Object::location.Position.y, file);
    floatToFile(Object::location.Position.z, file);

    floatToFile(Object::location.Scale.x, file);
    floatToFile(Object::location.Scale.y, file);
    floatToFile(Object::location.Scale.z, file);

    floatToFile(Object::location.Rotation.x, file);
    floatToFile(Object::location.Rotation.y, file);
    floatToFile(Object::location.Rotation.z, file);
    floatToFile(Object::location.Rotation.w, file);

    // Save Physics Data
    floatToFile(Object::movement.linearVelocity.x, file);
    floatToFile(Object::movement.linearVelocity.y, file);
    floatToFile(Object::movement.linearVelocity.z, file);

    floatToFile(Object::movement.linearAcceleration.x, file);
    floatToFile(Object::movement.linearAcceleration.y, file);
    floatToFile(Object::movement.linearAcceleration.z, file);

    floatToFile(Object::movement.rotationalVelocity.x, file);
    floatToFile(Object::movement.rotationalVelocity.y, file);
    floatToFile(Object::movement.rotationalVelocity.z, file);
    floatToFile(Object::movement.rotationalVelocity.w, file);

    floatToFile(Object::movement.rotationalAcceleration.x, file);
    floatToFile(Object::movement.rotationalAcceleration.y, file);
    floatToFile(Object::movement.rotationalAcceleration.z, file);
    floatToFile(Object::movement.rotationalAcceleration.w, file);

    // Save Material ID
    file << static_cast<unsigned char>(Object::material.getMaterial());

    // Save Brickmap filename
    std::string filename = Object::structure.filename.substr(0, 15);
    for (int i = filename.length(); i < 15; ++i) {
        filename.append(" ");
    }
    file << filename;

    // Cleanup
    file.close();
    return false;
}

void Object::start() {
    return;
}

void Object::update() {
    if (collisionCooldown > 0) collisionCooldown--;
    return;
}

std::string Object::getType() {
    return "Object";
}

static glm::vec3 quat2Euler(glm::vec4 q) {
    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);

    // pitch (y-axis rotation)
    double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
    double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);

    return glm::vec3(
        2 * std::atan2(sinp, cosp) - M_PI / 2, // pitch (x)
        std::atan2(siny_cosp, cosy_cosp),      // yaw (y)
        std::atan2(sinr_cosp, cosr_cosp)       // roll (z)
    );
}


// assumes rotation about (0, 0) in CCW direction
static glm::vec2 rotateViaSkew(const glm::vec2& origPos, const float& angle) {
    glm::vec2 result = origPos;

    // compute skew for each step
    float skewH = -tan(angle / 2);
    float skewV = sin(angle);

    // horizontal skew
    result.x = result.x + round(skewH * result.y);

    // vertical skew
    result.y = result.y + round(skewV * result.x);

    // repeat horizontal skew
    result.x = result.x + round(skewH * result.y);

    return result;
}

std::bitset<14 * 14 * 14> Object::getRotatedStructure(glm::ivec3 offset = glm::ivec3(0, 0, 0)) const {
    // create result bitmask to fill in
    std::bitset<14 * 14 * 14> result;
    
    // iterate over cells in the original structure
    for (int i = 0; i < Object::structure.solidMask.size(); ++i) {
        // skip empty cells
        if (!Object::structure.solidMask[i]) continue;

        // get pitch (x), yaw (y), roll (z)
        glm::vec3 eulerRot = quat2Euler(Object::location.Rotation);
        glm::vec4 quat = Object::location.Rotation;

        //std::cout << "Quat : (" << quat.x << ", " << quat.y << ", " << quat.z << ", " << quat.w << ")\n";
        //std::cout << "Euler: (" << eulerRot.x << ", " << eulerRot.y << ", " << eulerRot.z << ")\n\n";

        // get local coordinates ranging from -4 to 3
        glm::ivec3 internalCoords = Object::structure.indexToCoords(i) + glm::ivec3(-4, -4, -4);

        // rotate yaw, pitch, then roll
        glm::ivec3 newCoords = internalCoords;
        //yaw
        glm::ivec2 rot = rotateViaSkew(glm::ivec2(newCoords.z, newCoords.x), eulerRot.y);
        newCoords.z = rot.x;
        newCoords.x = rot.y;
        //pitch
        rot = rotateViaSkew(glm::ivec2(newCoords.z, newCoords.y), eulerRot.x);
        newCoords.z = rot.x;
        newCoords.y = rot.y;
        //roll
        rot = rotateViaSkew(glm::ivec2(newCoords.x, newCoords.y), eulerRot.z);
        newCoords.x = rot.x;
        newCoords.y = rot.y;

        //std::cout << "Old Coords: (" << internalCoords.x << ", " << internalCoords.y << ", " << internalCoords.z << ")\n";
        //std::cout << "New Coords: (" << newCoords.x << ", " << newCoords.y << ", " << newCoords.z << ")\n\n";

        // offset coordinates to center on offset
        newCoords += glm::ivec3(7, 7, 7) + offset;

        // assign bit in mask
        int index = Object::structure.coordsToIndex(newCoords, 14);
        if (index >= 0 && index < result.size())
            result[index] = true;
    }
    return result;
}

void Object::detectCollision(const Object& collider) {
    return;
}
