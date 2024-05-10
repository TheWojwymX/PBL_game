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

#include "Managers/ComponentsManager.h"
#include "Managers/SceneManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/NodesManager.h"
#include "Gui/ImguiMain.h"

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SandBOX", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
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
    INPUT.Init(window, SCR_WIDTH, SCR_HEIGHT);
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
    glm::vec3 lightPos(50.0f, 120.0f, 40.0f);
    glm::vec3 lightCenter(50.0f, 100.0f,50.0f);

    //SpotLight Properties
    bool isSpotActive = true;
    glm::vec3 spotLightCurrentPosition = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition();
    glm::vec3 spotLightCurrentDirection = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetFrontVector();
    glm::vec3 spotLightColor(1.0f);
    float spotLightConstant = 1.0f;
    float spotLightLinear = 0.1f;
    float spotLightQuadratic = 0.03f;
    float spotLightCutOff = 16.5f;
    float spotLightOuterCutOff = 20.5f;
    bool _renderWireframeBB = false;

    ComponentsManager::getInstance().GetComponentByID<Camera>(2)->setScreenWidth(SCR_WIDTH);
    ComponentsManager::getInstance().GetComponentByID<Camera>(2)->setScreenHeight(SCR_HEIGHT);

    std::shared_ptr<ImguiMain> imguiMain = std::make_shared<ImguiMain>(window, glsl_version);
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

    shared_ptr<Model> antModel = make_shared<Model>("../../res/Models/Ant/ant_walk_0.1-0.obj", "Ant");

    //NODESMANAGER.getNodeByName("Enemy1")->GetComponent<MeshRenderer>()->_shouldRenderOutline = true;

    ENEMIESMANAGER.Init();

    PAGEMANAGER.Init();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        ENEMIESMANAGER.Update();

        // Calculate deltaTime
        TIME.Update();

        // Input
        GAMEMANAGER.root->Input();

        // Update
        GAMEMANAGER.root->Update();

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
        ImGui::Checkbox("Flashlight", &isSpotActive);
        ImGui::ColorEdit3("Spot light Color", glm::value_ptr(spotLightColor));
        ImGui::InputFloat("Spot light Strength", &spotLightQuadratic);

        ImGui::InputFloat3("Light Position", &lightPos[0]);  // Change lightPos
        ImGui::InputFloat3("Center", &lightCenter[0]);

        ImGui::ColorEdit3("Directional Light Color", dirColor);

        ImGui::Checkbox("Wireframe Frustum Boxes", &_renderWireframeBB);

        //Przypisz _renderWireframeBB do MeshRenderera modelu, aby widzieć jego bounding box
        ComponentsManager::getInstance().GetComponentByID<MeshRenderer>(6)->_renderWireframeBB = _renderWireframeBB;
        ComponentsManager::getInstance().GetComponentByID<MeshRenderer>(8)->_renderWireframeBB = _renderWireframeBB;

        // Applying clear color
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glDepthMask(GL_FALSE);
        RESOURCEMANAGER.GetShaderByName("skyboxShader")->use();

        glm::mat4 projection = glm::perspective(glm::radians(ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetViewMatrix();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetViewMatrix())); // remove translation from the view matrix
        RESOURCEMANAGER.GetShaderByName("skyboxShader")->setMat4("view", skyboxView);
        RESOURCEMANAGER.GetShaderByName("skyboxShader")->setMat4("projection", projection);

        skybox.draw();

        spotLightCurrentPosition = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->LerpPosition(spotLightCurrentPosition);
        spotLightCurrentDirection = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->LerpDirection(spotLightCurrentDirection);

        //ShadowMap Creation
        glm::vec3 shadowDir = glm::normalize(lightCenter - lightPos);
        dirDirection[0] = shadowDir.x;
        dirDirection[1] = shadowDir.y;
        dirDirection[2] = shadowDir.z;

        float near_plane = 0.2f, far_plane = 130.0f;
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

        RESOURCEMANAGER.GetShaderByName("modelShader")->use();
        RESOURCEMANAGER.GetShaderByName("modelShader")->setVec3("dirLight.direction", dirDirection);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setVec3("dirLight.color", dirColor);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setInt("dirLight.isActive", dirActive);

        RESOURCEMANAGER.GetShaderByName("modelShader")->setBool("spotLights[0].isActive", isSpotActive);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setVec3("spotLights[0].position", spotLightCurrentPosition);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setVec3("spotLights[0].direction", spotLightCurrentDirection);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setFloat("spotLights[0].constant", spotLightConstant);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setFloat("spotLights[0].linear", spotLightLinear);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setFloat("spotLights[0].quadratic", spotLightQuadratic);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setVec3("spotLights[0].color", spotLightColor);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(spotLightCutOff)));
        RESOURCEMANAGER.GetShaderByName("modelShader")->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(spotLightOuterCutOff)));

        RESOURCEMANAGER.GetShaderByName("modelShader")->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        RESOURCEMANAGER.GetShaderByName("modelShader")->setMat4("projection", projection);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setMat4("view", view);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setVec3("lightPos", lightPos);
        RESOURCEMANAGER.GetShaderByName("modelShader")->setMat4("lightSpaceMatrix", shadowMap.GetLightSpaceMatrix());

        RESOURCEMANAGER.GetShaderByName("outlineShader")->use();
        RESOURCEMANAGER.GetShaderByName("outlineShader")->setVec3("dirLight.direction", dirDirection);
        RESOURCEMANAGER.GetShaderByName("outlineShader")->setVec3("dirLight.color", dirColor);
        RESOURCEMANAGER.GetShaderByName("outlineShader")->setInt("dirLight.isActive", dirActive);

        RESOURCEMANAGER.GetShaderByName("outlineShader")->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        RESOURCEMANAGER.GetShaderByName("outlineShader")->setMat4("projection", projection);
        RESOURCEMANAGER.GetShaderByName("outlineShader")->setMat4("view", view);

        RESOURCEMANAGER.GetShaderByName("skyboxReflectionShader")->use();
        RESOURCEMANAGER.GetShaderByName("skyboxReflectionShader")->setInt("skybox", 0);
        RESOURCEMANAGER.GetShaderByName("skyboxReflectionShader")->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        RESOURCEMANAGER.GetShaderByName("skyboxReflectionShader")->setMat4("projection", projection);
        RESOURCEMANAGER.GetShaderByName("skyboxReflectionShader")->setMat4("view", view);

        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->use();
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setVec3("dirLight.direction", dirDirection);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setVec3("dirLight.color", dirColor);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setInt("dirLight.isActive", dirActive);

        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setBool("spotLights[0].isActive", isSpotActive);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setVec3("spotLights[0].position", spotLightCurrentPosition);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setVec3("spotLights[0].direction", spotLightCurrentDirection);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setFloat("spotLights[0].constant", spotLightConstant);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setFloat("spotLights[0].linear", spotLightLinear);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setFloat("spotLights[0].quadratic", spotLightQuadratic);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setVec3("spotLights[0].color", spotLightColor);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(spotLightCutOff)));
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(spotLightOuterCutOff)));

        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setMat4("projection", projection);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setMat4("view", view);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setVec3("lightPos", lightPos);
        RESOURCEMANAGER.GetShaderByName("instanceModelShader")->setMat4("lightSpaceMatrix", shadowMap.GetLightSpaceMatrix());

        RESOURCEMANAGER.GetShaderByName("lightObjectShader")->use();
        RESOURCEMANAGER.GetShaderByName("lightObjectShader")->setVec3("lightColor", dirColor);
        RESOURCEMANAGER.GetShaderByName("lightObjectShader")->setMat4("projection", projection);
        RESOURCEMANAGER.GetShaderByName("lightObjectShader")->setMat4("view", view);

        RESOURCEMANAGER.GetShaderByName("cloudShader")->use();
        RESOURCEMANAGER.GetShaderByName("cloudShader")->setVec3("dirLight.direction", dirDirection);
        RESOURCEMANAGER.GetShaderByName("cloudShader")->setVec3("dirLight.color", dirColor);
        RESOURCEMANAGER.GetShaderByName("cloudShader")->setInt("dirLight.isActive", dirActive);

        RESOURCEMANAGER.GetShaderByName("cloudShader")->setVec3("viewPos", ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
        RESOURCEMANAGER.GetShaderByName("cloudShader")->setVec3("initialCloudPosition", initialCloudPosition);
        RESOURCEMANAGER.GetShaderByName("cloudShader")->setFloat("cloudSpeed", cloudSpeed);
        RESOURCEMANAGER.GetShaderByName("cloudShader")->setFloat("time", TIME.GetTime());
        RESOURCEMANAGER.GetShaderByName("cloudShader")->setMat4("projection", projection);
        RESOURCEMANAGER.GetShaderByName("cloudShader")->setMat4("view", view);


        GAMEMANAGER.root->Render(Transform::Origin());

        //HUD.Update();
        PAGEMANAGER.Update();
        AUDIOENGINEMANAGER.Update();


        imguiMain->endDraw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        INPUT.UpdateOldStates();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    AUDIOENGINEMANAGER.Cleanup();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}