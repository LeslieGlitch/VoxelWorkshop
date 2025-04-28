/*
 * RenderingEngine.cpp
 * 
 * Runs a loop at a configurable framerate that handles rendering a scene
 */

// Turn on GLM experiments
#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

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

#include "../Physics/Scene.h"

const unsigned int screenSize[] = { 1280, 720 };

namespace Render {
    //void ShowMenuBar(Scene&);
    void ShowSceneGui(Scene&);
    void ShowObjectGui(Scene&);

    // GUI globals
    Object* selectedObj = nullptr;
    LocationData customLoc{
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::angleAxis(0.1f, glm::vec3(0.0f, 1.0f, 0.0f))
    };
    PhysicsData customPhy{
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f))
    };
    Material customMat;
    std::string structureFile = "Box.bm";
    std::string rigidLabel[16];
    std::string staticLabel[16];
    static int gravityType = 0;
    glm::vec3 gravPoint = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 gravDir = glm::vec3(0.0f, -1.0f, 0.0f);

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
        ImGui_ImplOpenGL3_Init("#version 330 core");

        /* Shaders */

        // Generates Shader object using shaders defualt.vert and default.frag
        Shader shaderProgram("src/Shaders/default.vert", "src/Shaders/default.frag");

        // Enable depth buffer
        glEnable(GL_DEPTH_TEST);

        Camera camera(screenSize[0], screenSize[1], glm::vec3(0.0f, 0.0f, 15.0f));

        // Test Scene
        Scene currentScene;

        bool firstClickL = true;
        bool firstClickR = true;
        bool isPhysicsTicking = false;

        /* Render Loop */
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // Specify the color of the background
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            // Clean the back buffer and assign the new color to it
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Start the DearImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // GUI windows
            //ShowMenuBar(currentScene); // Save/Load scene and play/pause/stop simulation
            ShowSceneGui(currentScene); // Objects in active scene
            ShowObjectGui(currentScene); // Object properties

            // Tell OpenGL which Shader Program we want to use
            shaderProgram.Activate();
            // Collect inputs from user
            camera.Inputs(window);
            // Transform coordinates based on camera position
            camera.Matrix(45.0f, 0.1f, 1000.0f, shaderProgram, "camMatrix");

            // Stop past frame impulses from acting on current frame
            currentScene.resetAllAcceleration();

            // toggle physics
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (firstClickL) {
                    firstClickL = false;
                }

            }
            else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
                firstClickL = true;
            }

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                if (firstClickR) {
                    //currentScene.rigidBodies.at(diskIndex).Impulse(2000.0f, glm::vec3(0.0, 1.0, 0.0), glm::vec3(4.0, 0.0, 0.0));
                    //currentScene.rigidBodies.at(sphereIndex).Impulse(2000.0f, glm::vec3(0.0, 1.0, 0.0), glm::vec3(4.0, 0.0, 0.0));

                    selectedObj->hardRotateStructure();
                    //Ball.hardRotateStructure();

                    //std::cout << glm::distance(Disk.location.Position, Ball.location.Position) << "\n";

                    //std::cout << "Location: (" << selectedObj->location.Position.x << ", " << selectedObj->location.Position.y << ", " << selectedObj->location.Position.z << ")\n";
                    //std::cout << "Rotation: (" << selectedObj->location.Rotation.w << ", " << selectedObj->location.Rotation.x << ", " << selectedObj->location.Rotation.y << ", " << selectedObj->location.Rotation.z << ")\n";
                    //std::cout << "RotVel  : (" << selectedObj->movement.rotationalVelocity.w << ", " << selectedObj->movement.rotationalVelocity.x << ", " << selectedObj->movement.rotationalVelocity.y << ", " << selectedObj->movement.rotationalVelocity.z << ")\n";

                    if (selectedObj == nullptr) {
                        std::cout << "No Object Selected\n";
                    }
                    else {
                        std::cout << selectedObj->getLabel() << "\n";
                    }
                    std::cout << currentScene.rigidBodies.size() << "\n\n";

                    firstClickR = false;
                }

            }
            else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
                firstClickR = true;
            }

            switch (gravityType) {
            case 0:
                //No gravity
                break;
            case 1:
                // Directional gravity
                currentScene.gravitateAllInDirection(gravDir);
                break;
            case 2:
                // Point gravity
                currentScene.gravitateAllToPoint(gravPoint);
                break;
            }

            currentScene.detectAllCollisions();
            currentScene.updateAll();

            currentScene.renderAll();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap the back buffer with the front buffer
            glfwSwapBuffers(window);
        }

        /* Cleanup */
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

    /*
    void ShowMenuBar(Scene& currentScene) {
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
    */
    void ShowSceneGui(Scene& currentScene) {
        // Main Body of window
        if (!ImGui::Begin("Scene View", NULL)) {
            ImGui::End();
            return;
        }

        ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

        // Scene play/pause/stop
        if (ImGui::Button("Play")) {
            currentScene.play();
        }
        ImGui::SameLine();
        if (ImGui::Button("Pause")) {
            currentScene.pause();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            currentScene.reset();
        }

        ImGui::Separator();
        if (ImGui::Button("Deselect")) {
            selectedObj = nullptr;
            customLoc = {
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::angleAxis(0.1f, glm::vec3(0.0f, 1.0f, 0.0f))
            };
            customPhy = {
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
                glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f))
            };
            customMat = stone;
            structureFile = "Box.bm";
        }

        // Only show rigid bodies if there are some to show
        if (!currentScene.rigidBodies.empty()) {
            // List objects in scene
            ImGui::SeparatorText("Rigid Bodies");
            for (int i = 0; i < currentScene.rigidBodies.size(); i++)
            {
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV((float)i / currentScene.rigidBodies.size(), 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV((float)i / currentScene.rigidBodies.size(), 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV((float)i / currentScene.rigidBodies.size(), 0.8f, 0.8f));
                char buffer[32];
                if (currentScene.rigidBodies.at(i).label == nullptr) {
                    rigidLabel[i] = "NewRigidObject";
                    currentScene.rigidBodies.at(i).setLabel(&rigidLabel[i]);
                }
                std::sprintf(buffer, "%s%d", currentScene.rigidBodies.at(i).getLabel().c_str(), i);
                if (ImGui::Button(buffer)) {
                    selectedObj = &currentScene.rigidBodies.at(i);
                    customLoc = selectedObj->location;
                    customPhy = selectedObj->movement;
                    customMat = selectedObj->material;
                    rigidLabel[i] = selectedObj->getLabel();
                    structureFile = *selectedObj->structure.filename;
                }
                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
        }

        // Only show static bodies if there are some to show
        if (!currentScene.staticBodies.empty()) {
            // List objects in scene
            ImGui::SeparatorText("Static Bodies");
            for (int i = 0; i < currentScene.staticBodies.size(); i++)
            {
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV((float)i / currentScene.staticBodies.size(), 0.6f, 0.6f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV((float)i / currentScene.staticBodies.size(), 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV((float)i / currentScene.staticBodies.size(), 0.8f, 0.8f));
                char buffer[32];
                if (currentScene.staticBodies.at(i).label == nullptr) {
                    staticLabel[i] = "NewStaticObject";
                    currentScene.staticBodies.at(i).setLabel(&staticLabel[i]);
                }
                std::sprintf(buffer, "%s%d", currentScene.staticBodies.at(i).getLabel().c_str(), i);
                if (ImGui::Button(buffer)) {
                    selectedObj = &currentScene.staticBodies.at(i);
                    customLoc = selectedObj->location;
                    customPhy = selectedObj->movement;
                    customMat = selectedObj->material;
                    staticLabel[i] = selectedObj->getLabel();
                    structureFile = *selectedObj->structure.filename;
                }
                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
        }

        // Gravity controls
        ImGui::SeparatorText("Gravity");
        if (ImGui::RadioButton("No Gravity", &gravityType, 0));
        if (ImGui::RadioButton("Vector Gravity", &gravityType, 1));
        if (ImGui::RadioButton("Point Gravity", &gravityType, 2));
        switch (gravityType) {
        case 0:
            break;
        case 1:
            ImGui::Text("Gravity Direction");
            ImGui::InputFloat("x", &gravDir.x);
            ImGui::InputFloat("y", &gravDir.y);
            ImGui::InputFloat("z", &gravDir.z);
            break;
        case 2:
            ImGui::Text("Gravity Center Location");
            ImGui::InputFloat("x", &gravPoint.x);
            ImGui::InputFloat("y", &gravPoint.y);
            ImGui::InputFloat("z", &gravPoint.z);
            break;
        }

        // End and return
        ImGui::End();
        return;
    }

    void ShowObjectGui(Scene& currentScene) {
        // Main Body of window
        if (!ImGui::Begin("Object View", NULL)) {
            ImGui::End();
            return;
        }

        ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

        int index = 0;

        // No selected object
        if (selectedObj == nullptr) {
            ImGui::SeparatorText("New Object");

            // Select Static vs Rigid
            static int objectType = 1;
            ImGui::RadioButton("Static Body", &objectType, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Rigid Body", &objectType, 1);

            // Spawn Object
            if (ImGui::Button("Spawn Object")) {
                std::cout << "obj added";
                if (objectType == 0) {
                    index = currentScene.newStatic(structureFile);
                    selectedObj = &(currentScene.staticBodies.at(index));
                }
                else {
                    index = currentScene.newRigid(structureFile);
                    selectedObj = &(currentScene.rigidBodies.at(index));
                }

                selectedObj->setTransformation(customLoc);
                selectedObj->setPhysics(customPhy);
                selectedObj->setMaterial(customMat);

                selectedObj->start();
            }
        }
        else { // Selected object
            ImGui::SeparatorText(selectedObj->getLabel().c_str());

            // Update Object
            if (ImGui::Button("Update parameters")) {
                selectedObj->setPhysics(customPhy);
                selectedObj->structure.loadFromFile(structureFile);
                selectedObj->setMaterial(customMat);
                selectedObj->setTransformation(customLoc);
            }
        }

        // Location Data
        ImGui::SeparatorText("Position");
        ImGui::InputFloat("x##POS", &customLoc.Position.x);
        ImGui::InputFloat("y##POS", &customLoc.Position.y);
        ImGui::InputFloat("z##POS", &customLoc.Position.z);

        ImGui::SeparatorText("Scale");
        ImGui::InputFloat("x##SCL", &customLoc.Scale.x);
        ImGui::InputFloat("y##SCL", &customLoc.Scale.y);
        ImGui::InputFloat("z##SCL", &customLoc.Scale.z);

        ImGui::SeparatorText("Rotation");
        ImGui::InputFloat("w##ROT", &customLoc.Rotation.w);
        ImGui::InputFloat("x##ROT", &customLoc.Rotation.x);
        ImGui::InputFloat("y##ROT", &customLoc.Rotation.y);
        ImGui::InputFloat("z##ROT", &customLoc.Rotation.z);

        // Physics Data
        ImGui::SeparatorText("Linear Velocity");
        ImGui::InputFloat("x##LVL", &customPhy.linearVelocity.x);
        ImGui::InputFloat("y##LVL", &customPhy.linearVelocity.y);
        ImGui::InputFloat("z##LVL", &customPhy.linearVelocity.z);

        ImGui::SeparatorText("Linear Acceleration");
        ImGui::InputFloat("x##LAC", &customPhy.linearAcceleration.x);
        ImGui::InputFloat("y##LAC", &customPhy.linearAcceleration.y);
        ImGui::InputFloat("z##LAC", &customPhy.linearAcceleration.z);

        ImGui::SeparatorText("Rotational Velocity");
        ImGui::InputFloat("w##RVL", &customPhy.rotationalVelocity.w);
        ImGui::InputFloat("x##RVL", &customPhy.rotationalVelocity.x);
        ImGui::InputFloat("y##RVL", &customPhy.rotationalVelocity.y);
        ImGui::InputFloat("z##RVL", &customPhy.rotationalVelocity.z);

        ImGui::SeparatorText("Rotational Acceleration");
        ImGui::InputFloat("w##RAC", &customPhy.rotationalAcceleration.w);
        ImGui::InputFloat("x##RAC", &customPhy.rotationalAcceleration.x);
        ImGui::InputFloat("y##RAC", &customPhy.rotationalAcceleration.y);
        ImGui::InputFloat("z##RAC", &customPhy.rotationalAcceleration.z);

        // Material
        ImGui::SeparatorText("Material");
        const char* matItems[] = {
            "stone",
            "wood",
            "foliage",
            "iron",
            "gold",
            "cloth",
            "dirt",
            "clay"
        };
        static int matSelectedIndex = 0;
        if (ImGui::BeginListBox("##Material")) {
            for (int i = 0; i < IM_ARRAYSIZE(matItems); ++i) {
                const bool isSelected = (matSelectedIndex == i);
                if (ImGui::Selectable(matItems[i], isSelected)) {
                    matSelectedIndex = i;
                    customMat = (MatList)i;
                }
            }

            ImGui::EndListBox();
        }

        // Shape
        ImGui::SeparatorText("Voxel Shape");
        const char* items[] = {
            "Box.bm",
            "hBox.bm",
            "Sphere.bm",
            "hSphere.bm",
            "Disk.bm",
            "hDisk.bm"
        };
        static int selectedIndex = 0;
        if (ImGui::BeginListBox("##Shape")) {
            for (int i = 0; i < IM_ARRAYSIZE(items); ++i) {
                const bool isSelected = (selectedIndex == i);
                if (ImGui::Selectable(items[i], isSelected)) {
                    selectedIndex = i;
                    structureFile = items[i];
                }
            }

            ImGui::EndListBox();
        }

        // End and return
        ImGui::End();
        return;
    }
}
