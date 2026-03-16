// FILE: src/rendering/Lighting.cpp
#include "Lighting.h"
#include "Shader.h"
#include "../scene/Scene.h"
#include "../scene/SceneObject.h"

#include <cmath>
#include <string>

namespace t2c {

void Lighting::setDirectional(const glm::vec3& direction,
                               const glm::vec3& color,
                               float intensity)
{
    m_dirDirection = glm::normalize(direction);
    m_dirColor     = color;
    m_dirIntensity = intensity;
}

void Lighting::addPointLight(const PointLight& light) {
    if (static_cast<int>(m_pointLights.size()) < 8)
        m_pointLights.push_back(light);
}

void Lighting::applyToShader(Shader& shader) const {
    shader.setVec3("u_AmbientColor",  m_ambient);
    shader.setVec3("u_DirLightDir",   m_dirDirection);
    shader.setVec3("u_DirLightColor", m_dirColor * m_dirIntensity);

    int n = static_cast<int>(m_pointLights.size());
    shader.setInt("u_NumPointLights", n);
    for (int i = 0; i < n && i < 8; ++i) {
        std::string base = "u_PointLights[" + std::to_string(i) + "]";
        shader.setVec3 (base + ".position", m_pointLights[i].position);
        shader.setVec3 (base + ".color",
                        m_pointLights[i].color * m_pointLights[i].intensity);
        shader.setFloat(base + ".radius",   m_pointLights[i].radius);
    }
}

void Lighting::update(const Scene& scene, float time) {
    m_glowPhase += time;
    clearPointLights();

    for (const auto& obj : scene.objects()) {
        if (!obj->alive()) continue;
        const Material& mat = obj->material();
        if (mat.glowing && mat.glowStrength > 0.0f) {
            float pulse      = 1.0f + 0.2f * std::sin(m_glowPhase * 4.0f);
            PointLight pl;
            pl.position  = obj->position();
            pl.color     = glm::vec3(mat.emissive);
            pl.intensity = mat.glowStrength * pulse;
            pl.radius    = 15.0f * obj->scale().x;
            addPointLight(pl);
        }
    }
}

} // namespace t2c