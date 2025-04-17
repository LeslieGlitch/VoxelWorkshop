/*
 * Object.cpp
 * 
 * Implementation class for base physics object
 */

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
    return;
}
