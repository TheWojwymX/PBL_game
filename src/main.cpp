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
#include "Component/TopLayerSnap.h"


#include "Balance/Balancer.h"
#include "Managers/ComponentsManager.h"
#include "Managers/SceneManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/NodesManager.h"
#include "Managers/LightsManager.h"
#include "Managers/WeatherManager.h"
#include "Managers/TransparentRenderer.h"
#include "Gui/ImguiMain.h"
#include "ParticleGenerator.h"
#include "WindSimulation.h"
#include "Framebuffer.h"
#include "BatchRandomGenerator.h"

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

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    bool _renderWireframeBB = false;

    ComponentsManager::getInstance().GetComponentByID<Camera>(2)->setScreenWidth(GAMEMANAGER._screenWidth);
    ComponentsManager::getInstance().GetComponentByID<Camera>(2)->setScreenHeight(GAMEMANAGER._screenHeight);

    std::shared_ptr<ImguiMain> imguiMain = std::make_shared<ImguiMain>(GAMEMANAGER._window, glsl_version);
    imguiMain->SetRoot(GAMEMANAGER.root);
    imguiMain->SetSelectedObject(GAMEMANAGER.root);

    /*    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), "testowy");
    COMPONENTSMANAGER.CreateComponent<Camera>();
    NODESMANAGER.getNodeByName("testowy")->AddComponent(COMPONENTSMANAGER.GetComponentByID<Camera>(4));*/
    // json save for testing
    nlohmann::json jsonData = SCENEMANAGER.SerializeRoot(GAMEMANAGER.root);
    //std::cout << jsonData;
    SCENEMANAGER.SaveToJsonFile(jsonData, "scenes/test2.json");

    HUD.Init();

    ENEMIESMANAGER.Init();

    //Prepare shader pointers
    auto skyboxShader = RESOURCEMANAGER.GetShaderByName("skyboxShader");
    auto modelShader = RESOURCEMANAGER.GetShaderByName("modelShader");
    auto outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    auto skyboxReflectionShader = RESOURCEMANAGER.GetShaderByName("skyboxReflectionShader");
    auto instancedSandShader = RESOURCEMANAGER.GetShaderByName("instancedSandShader");
    auto instancedMetalShader = RESOURCEMANAGER.GetShaderByName("instancedMetalShader");
    auto instancedPlasticShader = RESOURCEMANAGER.GetShaderByName("instancedPlasticShader");
    auto lightObjectShader = RESOURCEMANAGER.GetShaderByName("lightObjectShader");
    auto cloudShader = RESOURCEMANAGER.GetShaderByName("cloudShader");
    auto particleShader = RESOURCEMANAGER.GetShaderByName("particleShader");
    auto rangeShader = RESOURCEMANAGER.GetShaderByName("turretRangeShader");
    auto shovelShader = RESOURCEMANAGER.GetShaderByName("shovelShader");
    auto blueprintShader = RESOURCEMANAGER.GetShaderByName("blueprintShader");
    auto glowstickShader = RESOURCEMANAGER.GetShaderByName("glowstickShader");

    PAGEMANAGER.Init();

    TURRETSMANAGER.Init();

    TUTORIALMANAGER.Init();

    LIGHTSMANAGER.InitLights();

    WEATHERMANAGER.Init();

    auto shovelController = NODESMANAGER.getNodeByName("Shovel")->GetComponent<ShovelController>();
    auto shovelRenderer = NODESMANAGER.getNodeByName("Shovel")->GetComponent<ShovelRenderer>();

    auto uPDAController = NODESMANAGER.getNodeByName("PDA")->GetComponent<PDAController>();
    auto uPDARenderer = NODESMANAGER.getNodeByName("PDA")->GetComponent<ShovelRenderer>();
    auto uPDAAntennaRenderer = NODESMANAGER.getNodeByName("PDAAntenna")->GetComponent<ShovelRenderer>();

    auto compassController = NODESMANAGER.getNodeByName("CompassNode")->GetComponent<CompassController>();
    auto compassRenderer = NODESMANAGER.getNodeByName("CompassNode")->GetComponent<ShovelRenderer>();
    auto compassUpRenderer = NODESMANAGER.getNodeByName("CompassUp")->GetComponent<ShovelRenderer>();
    auto compassDownRenderer = NODESMANAGER.getNodeByName("CompassDown")->GetComponent<ShovelRenderer>();

    GAMEMANAGER.PlayMenuMusic();

    //bool antialiasing = true;
    // Main loop
    while (!glfwWindowShouldClose(GAMEMANAGER._window))
    {
/*        if (INPUT.IsKeyPressed(GLFW_KEY_9)) {
            if(antialiasing){
                glDisable(GL_MULTISAMPLE);
                antialiasing = false;
                std::cout << "wylaczono antialiasing" << std::endl;
            }else{
                glEnable(GL_MULTISAMPLE);
                antialiasing = true;
                std::cout << "wlaczono antialiasing" << std::endl;
            }

        }*/

        // Calculate deltaTime
        TIME.Update();

        if(INPUT.IsKeyPressed(GLFW_KEY_B)){
            BALANCER.resetToDefault();
        }

        if(!GAMEMANAGER._isInMainMenu) {

            if(INPUT.IsKeyPressed(GLFW_KEY_0)){
                GAMEMANAGER.GoToMainMenu();
            }


            //debugging adding money
            if (INPUT.IsKeyPressed(77)) {
                GAMEMANAGER.AddPlastic(10);
                //std::cout << "Aktualny stan portfela " << GAMEMANAGER._money << std::endl;
            }

            if (INPUT.IsKeyPressed(GLFW_KEY_RIGHT_BRACKET) && !TUTORIALMANAGER._isTutorialEnded) {
                TUTORIALMANAGER.SkipTutorial();
            }

            if (!GAMEMANAGER._paused) {
                ENEMIESMANAGER.Update();
                TURRETSMANAGER.Update();
                UPGRADEMANAGER.Update();
                DOMEMANAGER.Update();
                GAMEMANAGER.Update();
                WEATHERMANAGER.Update();
            }
            TUTORIALMANAGER.Update();
            LIGHTSMANAGER.Update();


            // Input
            GAMEMANAGER.root->Input();

            // Update
            GAMEMANAGER.root->Update();

/*        if(TIME.GetTime() > 10 && TIME.GetTime() < 11){
            std::cout << "minal czas";
            NODESMANAGER.getNodeByName("AntModel")->GetTransform()->SetPosition(glm::vec3(0.0f, 115.0f, 0.0f));
        }*/

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

            float near_plane = 0.2f, far_plane = 550.0f;
            glm::mat4 lightProjection = glm::ortho(-200.0f, 200.0f, -300.0f, 200.0f, near_plane, far_plane);
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

            glowstickShader->use();
            glowstickShader->setVec3("viewPos", viewPos);
            glowstickShader->setMat4("projection", projection);
            glowstickShader->setMat4("view", view);
            glowstickShader->setMat4("lightSpaceMatrix", SHADOWMAP.GetLightSpaceMatrix());

            outlineShader->use();
            outlineShader->setVec3("viewPos", viewPos);
            outlineShader->setMat4("projection", projection);
            outlineShader->setMat4("view", view);

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

#pragma region InstanceMetalShader setup
            instancedMetalShader->use();
            instancedMetalShader->setVec3("viewPos", viewPos);
            instancedMetalShader->setMat4("projection", projection);
            instancedMetalShader->setMat4("view", view);
            instancedMetalShader->setMat4("lightSpaceMatrix", SHADOWMAP.GetLightSpaceMatrix());
#pragma endregion

#pragma region InstancePlasticShader setup
            instancedPlasticShader->use();
            instancedPlasticShader->setVec3("viewPos", viewPos);
            instancedPlasticShader->setMat4("projection", projection);
            instancedPlasticShader->setMat4("view", view);
            instancedPlasticShader->setMat4("lightSpaceMatrix", SHADOWMAP.GetLightSpaceMatrix());
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

            rangeShader->use();
            rangeShader->setMat4("projection", projection);
            rangeShader->setMat4("view", view);

            ///
            blueprintShader->use();

            blueprintShader->setVec3("viewPos", viewPos);
            blueprintShader->setMat4("projection", projection);
            blueprintShader->setMat4("view", view);
            blueprintShader->setVec3("additionalColor", TURRETSMANAGER._additionalColor);

            GAMEMANAGER.root->Render(Transform::Origin());
            TRANSPARENT_RENDERER.Render();
//            FRAMEBUFFER.BeginRender();
//            FRAMEBUFFER.EndRender();

            // RealUpdate is after standard Update and has to be here if you want smooth rotation!
            // This order (shovelRender -> shovelController -> shovelShader) is working and the place
            // (at the end of loop) is also working, so it is recommended to not move it
            // moving may cause some problems like lack of smoothness or models bugging
            glClear(GL_DEPTH_BUFFER_BIT); // Clear depth because you want items always "closest to screen"
            shovelRenderer->RenderShovel(Transform::Origin());
            shovelController->RealUpdate();

            uPDAAntennaRenderer->RenderShovel(Transform::Origin());
            uPDARenderer->RenderShovel(Transform::Origin());
            uPDAController->RealUpdate();

            compassDownRenderer->RenderShovel(Transform::Origin());
            compassUpRenderer->RenderShovel(Transform::Origin());
            compassRenderer->RenderShovel(Transform::Origin());
            compassController->RealUpdate();

            shovelShader->use();
            shovelShader->setVec3("viewPos", viewPos);
            shovelShader->setMat4("projection", projection);
            shovelShader->setMat4("view", view);
            shovelShader->setMat4("lightSpaceMatrix", SHADOWMAP.GetLightSpaceMatrix());
        }
        PAGEMANAGER.Update();
        HUD.Update();
        AUDIOMANAGER.Update();


        // Start the Dear ImGui frame
        imguiMain->draw();

//        ImGui::Begin("Depth Map");
//        ImGui::SetWindowSize(ImVec2(300, 300), ImGuiCond_Once);
//        ImGui::Image((void *) (intptr_t) SHADOWMAP.GetDepthMapTexture(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
//        ImGui::End();
//
        // Quick Debug
        ImGui::SliderFloat("Shot Const", &LIGHTSMANAGER.shotConstant, -10.0f, 10.0f);
        ImGui::SliderFloat("Shot Linear", &LIGHTSMANAGER.shotLinear, -10.0f, 10.0f);
        ImGui::SliderFloat("Shot Quadratic", &LIGHTSMANAGER.shotQuadratic, -10.0f, 10.0f);

        ImGui::InputFloat3("Light Position", &LIGHTSMANAGER.lightPos[0]);  // Change lightPos
        ImGui::InputFloat3("Center", &LIGHTSMANAGER.lightCenter[0]);

        ImGui::SliderFloat("glowstickConstant", &LIGHTSMANAGER.glowstickConstant, -5.0f, 5.0f);
        ImGui::SliderFloat("glowstickLinear", &LIGHTSMANAGER.glowstickLinear, -5.0f, 5.0f);
        ImGui::SliderFloat("glowstickQuadratic", &LIGHTSMANAGER.glowstickQuadratic, -5.0f, 5.0f);

        ImGui::ColorEdit3("Directional Light Color", glm::value_ptr(LIGHTSMANAGER.dirColor));
        ImGui::ColorEdit3("Sky Color", glm::value_ptr(LIGHTSMANAGER.skyColor));

        ImGui::Checkbox("Wireframe Frustum Boxes", &_renderWireframeBB);

        ImGui::SliderFloat3("Wind Direction", &WEATHERMANAGER.getWindDirection()[0], -1.0f, 1.0f);

        BALANCER.addEnemyStatsToImgui();
        BALANCER.addTurretStatsToImgui();
        BALANCER.addPhaseStatsToImgui();
        BALANCER.addUpgradesStatsToImgui();

        imguiMain->endDraw();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        INPUT.UpdateOldStates();
        glfwSwapBuffers(GAMEMANAGER._window);
        glfwPollEvents();
    }
    // Cleanup
    AUDIOMANAGER.Cleanup();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(GAMEMANAGER._window);
    glfwTerminate();


    return 0;
}