/*
 * Object.h
 * 
 * Include file for base physics object
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "../Voxels/Brickmap.h"
#include "../Voxels/Material.h"

const float VOXEL_SIZE = 1.0f / (8 * 8 * 8);

struct PhysicsData {
    glm::vec3 linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 linearAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 rotationalVelocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 rotationalAcceleration = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
};

class Object {
protected:
    const float delta = 1.0f / 60.0f;

    float collisionCooldown = 0;
public:
    // Where the object is and is going
    LocationData location;
    PhysicsData movement;

    // Object Material
    Material material = Material(stone);

    // The brickmap model
    Brickmap structure;

    // Index array size for rendering
    unsigned int indexArraySize = 0;

    /// @brief Default Constructor
    Object();

    /// @brief Constructor with brickmap filename
    Object(std::string filename);

    /// @brief Contructor with Brickmap
    Object(Brickmap structure);

    /// @brief Updates the object position and regenerates the mesh
    /// @param position new position for the object
    void setTransformation(const LocationData& location);

    /// @brief Updates the object's velocity and acceleration
    /// @param movement new movement for the object
    void setPhysics(const PhysicsData& movement);

    void setMaterial(const Material& material);

    /// @brief Renders the game object
    void render();

    /// @brief Get the mass of the object
    /// @return Mass of the object
    float mass() const;

    /// @brief Apply a force impulse to the object
    /// @param impulse Total impulse in N*s
    /// @param direction The direction of the force applied
    /// @param offsetFromCenterOfMass Where the force is applied relative to the center of mass
    void Impulse(float impulse, glm::vec3 direction, glm::vec3 offsetFromCenterOfMass = glm::vec3(0.0, 0.0, 0.0));

    /// @brief Loads an object from disk
    /// @param fileName name of the file to be loaded
    /// @return status, false is success and true is failure
    bool loadFromFile(const std::string& fileName);

    /// @brief Saves an object to disk
    /// @param fileName name of the file to save to
    /// @return status, false is success and true is failure
    bool saveToFile(const std::string& fileName);

    /// @brief Function called when object spawns
    void start();

    /// @brief Function called once every game tick
    void update();

    static std::string getType();

    /// @brief Detects if there is a collision between self and another object, applies force to itself
    /// @param collider Object to test collision with
    void detectCollision(const Object& collider);

    /// @brief Rotates the bitmap via 3 skews and returns in a larger bitmap
    /// @return a 14x14x14 bitset of the rotated structure
    std::bitset<14 * 14 * 14> getRotatedStructure(glm::ivec3) const;
};

#endif
