#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Model* model, Shader* shader) : _model(model), _shader(shader) {}

MeshRenderer::MeshRenderer(){
    _type = ComponentType::MESHRENDERER;
}

void MeshRenderer::Render(glm::mat4 parentWorld) {
    glm::mat4 world = _ownerTransform->Combine(parentWorld);
    glm::mat4 viewProjectionMatrix = _cameraRef->GetProjectionMatrix(1280,720) * _cameraRef->GetViewMatrix();
    if(IsInFrustum(viewProjectionMatrix)) {
        RenderModel(_model, world);
        //framesRendered++;
        //cout << "Rendering: " << framesRendered << endl;  //Frustum Culling working
    }
}

void MeshRenderer::RenderShadows(glm::mat4 parentWorld) {

    shared_ptr<Shader> currentShader = _shader;
    SetShader(RESOURCEMANAGER.GetShaderByName("shadowShader"));
    glm::mat4 world = _ownerTransform->Combine(parentWorld);
    glm::mat4 viewProjectionMatrix = _cameraRef->GetProjectionMatrix(1280,720) * _cameraRef->GetViewMatrix();
    RenderModel(_model, world);
    SetShader(currentShader);
}

void MeshRenderer::RenderModel(shared_ptr<Model> model, glm::mat4 ctm) {
    if(_shouldRenderOutline) {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
        glStencilMask(0xFF); // enable writing to the stencil buffer
    }

    _shader->use();
    _shader->setMat4("model", ctm);
    model->Draw(*_shader);

    if(_shouldRenderOutline) {
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00); // disable writing to the stencil buffer

        _outlineShader->use();
        _outlineShader->setMat4("model", ctm);
        model->Draw(*_outlineShader);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
    }
}

void MeshRenderer::addToInspector(ImguiMain *imguiMain)
{
    if (ImGui::TreeNode("Mesh Renderer"))
    {
        // Block Gui related things go there
        ImGui::Text("Test1:");
        ImGui::Text("Test2:");

        ImGui::TreePop();

    }
}

nlohmann::json MeshRenderer::Serialize() {
    nlohmann::json data = Component::Serialize();

    data["model"] = _model->_name;
    data["shader"] = _shader->_name;
    data["outlineShader"] = _outlineShader->_name;

    return data;
}

void MeshRenderer::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("model")) {
        _model = RESOURCEMANAGER.GetModelByName(jsonData["model"].get<string>());
    }

    if (jsonData.contains("shader")) {
        _shader = RESOURCEMANAGER.GetShaderByName(jsonData["shader"].get<string>());
    }

    if (jsonData.contains("shader")) {
        _outlineShader = RESOURCEMANAGER.GetShaderByName(jsonData["outlineShader"].get<string>());
    }

    Component::Deserialize(jsonData);
}

void MeshRenderer::initiate() {
    Component::initiate();
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(2);
    i = 0;
}

void MeshRenderer::Update() {

    _ownerTransform->SetPosition(_ownerTransform->GetPosition());
    _ownerTransform->SetScale(_ownerTransform->GetScale());
    _ownerTransform->SetRotation(_ownerTransform->GetRotation());

    Component::Update();
}


bool MeshRenderer::IsInFrustum(const glm::mat4& viewProjectionMatrix)
{
    glm::vec3 min = GetWorldMinBoundingBox(_model, _ownerTransform);
    glm::vec3 max = GetWorldMaxBoundingBox(_model, _ownerTransform);

    if(_renderWireframeBB) {RenderBoundingBox(viewProjectionMatrix, min, max);} //Wireframe bounding boxes

    // Extract the frustum planes from the view-projection matrix
    auto frustumPlanes = extractFrustumPlanes(viewProjectionMatrix);

    // Check if the bounding box intersects the frustum
    return isBoxInFrustum(frustumPlanes, min, max);
}

