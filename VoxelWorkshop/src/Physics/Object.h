/*
 * Object.h
 * 
 * Include file for base physics object
 */

#include <glm/glm.hpp>
#include "../Voxels/Brickmap.h"

struct PhysicsData {
    glm::vec3 linearVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 linearAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 rotationalVelocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 rotationalAcceleration = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
};

class Object {
public:
    // Where the object is and is going
    LocationData location;
    PhysicsData movement;

    // The brickmap model
    Brickmap structure;

    /// @brief Default Constructor
    Object();

    /// @brief Contructor with Brickmap
    Object(Brickmap structure);

    /// @brief Updates the object position and regenerates the mesh
    /// @param position new position for the object
    void setPosition(const LocationData& location);

    /// @brief Renders the game object
    void render();

    /// @brief Function called when object spawns
    void start();

    /// @brief Function called once every game tick
    void update();
};