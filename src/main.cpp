#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "texture.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

#include <../thirdparty/nlohmann/json.hpp>

#include "Core/Node.h"
#include "Core/Time.h"
#include "Core/Input.h"
#include "Component/BlockManager.h"
#include "Component/MeshRenderer.h"
#include "Component/StaticBlockController.h"
#include "Component/PlayerController.h"
#include "Component/Camera.h"
#include "Component/InstanceRenderer.h"
#include "Component/Skybox.h"
#include "Component/ShadowMap.h"
#include "Component/Rotate.h"
#include "Component/Disabler.h"


#include "Managers/ComponentsManager.h"
#include "Managers/SceneManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/NodesManager.h"
#include "Managers/LightsManager.h"
#include "Gui/ImguiMain.h"
#include "ParticleGenerator.h"

#include "Enemies/EnemiesManager.h"

#include "HUD/HUDMain.h"

#include <iostream>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include "Managers/ComponentsManager.h"
#include "Managers/GameManager.h"
#include "Managers/AudioEngineManager.h"
#include "HUD/PageManager.h"
#include "Managers/DomeManager.h"
#include "Managers/UpgradeManager.h"
#include "Turrets/TurretsManager.h"
#include "Managers/TutorialManager.h"
#include "Component/ShovelController.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GAMEMANAGER.Init();
    if (GAMEMANAGER._window == NULL)
        return 1;
    glfwMakeContextCurrent(GAMEMANAGER._window);
    glfwSwapInterval(1); // Enable VSync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
    if (err)
    {
        spdlog::error("Failed to initialize OpenGL loader!");
        return 1;
    }
    spdlog::info("Successfully initialized OpenGL loader!");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_STENCIL_TEST);
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);

    AUDIOENGINEMANAGER.Init();

    // Deserialization of resources and nodes
    GAMEMANAGER.root = SCENEMANAGER.LoadFromJsonFile("../../scenes/test1.json");

    RESOURCEMANAGER.GetShaderByName("skyboxShader")->use();
    RESOURCEMANAGER.GetShaderByName("skyboxShader")->setInt("skybox", 0);

    //NodesManager::getInstance().getNodeByName("player")->GetTransform()->SetPosition(glm::vec3(0.0f, 115.0f, 0.0f));

    Skybox skybox;
    skybox.init();

    // Init
    INPUT.Init(GAMEMANAGER._window, SCR_WIDTH, SCR_HEIGHT);
    GAMEMANAGER.root->Init();

    //ShadowMap init
    ShadowMap shadowMap;
    shadowMap.Init();
    shadowMap.AssignShadowMapToShader();

    glm::vec3 initialCloudPosition(0.0f, 0.0f, 0.0f);
    float cloudSpeed = 8.0f;

    //Directional Light Properties
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    float dirColor[3] = { 0.999f, 0.999f, 1.00f };
    float dirDirection[3] = { -0.5f, -0.5f, -0.5f };
    bool dirActive = true;

    //Shadowmap Creation POV
    glm::vec3 lightPos(49.999f, 350.0f, 40.0f);
    glm::vec3 lightCenter(50.0f, 300.0f,50.0f);

    glm::vec3 windDirection(1.0f,0.0f,0.0f);
    float windStrength = 0.0f;

    bool _renderWireframeBB = false;

    ComponentsManager::getInstance().GetComponentByID<Camera>(2)->setScreenWidth(SCR_WIDTH);
    ComponentsManager::getInstance().GetComponentByID<Camera>(2)->setScreenHeight(SCR_HEIGHT);

    std::shared_ptr<ImguiMain> imguiMain = std::make_shared<ImguiMain>(GAMEMANAGER._window, glsl_version);
    imguiMain->SetRoot(GAMEMANAGER.root);
    imguiMain->SetSelectedObject(GAMEMANAGER.root);

    /*    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), "testowy");
    COMPONENTSMANAGER.CreateComponent<Camera>();
    NODESMANAGER.getNodeByName("testowy")->AddComponent(COMPONENTSMANAGER.GetComponentByID<Camera>(4));*/
    // json save for testing
    nlohmann::json jsonData = SCENEMANAGER.SerializeRoot(GAMEMANAGER.root);
    //std::cout << jsonData;
    SCENEMANAGER.SaveToJsonFile(jsonData, "../../scenes/test2.json");

    HUD.Init();

