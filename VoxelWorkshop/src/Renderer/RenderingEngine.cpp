/*
 * RenderingEngine.cpp
 * 
 * Runs a loop at a configurable framerate that handles rendering a scene
 */

// Turn on GLM experiments
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderingEngine.hpp"
#include "Shader.h"
#include "VAO.h"
#include "Camera.h"

#include "../Voxels/Brickgrid.h"
#include "../Physics/Object.h"

Material stone("stone", glm::vec3(0.82, 0.82, 0.82));
Material wood("wood", glm::vec3(0.87, 0.72, 0.53));
Material foliage("foliage", glm::vec3(0.13, 0.55, 0.13));
Material iron("iron", glm::vec3(0.6, 0.6, 0.6));
Material gold("gold", glm::vec3(1.0, 0.84, 0.0));
Material cloth("cloth", glm::vec3(0.94, 0.90, 0.55));
Material dirt("dirt", glm::vec3(0.55, 0.27, 0.08));
Material clay("clay", glm::vec3(0.68, 0.85, 0.90));

const unsigned int screenSize[] = { 1280, 720 };

namespace Render {
    // local variables
    const GLdouble pi = 3.1415926535897932384626433832795;

    bool render(int framerate, int windowSize[]) {
        /* Initialization */

        // Initialize GLFW
        glfwInit();

        // Tell GLFW what version of OpenGL we are using 
        // In this case we are using OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // Tell GLFW we are using the CORE profile
        // So that means we only have the modern functions
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a GLFWwindow object, naming it "Voxel Workshop"
        GLFWwindow* window = glfwCreateWindow(screenSize[0], screenSize[1], "Voxel Workshop", NULL, NULL);
        // Error check if the window fails to create
        if (window == NULL)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        // Introduce the window into the current context
        glfwMakeContextCurrent(window);

        // Set framerate
        glfwSwapInterval(1);

        //Load GLAD so it configures OpenGL
        gladLoadGL();

        // Specify the viewport of OpenGL in the Window
        glViewport(0, 0, screenSize[0], screenSize[1]);

        // Enable culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);

        /* Shaders */

        // Generates Shader object using shaders defualt.vert and default.frag
        Shader shaderProgram("src/Shaders/default.vert", "src/Shaders/default.frag");

        // Enable depth buffer
        glEnable(GL_DEPTH_TEST);

        Camera camera(screenSize[0], screenSize[1], glm::vec3(6.0f, 6.0f, 6.0f));

        // Test object for movement
        Object hSphere;
        LocationData location{
            glm::vec3(-4.0, -4.0, -4.0), // Position
            glm::vec3(1.0, 1.0, 1.0), // Scale
            glm::vec4(1.0, 0.0, 0.0, 0.0)// Rotation
        };
        hSphere.setTransformation(location);
        hSphere.setMaterial(clay);
        double time = 0.0;

        /* Render Loop */
        while (!glfwWindowShouldClose(window))
        {
            // Specify the color of the background
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            // Clean the back buffer and assign the new color to it
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Tell OpenGL which Shader Program we want to use
            shaderProgram.Activate();
            // Collect inputs from user
            camera.Inputs(window);
            // Transform coordinates based on camera position
            camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

            // Bob test object up and down
            time += 0.01;
            hSphere.setTransformation({
            glm::vec3(0.0,  glm::sin(time), 0.0), // Position
            glm::vec3(1.0 + glm::sin(time) / 2, 1.0 + glm::sin(time) / 2, 1.0 + glm::sin(time) / 2), // Scale
            glm::vec4(0.0, 1.0, 0.0, glm::sin(time))// Rotation
                });

            hSphere.render();
            // Swap the back buffer with the front buffer
            glfwSwapBuffers(window);
            // Take care of all GLFW events
            glfwPollEvents();
        }

        /* Cleanup */
        
        shaderProgram.Delete();
        // Delete window before ending the program
        glfwDestroyWindow(window);
        // Terminate GLFW before ending the program
        glfwTerminate();
        return 0;
    }
}
