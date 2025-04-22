/*
 * RenderingEngine.cpp
 * 
 * Runs a loop at a configurable framerate that handles rendering a scene
 */

// Turn on GLM experiments
#define GLM_ENABLE_EXPERIMENTAL

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
#include "../Physics/PhysicsEngine.hpp"

const unsigned int screenSize[] = { 1280, 720 };

namespace Render {
    void ShowMenuBar();
    void ShowSceneGui();
    void ShowObjectGui();

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

        /* imgui */

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();

        /* Shaders */

        // Generates Shader object using shaders defualt.vert and default.frag
        Shader shaderProgram("src/Shaders/default.vert", "src/Shaders/default.frag");

        // Enable depth buffer
        glEnable(GL_DEPTH_TEST);

        Camera camera(screenSize[0], screenSize[1], glm::vec3(2.0f, 2.0f, 2.0f));

        // Test object for movement
        RigidBody Disk("hBox.bm");
        LocationData location{
            glm::vec3(10.0, 0.0, 0.0), // Position
            glm::vec3(1.0, 1.0, 1.0), // Scale
            glm::vec4(0.0, 0.0, 1.0, 1.0)// Rotation
        };
        PhysicsData movement{
            glm::vec3(0.0, 0.0, 9.0), // Linear Velocity
            glm::vec3(0.0, 0.0, 0.01), // Linear Acceleration
            glm::vec4(0.0, 0.0, 0.0, 1.0), // Rotational Velocity
            glm::vec4(0.0, 0.0, 0.0, 1.0), // Rotational Acceleration
        };
        Disk.setTransformation(location);
        Disk.setPhysics(movement);
        Disk.setMaterial(clay);

        RigidBody Ball("hSphere.bm");
        location = {
            glm::vec3(-10.0, 0.0, 0.0), // Position
            glm::vec3(1.0, 1.0, 1.0), // Scale
            glm::vec4(1.0, 1.0, 1.0, 1.0)// Rotation
        };
        movement = {
            glm::vec3(0.0, 0.0, 9.0), // Linear Velocity
            glm::vec3(0.0, 0.0, 0.01), // Linear Acceleration
            glm::vec4(1.0, 1.0, 1.0, 0.0), // Rotational Velocity
            glm::vec4(1.0, 1.0, 1.0, 0.0), // Rotational Acceleration
        };
        Ball.setTransformation(location);
        Ball.setPhysics(movement);
        Ball.setMaterial(dirt);

        std::cout << "Disk Mass: " << Disk.mass() << "\n";
        std::cout << "Ball Mass: " << Ball.mass() << "\n\n";

        bool firstClickL = true;
        bool firstClickR = true;
        bool isPhysicsTicking = false;

        /* Render Loop */
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // Start the DearImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // GUI windows
            ShowMenuBar(); // Save/Load scene and play/pause/stop simulation
            ShowSceneGui(); // Objects in active scene
            ShowObjectGui(); // Object properties

            // Specify the color of the background
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            // Clean the back buffer and assign the new color to it
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Tell OpenGL which Shader Program we want to use
            shaderProgram.Activate();
            // Collect inputs from user
            camera.Inputs(window);
            // Transform coordinates based on camera position
            camera.Matrix(45.0f, 0.1f, 1000.0f, shaderProgram, "camMatrix");

            // toggle physics
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (firstClickL) {
                    isPhysicsTicking = !isPhysicsTicking;
                    firstClickL = false;
                }

            }
            else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
                firstClickL = true;
            }

            if (isPhysicsTicking) {
                Disk.update();
                Ball.update();

                Disk.detectCollision(Ball);
                Ball.detectCollision(Disk);

                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                    if (firstClickR) {
                        Disk.Impulse(200.0f, glm::vec3(0.0, 1.0, 0.0), glm::vec3(2.0, 0.0, 0.0));
                        Ball.Impulse(200.0f, glm::vec3(0.0, 1.0, 0.0), glm::vec3(2.0, 0.0, 0.0));
                        firstClickR = false;
                    }

                }
                else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
                    firstClickR = true;
                }
            }

            Disk.render();
            Ball.render();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap the back buffer with the front buffer
            glfwSwapBuffers(window);
        }

        /* Cleanup */
        Physics::quitPhysics();
        shaderProgram.Delete();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        // Delete window before ending the program
        glfwDestroyWindow(window);
        // Terminate GLFW before ending the program
        glfwTerminate();
        return 0;
    }

    void ShowMenuBar() {
        // Menu Bar
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::MenuItem("(demo menu)", NULL, false, true);
                if (ImGui::MenuItem("Save")) {}
                if (ImGui::MenuItem("Load")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Demos")) {
                if (ImGui::MenuItem("Orbits", NULL, false, true)) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        return;
    }

    void ShowSceneGui() {
        // Main Body of window
        if (!ImGui::Begin("Scene View", NULL)) {
            ImGui::End();
            return;
        }

        ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

        // Text
        ImGui::Text("A list of objects in the scene");

        ///@TODO pull list of objects from current scene, display here

        // End and return
        ImGui::End();
        return;
    }

    void ShowObjectGui() {
        // Main Body of window
        if (!ImGui::Begin("Object View", NULL)) {
            ImGui::End();
            return;
        }

        ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

        ///@TODO Take currently selected object and display properties

        // Text
        ImGui::Text("A list of properties on the object");

        // End and return
        ImGui::End();
        return;
    }
}
