//
// Created by Jacek on 31.05.2024.
//

#ifndef SANDBOX_SHOVELRENDERER_H
#define SANDBOX_SHOVELRENDERER_H

#include "Managers/ResourceManager.h"
#include "Managers/ComponentsManager.h"
#include "Component/Camera.h"

class ShovelRenderer : public Component {
public:
    ShovelRenderer();

    ShovelRenderer(Model* model, Shader* shader);

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Initiate() override;

    void SetShader(shared_ptr<Shader> newShader) {
        _shader = newShader;
    }

    void Update() override;

    shared_ptr<Model> _model; // Raw pointer to the model
    shared_ptr<Shader> _shader; // Raw pointer to the shader
    shared_ptr<Shader> _outlineShader; // Raw pointer to the outline shader
    std::shared_ptr<Camera> _cameraRef;
    int framesRendered;
    void RenderShovel(glm::mat4 ctm);
};


#endif //SANDBOX_SHOVELRENDERER_H
