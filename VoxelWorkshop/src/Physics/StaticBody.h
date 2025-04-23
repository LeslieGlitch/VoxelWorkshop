/*
 * StaticBody.h
 * 
 * Include file for Static Body class
 */

#ifndef STATICBODY_H
#define STATICBODY_H

#include "Object.h"

class StaticBody : public Object {
public:
    /// @brief Default Constructor
    StaticBody();

    /// @brief Constructor with brickmap filename
    StaticBody(std::string filename);

    /// @brief Contructor with Brickmap
    StaticBody(Brickmap structure);

    /// @brief Copy Constructor
    /// @param source Object to be copied
    StaticBody(const StaticBody& source);

    StaticBody operator=(const StaticBody& right);

    /// @brief Function called when object spawns
    void start();

    /// @brief Function called once every game tick
    void update();

    static std::string getType();
};

#endif
