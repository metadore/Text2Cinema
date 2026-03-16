#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace t2c {

struct EnvironmentSettings {
    glm::vec3 skyColorBottom { 0.02f, 0.02f, 0.10f };
    glm::vec3 skyColorTop    { 0.00f, 0.00f, 0.04f };

    glm::vec3 ambientColor   { 0.05f, 0.05f, 0.10f };
    glm::vec3 sunDirection   { 0.3f,  1.0f,  0.5f  };
    glm::vec3 sunColor       { 1.0f,  0.95f, 0.85f };
    float     sunIntensity   = 0.8f;

    bool      fogEnabled     = false;
    glm::vec3 fogColor       { 0.5f, 0.5f, 0.6f };
    float     fogDensity     = 0.02f;

    bool      starsEnabled   = true;
    int       starCount      = 2000;

    bool      oceanEnabled   = false;
    float     waveAmplitude  = 0.2f;
    float     waveFrequency  = 1.5f;
};

class Environment {
public:
    Environment() = default;

    void configure(const std::string& sky,
                   const std::string& ground,
                   bool fog,
                   bool rain);

    const EnvironmentSettings& settings() const { return m_settings; }

    glm::vec3 skyColorAt(float t) const;

    const std::vector<float>& starPositions() const { return m_starPositions; }

private:
    void generateStars();

    EnvironmentSettings  m_settings;
    std::vector<float>   m_starPositions;
};

} // namespace t2c