/*    RESOURCEMANAGER.GetSoundByName("BackgroundMusic")->SetLooping(true);
    RESOURCEMANAGER.GetSoundByName("BackgroundMusic")->PlaySound();*/
    //NODESMANAGER.getNodeByName("Enemy1")->GetComponent<MeshRenderer>()->_shouldRenderOutline = true;

    ENEMIESMANAGER.Init();

    //Prepare shader pointers
    auto skyboxShader = RESOURCEMANAGER.GetShaderByName("skyboxShader");
    auto modelShader = RESOURCEMANAGER.GetShaderByName("modelShader");
    auto outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    auto skyboxReflectionShader = RESOURCEMANAGER.GetShaderByName("skyboxReflectionShader");
    auto instancedSandShader = RESOURCEMANAGER.GetShaderByName("instancedSandShader");
    auto instancedMetalShader = RESOURCEMANAGER.GetShaderByName("instancedMetalShader");
    auto lightObjectShader = RESOURCEMANAGER.GetShaderByName("lightObjectShader");
    auto cloudShader = RESOURCEMANAGER.GetShaderByName("cloudShader");
    auto particleShader = RESOURCEMANAGER.GetShaderByName("particleShader");
    auto rangeShader = RESOURCEMANAGER.GetShaderByName("turretRangeShader");
    auto shovelShader = RESOURCEMANAGER.GetShaderByName("shovelShader");
    auto blueprintShader = RESOURCEMANAGER.GetShaderByName("blueprintShader");

    PAGEMANAGER.Init();

    TURRETSMANAGER.Init();

    TUTORIALMANAGER.Init();

    LIGHTSMANAGER.InitLights();

    auto shovelController = NODESMANAGER.getNodeByName("Shovel")->GetComponent<ShovelController>();;
    auto shovelRenderer = NODESMANAGER.getNodeByName("Shovel")->GetComponent<ShovelRenderer>();;

    // Main loop
    while (!glfwWindowShouldClose(GAMEMANAGER._window))
    {
        //debugging adding money
        if(INPUT.IsKeyPressed(77)){
            GAMEMANAGER._money += 10;
            //std::cout << "Aktualny stan portfela " << GAMEMANAGER._money << std::endl;
        }

        if(INPUT.IsKeyPressed(GLFW_KEY_RIGHT_BRACKET) && !TUTORIALMANAGER._isTutorialEnded){
            NODESMANAGER.getNodeByName("waveSymbol1")->GetTransform()->SetPosition(glm::vec3(
                    ENEMIESMANAGER._spawnersPositions[0][0], 0, ENEMIESMANAGER._spawnersPositions[0][1]));
            NODESMANAGER.getNodeByName("waveSymbol1")->GetTransform()->SetPosition(glm::vec3(
                    ENEMIESMANAGER._spawnersPositions[1][1], 0, ENEMIESMANAGER._spawnersPositions[0][1]));
            TUTORIALMANAGER._isTutorialEnded = true;
            NODESMANAGER.getNodeByName("player")->GetComponent<PlayerController>()->_activeMineEntranceCollision = false;
        }

        if(!GAMEMANAGER._paused){
            ENEMIESMANAGER.Update();
            TURRETSMANAGER.Update();
            DOMEMANAGER.Update();
            GAMEMANAGER.Update();
        }

        UPGRADEMANAGER.Update();

        TUTORIALMANAGER.Update();

        // Calculate deltaTime
        TIME.Update();

        // Input
        GAMEMANAGER.root->Input();

        // Update
        GAMEMANAGER.root->Update();
        GAMEMANAGER.Update();

/*        if(TIME.GetTime() > 10 && TIME.GetTime() < 11){
            std::cout << "minal czas";
            NODESMANAGER.getNodeByName("AntModel")->GetTransform()->SetPosition(glm::vec3(0.0f, 115.0f, 0.0f));
        }*/

        // Start the Dear ImGui frame
        imguiMain->draw();

        ImGui::Begin("Depth Map");
        ImGui::SetWindowSize(ImVec2(300, 300), ImGuiCond_Once);
        ImGui::Image((void*)(intptr_t)shadowMap.GetDepthMapTexture(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        // Quick Debug
        ImGui::Checkbox("Flashlight", &LIGHTSMANAGER.isSpotActive);

        ImGui::InputFloat3("Light Position", &lightPos[0]);  // Change lightPos
        ImGui::InputFloat3("Center", &lightCenter[0]);

        ImGui::ColorEdit3("Directional Light Color", dirColor);

        ImGui::Checkbox("Wireframe Frustum Boxes", &_renderWireframeBB);

        ImGui::SliderFloat3("Wind Direction", &windDirection[0],-1.0f,1.0f);
        ImGui::SliderFloat("Wind Strength", &windStrength, -10.0f, 10.0f);

        FrustumCulling::_renderWireframeBB = _renderWireframeBB;

        // Applying clear color
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glDepthMask(GL_FALSE);

        GAMEMANAGER._windDirection = normalize(windDirection);
        GAMEMANAGER._windStrength = windStrength;

        RESOURCEMANAGER.GetShaderByName("skyboxShader")->use();

        glm::mat4 projection = glm::perspective(glm::radians(ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetViewMatrix();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetViewMatrix())); // remove translation from the view matrix
        skyboxShader->setMat4("view", skyboxView);
        skyboxShader->setMat4("projection", projection);

        skybox.draw();

        //ShadowMap Creation
        glm::vec3 shadowDir = glm::normalize(lightCenter - lightPos);
        dirDirection[0] = shadowDir.x;
        dirDirection[1] = shadowDir.y;
        dirDirection[2] = shadowDir.z;

        float near_plane = 0.2f, far_plane = 150.0f;
        glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(lightPos, lightCenter, glm::vec3(0.0, 1.0, 0.0));
        shadowMap.SetLightProjection(lightProjection);
        shadowMap.SetLightView(lightView);
        shadowMap.AssignLightSpaceMatrixToShader();

        glDepthMask(GL_TRUE);

        shadowMap.BeginRender();
        shadowMap.RenderMap();
        shadowMap.EndRender();

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); //Reset Viewport After Rendering to Shadow Map

        LIGHTSMANAGER.UpdateLights();

        modelShader->use();
        modelShader->setVec3("dirLight.direction", dirDirection);
        modelShader->setVec3("dirLight.color", dirColor);
        modelShader->setInt("dirLight.isActive", dirActive);

        modelShader->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        modelShader->setMat4("projection", projection);
        modelShader->setMat4("view", view);
        modelShader->setVec3("lightPos", lightPos);
        modelShader->setMat4("lightSpaceMatrix", shadowMap.GetLightSpaceMatrix());

        outlineShader->use();
        outlineShader->setVec3("dirLight.direction", dirDirection);
        outlineShader->setVec3("dirLight.color", dirColor);
        outlineShader->setInt("dirLight.isActive", dirActive);

        outlineShader->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        outlineShader->setMat4("projection", projection);
        outlineShader->setMat4("view", view);

        skyboxReflectionShader->use();
        skyboxReflectionShader->setInt("skybox", 0);
        skyboxReflectionShader->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        skyboxReflectionShader->setMat4("projection", projection);
        skyboxReflectionShader->setMat4("view", view);

#pragma region InstancedSandShader setup
        instancedSandShader->use();
        instancedSandShader->setVec3("dirLight.direction", dirDirection);
        instancedSandShader->setVec3("dirLight.color", dirColor);
        instancedSandShader->setInt("dirLight.isActive", dirActive);

        instancedSandShader->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        instancedSandShader->setMat4("projection", projection);
        instancedSandShader->setMat4("view", view);
        instancedSandShader->setVec3("lightPos", lightPos);
        instancedSandShader->setMat4("lightSpaceMatrix", shadowMap.GetLightSpaceMatrix());
#pragma endregion

#pragma region InstanceMetalShader setup
        instancedMetalShader->use();
        instancedMetalShader->setVec3("dirLight.direction", dirDirection);
        instancedMetalShader->setVec3("dirLight.color", dirColor);
        instancedMetalShader->setInt("dirLight.isActive", dirActive);

        instancedMetalShader->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        instancedMetalShader->setMat4("projection", projection);
        instancedMetalShader->setMat4("view", view);
        instancedMetalShader->setVec3("lightPos", lightPos);
        instancedMetalShader->setMat4("lightSpaceMatrix", shadowMap.GetLightSpaceMatrix());

        lightObjectShader->use();
        lightObjectShader->setVec3("lightColor", dirColor);
        lightObjectShader->setMat4("projection", projection);
        lightObjectShader->setMat4("view", view);
#pragma endregion

        cloudShader->use();
        cloudShader->setVec3("dirLight.direction", dirDirection);
        cloudShader->setVec3("dirLight.color", dirColor);
        cloudShader->setInt("dirLight.isActive", dirActive);

        cloudShader->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        cloudShader->setVec3("initialCloudPosition", initialCloudPosition);
        cloudShader->setFloat("cloudSpeed", cloudSpeed);
        cloudShader->setFloat("time", TIME.GetTime());
        cloudShader->setMat4("projection", projection);
        cloudShader->setMat4("view", view);

        particleShader->use();
        particleShader->setMat4("projection", projection);
        particleShader->setMat4("view", view);
        particleShader->setVec3("dirColor", dirColor);

        rangeShader->use();
        rangeShader->setMat4("projection", projection);
        rangeShader->setMat4("view", view);

        ///
        blueprintShader->use();

        blueprintShader->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        blueprintShader->setMat4("projection", projection);
        blueprintShader->setMat4("view", view);
        blueprintShader->setVec3("additionalColor", TURRETSMANAGER._additionalColor);

        GAMEMANAGER.root->Render(Transform::Origin());

        // RealUpdate is after standard Update and has to be here if you want smooth rotation!
        // This order (shovelRender -> shovelController -> shovelShader) is working and the place
        // (at the end of loop) is also working, so it is recommended to not move it
        // moving may cause some problems like lack of smoothness or models bugging
        shovelRenderer->RenderShovel(Transform::Origin());
        shovelController->RealUpdate();

        shovelShader->use();
        shovelShader->setVec3("dirLight.direction", dirDirection);
        shovelShader->setVec3("dirLight.color", dirColor);
        shovelShader->setInt("dirLight.isActive", dirActive);

        shovelShader->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        shovelShader->setMat4("projection", projection);
        shovelShader->setMat4("view", view);
        shovelShader->setVec3("lightPos", lightPos);
        shovelShader->setMat4("lightSpaceMatrix", shadowMap.GetLightSpaceMatrix());

        HUD.Update();
        PAGEMANAGER.Update();

        AUDIOENGINEMANAGER.Update();

        imguiMain->endDraw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        INPUT.UpdateOldStates();
        glfwSwapBuffers(GAMEMANAGER._window);
        glfwPollEvents();
    }
    // Cleanup
    AUDIOENGINEMANAGER.Cleanup();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(GAMEMANAGER._window);
    glfwTerminate();


    return 0;
}