void MeshRenderer::RenderBoundingBox(const glm::mat4& viewProjectionMatrix, glm::vec3 min, glm::vec3 max) {
    glm::vec4 corners[8];

    corners[0] = glm::vec4(min.x, min.y, min.z, 1.0f);
    corners[1] = glm::vec4(max.x, min.y, min.z, 1.0f);
    corners[2] = glm::vec4(min.x, max.y, min.z, 1.0f);
    corners[3] = glm::vec4(max.x, max.y, min.z, 1.0f);
    corners[4] = glm::vec4(min.x, min.y, max.z, 1.0f);
    corners[5] = glm::vec4(max.x, min.y, max.z, 1.0f);
    corners[6] = glm::vec4(min.x, max.y, max.z, 1.0f);
    corners[7] = glm::vec4(max.x, max.y, max.z, 1.0f);

    // Indices for lines connecting the corners to form the bounding box
    std::vector<int> indices = {
            0, 1, 1, 3, 3, 2, 2, 0,  // bottom rectangle
            4, 5, 5, 7, 7, 6, 6, 4,  // top rectangle
            0, 4, 1, 5, 2, 6, 3, 7   // vertical lines
    };

    std::vector<glm::vec3> vertices;
    for (auto index : indices) {
        vertices.push_back(corners[index]);
    }

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // Set up your wireframe shader and render the lines
    RESOURCEMANAGER.GetShaderByName("wireframeShader")->use();
    RESOURCEMANAGER.GetShaderByName("wireframeShader")->setMat4("viewProjection", viewProjectionMatrix);

    glDrawArrays(GL_LINES, 0, vertices.size());

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

std::vector<FrustumPlane> MeshRenderer::extractFrustumPlanes(const glm::mat4& viewProjectionMatrix) {
    std::vector<FrustumPlane> planes(6);

    // Extract the left plane
    planes[0].normal = glm::vec3(viewProjectionMatrix[0][3] + viewProjectionMatrix[0][0],
                                 viewProjectionMatrix[1][3] + viewProjectionMatrix[1][0],
                                 viewProjectionMatrix[2][3] + viewProjectionMatrix[2][0]);
    planes[0].distance = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][0];

    // Extract the right plane
    planes[1].normal = glm::vec3(viewProjectionMatrix[0][3] - viewProjectionMatrix[0][0],
                                 viewProjectionMatrix[1][3] - viewProjectionMatrix[1][0],
                                 viewProjectionMatrix[2][3] - viewProjectionMatrix[2][0]);
    planes[1].distance = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][0];

    // Extract the bottom plane
    planes[2].normal = glm::vec3(viewProjectionMatrix[0][3] + viewProjectionMatrix[0][1],
                                 viewProjectionMatrix[1][3] + viewProjectionMatrix[1][1],
                                 viewProjectionMatrix[2][3] + viewProjectionMatrix[2][1]);
    planes[2].distance = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][1];

    // Extract the top plane
    planes[3].normal = glm::vec3(viewProjectionMatrix[0][3] - viewProjectionMatrix[0][1],
                                 viewProjectionMatrix[1][3] - viewProjectionMatrix[1][1],
                                 viewProjectionMatrix[2][3] - viewProjectionMatrix[2][1]);
    planes[3].distance = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][1];

    // Extract the near plane
    planes[4].normal = glm::vec3(viewProjectionMatrix[0][3] + viewProjectionMatrix[0][2],
                                 viewProjectionMatrix[1][3] + viewProjectionMatrix[1][2],
                                 viewProjectionMatrix[2][3] + viewProjectionMatrix[2][2]);
    planes[4].distance = viewProjectionMatrix[3][3] + viewProjectionMatrix[3][2];

    // Extract the far plane
    planes[5].normal = glm::vec3(viewProjectionMatrix[0][3] - viewProjectionMatrix[0][2],
                                 viewProjectionMatrix[1][3] - viewProjectionMatrix[1][2],
                                 viewProjectionMatrix[2][3] - viewProjectionMatrix[2][2]);
    planes[5].distance = viewProjectionMatrix[3][3] - viewProjectionMatrix[3][2];

    // Normalize the plane normals
    for (auto& plane : planes) {
        float length = glm::length(plane.normal);
        plane.normal /= length;
        plane.distance /= length;
    }

    return planes;
}

bool MeshRenderer::isBoxInFrustum(const std::vector<FrustumPlane>& planes, const glm::vec3& min, const glm::vec3& max) {
    // Check each frustum plane
    for (const auto& plane : planes) {
        // Calculate the box's farthest point along the plane's normal
        glm::vec3 positiveVertex = {
                plane.normal.x >= 0 ? max.x : min.x,
                plane.normal.y >= 0 ? max.y : min.y,
                plane.normal.z >= 0 ? max.z : min.z,
        };

        // If the farthest point is outside the plane, the box is outside the frustum
        if (plane.distanceToPoint(positiveVertex) < 0) {
            return false;
        }
    }

    // If the box is not outside any plane, it's in the frustum
    return true;
}

glm::vec3 MeshRenderer::GetWorldMinBoundingBox(const std::shared_ptr<Model>& model, const std::shared_ptr<Transform>& transform) {
    glm::vec3 localMin = model->GetMinBoundingBox(); // Calculate local bounding box
    glm::vec3 scale = transform->GetScale(); // Get the scale of the transform

    // Apply scale to localMin
    glm::vec3 scaledMin = glm::vec3(localMin.x * scale.x, localMin.y * scale.y, localMin.z * scale.z);

    // Apply the object's position to get the world bounding box
    glm::vec3 worldMin = transform->GetPosition() + scaledMin;

    return worldMin;
}

glm::vec3 MeshRenderer::GetWorldMaxBoundingBox(const std::shared_ptr<Model>& model, const std::shared_ptr<Transform>& transform) {
    glm::vec3 localMax = model->GetMaxBoundingBox(); // Calculate local bounding box
    glm::vec3 scale = transform->GetScale(); // Get the scale of the transform

    // Apply scale to localMax
    glm::vec3 scaledMax = glm::vec3(localMax.x * scale.x, localMax.y * scale.y, localMax.z * scale.z);

    // Apply the object's position to get the world bounding box
    glm::vec3 worldMax = transform->GetPosition() + scaledMax;

    return worldMax;
}