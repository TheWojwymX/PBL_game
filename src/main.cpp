#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "texture.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

#define MINIAUDIO_IMPLEMENTATION
#include <../thirdparty//miniaudio.h>

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


static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
unsigned int loadCubemap(vector<std::string> faces);
Texture2D loadTextureFromFile(const char* file, bool alpha);

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
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);

    // Build and compile the shader program
    Shader* modelShader = new Shader("res/vecShader.vs", "res/fragShader.fs");
    Shader* lightObjectShader = new Shader("res/vecLightObjectShader.vs", "res/fragLightObjectShader.fs");
    Shader* instanceModelShader = new Shader("res/instanceVecShader.vs", "res/instanceFragShader.fs");
    Shader* skyboxShader = new Shader("res/skyboxVecShader.vs", "res/skyboxFragShader.fs");

    Skybox skybox;
    skybox.init();

    skyboxShader->use();
    skyboxShader->setInt("skybox", 0);

    // Load models
    Model* sandModel = new Model("res/Sand/Sand.obj");

    // Create components manager
    auto componentsManager = ComponentsManager();

    // Create graph
    std::shared_ptr<Node> root = std::make_shared<Node>();

    std::shared_ptr<Node> sand = std::make_shared<Node>();
    //std::shared_ptr<InstanceRenderer> sandRenderer = std::make_shared<InstanceRenderer>(sandModel, 1000000, instanceModelShader);
    auto sandRenderer = componentsManager.createComponent<InstanceRenderer>(sandModel, 1000000, instanceModelShader);

    sand->AddComponent(sandRenderer);

    std::shared_ptr<Node> blockManager = std::make_shared<Node>();

    //std::shared_ptr<BlockManager> blockManagerComp = std::make_shared<BlockManager>(100,100,100);
    auto blockManagerComp = componentsManager.createComponent<BlockManager>(100, 100, 100);
    //auto blockManagerComp2 = componentsManager.createComponent<BlockManager>(666, 100, 100);
    blockManagerComp->SetInstanceRenderer(sandRenderer);
    blockManager->AddComponent(blockManagerComp);

    std::shared_ptr<Node> player = std::make_shared<Node>();
    //std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f,1.8f,0.0f));
    auto camera = componentsManager.createComponent<Camera>(glm::vec3(0.0f,1.8f,0.0f));

    player->AddComponent(camera);
    //std::shared_ptr<PlayerController> playerController = std::make_shared<PlayerController>();
    auto playerController = componentsManager.createComponent<PlayerController>();

    playerController->SetCamera(camera);
    playerController->SetBlockManager(blockManagerComp);
    player->AddComponent(playerController);
    player->GetTransform()->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));

    // root
    root->AddChild(sand);
    root->AddChild(blockManager);
    root->AddChild(player);

    //std::cout << componentsManager.getComponentByID<BlockManager>(1)->id << componentsManager.getComponentByID<BlockManager>(0)->id;

    nlohmann::json data = camera->Serialize();
    std::cout << data;


    // Init
    INPUT.Init(window, SCR_WIDTH, SCR_HEIGHT);
    root->Init();

    // Setup miniaudio
    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine.");
        return -1;
    }

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    float dirColor[3] = { 0.999f, 0.999f, 1.00f };
    float dirDirection[3] = { -0.5f, -0.5f, -0.5f };
    bool dirActive = true;


    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltaTime
        TIME.Update();

        // Input
        root->Input();

        // Update
        root->Update();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Tool window");

        ImGui::ColorEdit3("Dir Color", dirColor);
        ImGui::SliderFloat3("Dir direction", dirDirection, -1.0f, 1.0f);
        ImGui::Checkbox("Dir Active", &dirActive);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        // Applying clear color
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);
        skyboxShader->use();

        glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera->GetViewMatrix();

        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->GetViewMatrix())); // remove translation from the view matrix
        skyboxShader->setMat4("view", skyboxView);
        skyboxShader->setMat4("projection", projection);

        skybox.draw();
        glDepthMask(GL_TRUE);

        modelShader->use();
        modelShader->setVec3("dirLight.direction", dirDirection);
        modelShader->setVec3("dirLight.color", dirColor);
        modelShader->setInt("dirLight.isActive", dirActive);

        modelShader->setVec3("viewPos", camera->GetPosition());
        modelShader->setMat4("projection", projection);
        modelShader->setMat4("view", view);

        instanceModelShader->use();
        instanceModelShader->setVec3("dirLight.direction", dirDirection);
        instanceModelShader->setVec3("dirLight.color", dirColor);
        instanceModelShader->setInt("dirLight.isActive", dirActive);

        instanceModelShader->setVec3("viewPos", camera->GetPosition());
        instanceModelShader->setMat4("projection", projection);
        instanceModelShader->setMat4("view", view);

        lightObjectShader->use();
        lightObjectShader->setVec3("lightColor", dirColor);
        lightObjectShader->setMat4("projection", projection);
        lightObjectShader->setMat4("view", view);

        root->Render(Transform::Origin());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        INPUT.UpdateOldStates();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ma_engine_uninit(&engine);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

Texture2D loadTextureFromFile(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}