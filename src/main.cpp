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
#include "Managers/SceneManager.h"
#include "Managers/ResourceMapsManager.h"
#include "Managers/NodesManager.h"
#include "Gui/ImguiMain.h"

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

    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    vector<std::string> faces
            {
                    "res/skybox/right.jpg",
                    "res/skybox/left.jpg",
                    "res/skybox/top.jpg",
                    "res/skybox/bottom.jpg",
                    "res/skybox/front.jpg",
                    "res/skybox/back.jpg"
            };
    unsigned int cubemapTexture = loadCubemap(faces);

    skyboxShader->use();
    skyboxShader->setInt("skybox", 0);

    // Load models
    Model* sandModel = new Model("res/Sand/Sand.obj");

    // Nodes id
    int nextNodeId = 0;

    // Create graph



/*    std::shared_ptr<Node> root = std::make_shared<Node>();
    root->id = nextNodeId;
    root->name = "root";
    nextNodeId++;*/

    // maps
    ResourceMapsManager::GetInstance().RegisterModel("sandModel", sandModel);
    ResourceMapsManager::GetInstance().RegisterShader("instanceModelShader", instanceModelShader);

    std::shared_ptr<Node> root = SceneManager::LoadFromJsonFile("../../scenes/test.json");
/*    nlohmann::json jsonData2 = SceneManager::SerializeRoot(root, nextNodeId);
    SceneManager::SaveToJsonFile(jsonData2, "../../scenes/test2.json");*/

/*    std::shared_ptr<Node> sand = std::make_shared<Node>();
    sand->id = nextNodeId;
    sand->name = "sand";
    nextNodeId++;*/

/*    auto sandRenderer = ComponentsManager::getInstance().createComponent<InstanceRenderer>(sandModel, 1000000, instanceModelShader);
    sand->AddComponent(sandRenderer);*/

/*    std::shared_ptr<Node> blockManager = std::make_shared<Node>();
    blockManager->id = nextNodeId;
    blockManager->name = "blockManager";
    nextNodeId++;*/

/*    auto blockManagerComp = ComponentsManager::getInstance().createComponent<BlockManager>(100, 100, 100);
    blockManagerComp->SetInstanceRenderer(sandRenderer);
    blockManager->AddComponent(blockManagerComp);*/

    //TAK NIE MOZE BYC DO ZMIANY NIZEJ ANALOGICZNIE
    ComponentsManager::getInstance().getComponentByID<BlockManager>(0)->SetInstanceRenderer(ComponentsManager::getInstance().getComponentByID<InstanceRenderer>(0));

/*    std::shared_ptr<Node> player = std::make_shared<Node>();
    player->id = nextNodeId;
    player->name = "player";
    nextNodeId++;*/

/*    auto camera = ComponentsManager::getInstance().createComponent<Camera>(glm::vec3(0.0f,1.8f,0.0f));*/

/*    player->AddComponent(camera);
    auto playerController = ComponentsManager::getInstance().createComponent<PlayerController>();*/



/*    playerController->SetCamera(camera);
    playerController->SetBlockManager(blockManagerComp);
    player->AddComponent(playerController);
    player->GetTransform()->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));*/

    ComponentsManager::getInstance().getComponentByID<PlayerController>(0)->SetCamera(ComponentsManager::getInstance().getComponentByID<Camera>(0));
    ComponentsManager::getInstance().getComponentByID<PlayerController>(0)->SetBlockManager(ComponentsManager::getInstance().getComponentByID<BlockManager>(0));
    NodesManager::getInstance().getNodeByName("player")->GetTransform()->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));

    // root
/*    root->AddChild(sand);
    root->AddChild(blockManager);
    root->AddChild(player);*/

    // json
    nlohmann::json jsonData = SceneManager::SerializeRoot(root, nextNodeId);
    //std::cout << jsonData;
    SceneManager::SaveToJsonFile(jsonData, "../../scenes/test2.json");

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

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    float dirColor[3] = { 0.999f, 0.999f, 1.00f };
    float dirDirection[3] = { -0.5f, -0.5f, -0.5f };
    bool dirActive = true;

    std::shared_ptr<ImguiMain> imguiMain = std::make_shared<ImguiMain>(window, glsl_version);
    imguiMain->SetRoot(root);

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
        imguiMain->draw(nextNodeId);

        // Applying clear color
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDepthMask(GL_FALSE);
        skyboxShader->use();

        glm::mat4 projection = glm::perspective(glm::radians(ComponentsManager::getInstance().getComponentByID<Camera>(0)->GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = ComponentsManager::getInstance().getComponentByID<Camera>(0)->GetViewMatrix();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(ComponentsManager::getInstance().getComponentByID<Camera>(0)->GetViewMatrix())); // remove translation from the view matrix
        skyboxShader->setMat4("view", skyboxView);
        skyboxShader->setMat4("projection", projection);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);

        modelShader->use();
        modelShader->setVec3("dirLight.direction", dirDirection);
        modelShader->setVec3("dirLight.color", dirColor);
        modelShader->setInt("dirLight.isActive", dirActive);

        modelShader->setVec3("viewPos", ComponentsManager::getInstance().getComponentByID<Camera>(0)->GetPosition());
        modelShader->setMat4("projection", projection);
        modelShader->setMat4("view", view);

        instanceModelShader->use();
        instanceModelShader->setVec3("dirLight.direction", dirDirection);
        instanceModelShader->setVec3("dirLight.color", dirColor);
        instanceModelShader->setInt("dirLight.isActive", dirActive);

        instanceModelShader->setVec3("viewPos", ComponentsManager::getInstance().getComponentByID<Camera>(0)->GetPosition());
        instanceModelShader->setMat4("projection", projection);
        instanceModelShader->setMat4("view", view);

        lightObjectShader->use();
        lightObjectShader->setVec3("lightColor", dirColor);
        lightObjectShader->setMat4("projection", projection);
        lightObjectShader->setMat4("view", view);

        root->Render(Transform::Origin());

        imguiMain->endDraw();

        INPUT.UpdateOldStates();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ma_engine_uninit(&engine);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    imguiMain->destroy();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
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