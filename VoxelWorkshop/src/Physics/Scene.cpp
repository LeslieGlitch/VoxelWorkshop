/*
 * Scene.cpp
 *
 * Implementation file for Scene class
 */

#include "Scene.h"

// Constructor
Scene::Scene() {
    staticBodies.reserve(16);
    rigidBodies.reserve(16);
}

// Spawn new static object
unsigned int Scene::newStatic() {
    staticBodies.emplace_back();
    return staticBodies.size() - 1;
}

unsigned int Scene::newStatic(std::string filename) {
    staticBodies.emplace_back(filename);
    return staticBodies.size() - 1;
}

unsigned int Scene::newStatic(Brickmap brickmap) {
    staticBodies.emplace_back(brickmap);
    return staticBodies.size() - 1;
}

// Spawn new rigid object
unsigned int Scene::newRigid() {
    rigidBodies.emplace_back();
    return rigidBodies.size() - 1;
}

unsigned int Scene::newRigid(std::string filename) {
    rigidBodies.emplace_back(filename);
    return rigidBodies.size() - 1;
}

unsigned int Scene::newRigid(Brickmap brickmap) {
    rigidBodies.emplace_back(brickmap);
    return rigidBodies.size() - 1;
}

// Controls
void Scene::play() {
    isRunning = true;
}

void Scene::pause() {
    isRunning = false;
}

void Scene::reset() {
    isRunning = false;
    loadFromFolder("currentScene/");
}

// Perform action on all objects
void Scene::renderAll() {
    for (int i = 0; i < staticBodies.size(); ++i) {
        staticBodies.at(i).render();
    }

    for (int i = 0; i < rigidBodies.size(); ++i) {
        rigidBodies.at(i).render();
    }
}

void Scene::startAll() {
    for (int i = 0; i < staticBodies.size(); ++i) {
        staticBodies.at(i).structure.VAO.genArray();
        staticBodies.at(i).start();
    }

    for (int i = 0; i < rigidBodies.size(); ++i) {
        rigidBodies.at(i).structure.VAO.genArray();
        rigidBodies.at(i).start();
    }
}

void Scene::updateAll() {
    if (!isRunning) return;

    for (int i = 0; i < staticBodies.size(); ++i) {
        staticBodies.at(i).update();
    }

    for (int i = 0; i < rigidBodies.size(); ++i) {
        rigidBodies.at(i).update();
    }
}

void Scene::detectAllCollisions() {
    if (!isRunning) return;

    for (int i = 0; i < rigidBodies.size(); ++i) {
        for (int j = 0; j < staticBodies.size(); ++j) {
            rigidBodies.at(i).detectCollision(staticBodies.at(j));
        }

        for (int j = 0; j < rigidBodies.size(); ++j) {
            if (i == j) continue;
            rigidBodies.at(i).detectCollision(rigidBodies.at(j));
        }
    }
}

void Scene::resetAllAcceleration() {
    if (!isRunning) return;

    for (int i = 0; i < staticBodies.size(); ++i) {
        staticBodies.at(i).resetAcceleration();
    }

    for (int i = 0; i < rigidBodies.size(); ++i) {
        rigidBodies.at(i).resetAcceleration();
    }
}

// File actions
void Scene::saveToFolder(std::string directory) {

}

void Scene::loadFromFolder(std::string directory) {

}
