// FILE: src/rendering/Lighting.h
#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

namespace t2c {

class Shader;
class Scene;

struct PointLight {
    glm::vec3 position  { 0.0f };
    glm::vec3 color     { 1.0f };
    float     intensity = 1.0f;
    float     radius    = 20.0f;
};

class Lighting {
public:
    Lighting() = default;

    void setDirectional(const glm::vec3& direction,
                        const glm::vec3& color,
                        float intensity);

    void setAmbient    (const glm::vec3& color) { m_ambient = color; }
    void addPointLight (const PointLight& light);
    void clearPointLights()                     { m_pointLights.clear(); }
    void applyToShader (Shader& shader) const;
    void update        (const Scene& scene, float time);

private:
    glm::vec3               m_ambient      { 0.05f, 0.05f, 0.10f };
    glm::vec3               m_dirDirection { 0.3f,  1.0f,  0.5f  };
    glm::vec3               m_dirColor     { 1.0f,  0.95f, 0.85f };
    float                   m_dirIntensity = 0.8f;
    std::vector<PointLight> m_pointLights;
    float                   m_glowPhase    = 0.0f;
};

} // namespace t2c