/*
 * Scene.h
 *
 * Include file for Scene class
 */

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "StaticBody.h"
#include "RigidBody.h"

class Scene {
public:
    bool isRunning = false;

    std::vector<StaticBody> staticBodies;
    std::vector<RigidBody> rigidBodies;

    // Constructor
    Scene();

    // Spawn new static object
    unsigned int newStatic();
    unsigned int newStatic(std::string filename);
    unsigned int newStatic(Brickmap brickmap);

    // Spawn new rigid object
    unsigned int newRigid();
    unsigned int newRigid(std::string filename);
    unsigned int newRigid(Brickmap brickmap);

    // Controls
    void play();
    void pause();
    void reset();

    // Perform action on all objects
    void renderAll();
    void startAll();
    void updateAll();
    void gravitateAllToPoint(glm::vec3 attractor);
    void gravitateAllInDirection(glm::vec3 direction);
    void detectAllCollisions();
    void resetAllAcceleration();

    // File actions
    void saveToFolder(std::string directory);
    void loadFromFolder(std::string directory);
};

#endif
