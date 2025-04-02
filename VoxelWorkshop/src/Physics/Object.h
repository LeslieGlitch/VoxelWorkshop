/*
 * Object.h
 * 
 * Include file for base physics object
 */

#include <glm/glm.hpp>

class Object {
public:
	// Cartesian Coordinates
	glm::vec3 Position, Velocity, Acceleration;
	// Quaternions to prevent gimbal lock
	glm::vec4 Rotation, AngVelocity, angAcceleration;

	/// @brief Function called when object spawns
	void start();

	/// @brief Function called once every game tick
	void update();
};