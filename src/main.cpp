/*

                                                              ,----..
  .--.--.                                          ,---,.    /   /   \  ,--,     ,--,
 /  /    '.                               ,---,  ,'  .'  \  /   .     : |'. \   / .`|
|  :  /`. /                   ,---,     ,---.'|,---.' .' | .   /   ;.  \; \ `\ /' / ;
;  |  |--`                ,-+-. /  |    |   | :|   |  |: |.   ;   /  ` ;`. \  /  / .'
|  :  ;_      ,--.--.    ,--.'|'   |    |   | |:   :  :  /;   |  ; \ ; | \  \/  / ./
 \  \    `.  /       \  |   |  ,"' |  ,--.__| |:   |    ; |   :  | ; | '  \  \.'  /
  `----.   \.--.  .-. | |   | /  | | /   ,'   ||   :     \.   |  ' ' ' :   \  ;  ;
  __ \  \  | \__\/: . . |   | |  | |.   '  /  ||   |   . |'   ;  \; /  |  / \  \  \
 /  /`--'  / ," .--.; | |   | |  |/ '   ; |:  |'   :  '; | \   \  ',  /  ;  /\  \  \
'--'.     / /  /  ,.  | |   | |--'  |   | '/  '|   |  | ;   ;   :    / ./__;  \  ;  \
  `--'---' ;  :   .'   \|   |/      |   :    :||   :   /     \   \ .'  |   : / \  \  ;
           |  ,     .-./'---'        \   \  /  |   | ,'       `---`    ;   |/   \  ' |
            `--`---'                  `----'   `----'                  `---'     `--`
*/

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
#include "Component/TopLayerSnap.h"


#include "Managers/ComponentsManager.h"
#include "Managers/SceneManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/NodesManager.h"
#include "Managers/LightsManager.h"
#include "Managers/WeatherManager.h"
#include "Managers/TransparentRenderer.h"
#include "ParticleGenerator.h"
#include "WindSimulation.h"
#include "Framebuffer.h"
#include "BatchRandomGenerator.h"

#include "Enemies/EnemiesManager.h"

#include "HUD/HUDMain.h"

#include <iostream>


#include <glad/glad.h>


