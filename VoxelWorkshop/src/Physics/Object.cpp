/*
 * Object.cpp
 * 
 * Implementation class for base physics object
 */

#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES
#include <cmath>
#include "Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
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
    indexArraySize = Object::structure.generateMesh(Object::location, Object::material);
    // Link mesh to the VAO
    Object::structure.linkMesh();
}

Object::Object(const Object& source) {
    Object::location = source.location;
    Object::movement = source.movement;
    Object::material = source.material;
    Object::structure = source.structure;
    Object::indexArraySize = source.indexArraySize;
}

Object Object::operator=(const Object& right) {
    Object result;
    result.location = right.location;
    result.movement = right.movement;
    result.material = right.material;
    result.structure = right.structure;
    result.indexArraySize = right.indexArraySize;
    return result;
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
    Object::structure.render(indexArraySize);
}

float Object::mass() const {
    return Object::structure.voxelCount() * Object::material.getDensity() * VOXEL_SIZE;
}

void Object::resetAcceleration() {
    // reset acceleration to prevent forces compounding over multiple frames
    PhysicsData resetPhysics = Object::movement;
    resetPhysics.linearAcceleration = glm::vec3(0.0, 0.0, 0.0);
    resetPhysics.rotationalAcceleration = glm::vec4(0.0, 0.0, 1.0, 0.0);
    Object::setPhysics(resetPhysics);
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

    std::string filename = *Object::structure.filename;
    filename = filename.substr(0, 15);
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
    // tick down collision cooldown
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

    // guard clauses to prevent division by 0
    if (abs(sin(angle)) < 0.1) {
        // angle is either 0 or 180deg
        if (cos(angle) > 0) {
            // no rotation
            return result;
        }
        // 180 rotation
        return glm::vec2(-result.x, -result.y);
    }

    float alpha = ((cos(angle) - 1) / sin(angle));
    float beta = sin(angle);

    glm::mat2 hSkew(1.0, alpha, 0.0, 1.0);
    glm::mat2 vSkew(1.0, 0.0, beta, 1.0);

    // first hSkew
    result = hSkew * result;
    result = glm::vec2(round(result.x), round(result.y));

    // vSkew
    result = vSkew * result;
    result = glm::vec2(round(result.x), round(result.y));

    // second vSkew
    result = hSkew * result;
    result = glm::vec2(round(result.x), round(result.y));

    return result;
}

std::bitset<14 * 14 * 14> Object::getRotatedStructure(glm::ivec3 offset = glm::ivec3(0, 0, 0)) const {
    // create result bitmask to fill in
    std::bitset<14 * 14 * 14> result;

    // get pitch (x), yaw (y), roll (z)
    glm::vec3 eulerRot;
    glm::extractEulerAngleXYZ(glm::rotate(Object::location.Rotation.w, glm::vec3(Object::location.Rotation.x, Object::location.Rotation.y, Object::location.Rotation.z)), eulerRot.x, eulerRot.y, eulerRot.z);
    glm::vec4 quat = Object::location.Rotation;

    // iterate over cells in the original structure
    for (int i = 0; i < Object::structure.solidMask.size(); ++i) {
        // skip empty cells
        if (!Object::structure.solidMask[i]) continue;

        // get local coordinates ranging from -4 to 3
        glm::ivec3 internalCoords = Object::structure.indexToCoords(i) + glm::ivec3(-4, -4, -4);

        // rotate yaw, pitch, then roll
        glm::ivec3 newCoords = internalCoords;
        //yaw
        glm::ivec2 rot = rotateViaSkew(glm::ivec2(newCoords.z, newCoords.x), eulerRot.y);
        newCoords.z = rot.x;
        newCoords.x = rot.y;
        //pitch
        rot = rotateViaSkew(glm::ivec2(newCoords.y, newCoords.z), eulerRot.x);
        newCoords.y = rot.x;
        newCoords.z = rot.y;
        //roll
        rot = rotateViaSkew(glm::ivec2(newCoords.x, newCoords.y), eulerRot.z);
        newCoords.x = rot.x;
        newCoords.y = rot.y;

        // offset coordinates to return to all positive numbers and to center on center of big bitmask plus offset
        newCoords += glm::ivec3(7, 7, 7) + offset;

        // assign bit in mask
        int index = Object::structure.coordsToIndex(newCoords, 14);
        if (index < 0 || index >= result.size()) continue;
        result[index] = true;
    }
    return result;
}

void Object::hardRotateStructure() {
    // rotate the structure
    std::bitset<14 * 14 * 14> rotated = getRotatedStructure();

    std::bitset<BRICKMAP_SIZE * BRICKMAP_SIZE * BRICKMAP_SIZE> result;
    // iterate over bits in result
    for (int i = 0; i < result.size(); ++i) {
        glm::ivec3 coords = structure.indexToCoords(i);
        // offset coords to get middle of larger bitmask
        coords += glm::ivec3(3, 3, 3);

        // convert back to an index within the larger bitmask
        int bigIndex = structure.coordsToIndex(coords, 14);

        // set the bit in the small mask to match the offset bit in the large mask
        // of note; this doesnt include bits in a 3-deep layer around the large mask
        if (bigIndex < 0 || bigIndex >= rotated.size()) continue;
        structure.solidMask[i] = rotated[bigIndex];
    }

    // Set object rotation to 0 since rotation is now baked into model
    LocationData newLoc{
        location.Position, // Position
        location.Scale, // Scale
        glm::vec4(0.0, 0.0, 1.0, 0.0)// Rotation
    };
    setTransformation(newLoc);
}

void Object::detectCollision(const Object& collider) {
    return;
}
