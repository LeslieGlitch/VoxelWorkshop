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
            std::cout << "Failed to create GLFW window" << std::endl;
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

        /* Shaders */

        // Generates Shader object using shaders defualt.vert and default.frag
        Shader shaderProgram("src/Shaders/default.vert", "src/Shaders/default.frag");

        // Generates Vertex Array Object
        VAO VAO1;

        // Collect mesh from a brickmap
        Brickmap map;
        unsigned int indexArraySize;

        map.loadFromFile("region0.txt");
        //map.saveToFile("region0.txt");

        // Generates a mesh and links it to the VAO
        map.linkMesh(VAO1, glm::vec3(0.0, 0.0, 0.0), indexArraySize);

        // Enable depth buffer
        glEnable(GL_DEPTH_TEST);

        Camera camera(screenSize[0], screenSize[1], glm::vec3(0.0f, 0.0f, 2.0f));

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

            // Bind the VAO so OpenGL knows to use it
            VAO1.Bind();
            // Draw primitives, number of indices, datatype of indices, index of indices
            glDrawElements(GL_TRIANGLES, indexArraySize/sizeof(int), GL_UNSIGNED_INT, 0);
            // Swap the back buffer with the front buffer
            glfwSwapBuffers(window);
            // Take care of all GLFW events
            glfwPollEvents();
        }

        /* Cleanup */

        // Delete the vertex array object we've created
        VAO1.Delete();
        
        shaderProgram.Delete();
        // Delete window before ending the program
        glfwDestroyWindow(window);
        // Terminate GLFW before ending the program
        glfwTerminate();
        return 0;
    }
}