#include "Managers/ComponentsManager.h"
#include "Managers/GameManager.h"
#include "Managers/AudioManager.h"
#include "HUD/PageManager.h"
#include "Managers/DomeManager.h"
#include "Managers/UpgradeManager.h"
#include "Turrets/TurretsManager.h"
#include "Managers/TutorialManager.h"
#include "Component/ShovelController.h"
#include "Component/PDAController.h"
#include "HUD/Pages/MainMenuPage/MainMenuPage.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

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
    const char* glsl_version = "#version 440";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    glfwWindowHint(GLFW_SAMPLES, 2);
    // Create window with graphics context
    GAMEMANAGER.Init();
    if (GAMEMANAGER._window == NULL)
        return 1;
    glfwMakeContextCurrent(GAMEMANAGER._window);
    glfwSwapInterval(1); // Enable VSync

    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
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


    AUDIOMANAGER.Init();

    // Deserialization of resources and nodes
    GAMEMANAGER.root = SCENEMANAGER.LoadFromJsonFile("scenes/test1.json");

    RESOURCEMANAGER.GetShaderByName("skyboxShader")->use();
    RESOURCEMANAGER.GetShaderByName("skyboxShader")->setInt("skybox", 0);

    //NodesManager::getInstance().getNodeByName("player")->GetTransform()->SetPosition(glm::vec3(0.0f, 115.0f, 0.0f));

    Skybox skybox;
    skybox.init();

    // Init
    INPUT.Init(GAMEMANAGER._window, GAMEMANAGER._screenWidth, GAMEMANAGER._screenHeight);
    GAMEMANAGER.root->Init();

    SHADOWMAP.Init();
    SHADOWMAP.AssignShadowMapToShader();

    //FRAMEBUFFER.Init();

    glm::vec3 initialCloudPosition(0.0f, 0.0f, 0.0f);
    float cloudSpeed = 5.0f;

    glm::vec4 clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.00f);

    bool _renderWireframeBB = false;

    ComponentsManager::getInstance().GetComponentByID<Camera>(2)->setScreenWidth(GAMEMANAGER._screenWidth);
    ComponentsManager::getInstance().GetComponentByID<Camera>(2)->setScreenHeight(GAMEMANAGER._screenHeight);
    /*    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), "testowy");
    COMPONENTSMANAGER.CreateComponent<Camera>();
    NODESMANAGER.getNodeByName("testowy")->AddComponent(COMPONENTSMANAGER.GetComponentByID<Camera>(4));*/
    // json save for testing
    nlohmann::json jsonData = SCENEMANAGER.SerializeRoot(GAMEMANAGER.root);
    //std::cout << jsonData;
    SCENEMANAGER.SaveToJsonFile(jsonData, "scenes/test2.json");

    ENEMIESMANAGER.Init();

    //Prepare shader pointers
    auto skyboxShader = RESOURCEMANAGER.GetShaderByName("skyboxShader");
    auto modelShader = RESOURCEMANAGER.GetShaderByName("modelShader");
    auto skyboxReflectionShader = RESOURCEMANAGER.GetShaderByName("skyboxReflectionShader");
    auto instancedSandShader = RESOURCEMANAGER.GetShaderByName("instancedSandShader");
    auto lightObjectShader = RESOURCEMANAGER.GetShaderByName("lightObjectShader");
    auto cloudShader = RESOURCEMANAGER.GetShaderByName("cloudShader");
    auto particleShader = RESOURCEMANAGER.GetShaderByName("particleShader");

    PAGEMANAGER.Init();

    TUTORIALMANAGER.Init();

    LIGHTSMANAGER.InitLights();

    WEATHERMANAGER.Init();

    GAMEMANAGER.PlayMenuMusic();

    //bool antialiasing = true;
    // Main loop
    while (!glfwWindowShouldClose(GAMEMANAGER._window))
    {
        // Calculate deltaTime
        TIME.Update();

        if(!GAMEMANAGER._isInMainMenu) {
            if (!GAMEMANAGER._paused) {
                WEATHERMANAGER.Update();
            }
            LIGHTSMANAGER.Update();

            // Input
            GAMEMANAGER.root->Input();

            // Update
            GAMEMANAGER.root->Update();


            FrustumCulling::_renderWireframeBB = _renderWireframeBB;

            // Applying clear color
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glDepthMask(GL_FALSE);

            RESOURCEMANAGER.GetShaderByName("skyboxShader")->use();

            glm::vec3 viewPos = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition();
            glm::mat4 projection = glm::perspective(glm::radians(ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetZoom()),
                                                    (float) GAMEMANAGER._screenWidth / (float) GAMEMANAGER._screenHeight, 0.1f, 1000.0f);
            glm::mat4 view = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetViewMatrix();
            glm::mat4 skyboxView = glm::mat4(glm::mat3(
                    ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetViewMatrix())); // remove translation from the view matrix
            skyboxShader->setMat4("view", skyboxView);
            skyboxShader->setMat4("projection", projection);

            skyboxShader->setVec3("lightPos", LIGHTSMANAGER.lightPosShader);
            skyboxShader->setVec3("viewPos", viewPos);
            skyboxShader->setVec3("lightColor", LIGHTSMANAGER.skyColor);

            skybox.draw();

            //ShadowMap Creation
            glm::vec3 shadowDir = glm::normalize(LIGHTSMANAGER.lightCenter - LIGHTSMANAGER.lightPos);
            LIGHTSMANAGER.dirDirection[0] = shadowDir.x;
            LIGHTSMANAGER.dirDirection[1] = shadowDir.y;
            LIGHTSMANAGER.dirDirection[2] = shadowDir.z;

            float near_plane = 0.2f, far_plane = 300.0f;
            glm::mat4 lightProjection = glm::ortho(-200.0f, 200.0f, -50.0f, 100.0f, near_plane, far_plane);
            glm::mat4 lightView = glm::lookAt(LIGHTSMANAGER.shadowLightPos, LIGHTSMANAGER.shadowLightCenter, glm::vec3(0.0, 1.0, 0.0));
            SHADOWMAP.SetLightProjection(lightProjection);
            SHADOWMAP.SetLightView(lightView);
            SHADOWMAP.AssignLightSpaceMatrixToShader();

            glDepthMask(GL_TRUE);

            SHADOWMAP.BeginRender();
            SHADOWMAP.RenderMap();
            SHADOWMAP.EndRender();

            glViewport(0, 0, GAMEMANAGER._screenWidth, GAMEMANAGER._screenHeight); //Reset Viewport After Rendering to Shadow Map

            modelShader->use();
            modelShader->setVec3("viewPos", viewPos);
            modelShader->setMat4("projection", projection);
            modelShader->setMat4("view", view);
            modelShader->setMat4("lightSpaceMatrix", SHADOWMAP.GetLightSpaceMatrix());

            skyboxReflectionShader->use();
            skyboxReflectionShader->setInt("skybox", 0);
            skyboxReflectionShader->setVec3("viewPos", viewPos);
            skyboxReflectionShader->setMat4("projection", projection);
            skyboxReflectionShader->setMat4("view", view);

#pragma region InstancedSandShader setup
            instancedSandShader->use();
            instancedSandShader->setVec3("viewPos", viewPos);
            instancedSandShader->setMat4("projection", projection);
            instancedSandShader->setMat4("view", view);
            instancedSandShader->setMat4("lightSpaceMatrix", SHADOWMAP.GetLightSpaceMatrix());
#pragma endregion


            lightObjectShader->use();
            lightObjectShader->setMat4("projection", projection);
            lightObjectShader->setMat4("view", view);

            cloudShader->use();
            cloudShader->setVec3("viewPos", viewPos);
            cloudShader->setVec3("initialCloudPosition", initialCloudPosition);
            cloudShader->setFloat("cloudSpeed", cloudSpeed);
            cloudShader->setFloat("time", TIME.GetTime());
            cloudShader->setMat4("projection", projection);
            cloudShader->setMat4("view", view);

            particleShader->use();
            particleShader->setMat4("projection", projection);
            particleShader->setMat4("view", view);


            GAMEMANAGER.root->Render(Transform::Origin());
            TRANSPARENT_RENDERER.Render();
//            FRAMEBUFFER.BeginRender();
//            FRAMEBUFFER.EndRender();

            // RealUpdate is after standard Update and has to be here if you want smooth rotation!
            // This order (shovelRender -> shovelController -> shovelShader) is working and the place
            // (at the end of loop) is also working, so it is recommended to not move it
            // moving may cause some problems like lack of smoothness or models bugging
            glClear(GL_DEPTH_BUFFER_BIT); // Clear depth because you want items always "closest to screen"
        }
        PAGEMANAGER.Update();
        AUDIOMANAGER.Update();

        INPUT.UpdateOldStates();
        glfwSwapBuffers(GAMEMANAGER._window);
        glfwPollEvents();
    }
    // Cleanup
    AUDIOMANAGER.Cleanup();

    glfwDestroyWindow(GAMEMANAGER._window);
    glfwTerminate();


    return 0;
}