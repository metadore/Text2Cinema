#include "Environment.h"
#include "../utils/Helpers.h"

#include <cmath>

namespace t2c {

void Environment::configure(const std::string& sky,
                             const std::string& ground,
                             bool fog,
                             bool rain)
{
    EnvironmentSettings& s = m_settings;

    if (sky == "night") {
        s.skyColorBottom = { 0.01f, 0.01f, 0.06f };
        s.skyColorTop    = { 0.00f, 0.00f, 0.03f };
        s.sunIntensity   = 0.05f;
        s.starsEnabled   = true;
        s.ambientColor   = { 0.03f, 0.03f, 0.08f };
    } else if (sky == "stormy") {
        s.skyColorBottom = { 0.10f, 0.10f, 0.13f };
        s.skyColorTop    = { 0.06f, 0.06f, 0.10f };
        s.sunIntensity   = 0.2f;
        s.starsEnabled   = false;
        s.fogEnabled     = true;
        s.fogDensity     = 0.03f;
        s.fogColor       = { 0.3f, 0.3f, 0.35f };
        s.ambientColor   = { 0.12f, 0.12f, 0.15f };
    } else if (sky == "sunset") {
        s.skyColorBottom = { 0.85f, 0.35f, 0.10f };
        s.skyColorTop    = { 0.20f, 0.10f, 0.40f };
        s.sunColor       = { 1.0f, 0.6f, 0.2f };
        s.sunIntensity   = 1.2f;
        s.starsEnabled   = false;
        s.ambientColor   = { 0.20f, 0.10f, 0.08f };
    } else if (sky == "space") {
        s.skyColorBottom = { 0.00f, 0.00f, 0.00f };
        s.skyColorTop    = { 0.00f, 0.00f, 0.00f };
        s.sunIntensity   = 2.0f;
        s.starsEnabled   = true;
        s.starCount      = 5000;
        s.ambientColor   = { 0.01f, 0.01f, 0.01f };
    } else {
        s.skyColorBottom = { 0.01f, 0.02f, 0.08f };
        s.skyColorTop    = { 0.00f, 0.00f, 0.04f };
        s.starsEnabled   = true;
    }

    if (ground == "ocean") {
        s.oceanEnabled = true;
    }

    if (fog) {
        s.fogEnabled = true;
        s.fogDensity = 0.015f;
    }
    if (rain) {
        s.fogEnabled = true;
        s.fogDensity = std::max(s.fogDensity, 0.02f);
    }

    if (s.starsEnabled) generateStars();
}

glm::vec3 Environment::skyColorAt(float t) const {
    const auto& s = m_settings;
    return s.skyColorBottom + (s.skyColorTop - s.skyColorBottom) * t;
}

void Environment::generateStars() {
    m_starPositions.clear();
    int n = m_settings.starCount;
    m_starPositions.reserve(n * 3);

    for (int i = 0; i < n; ++i) {
        glm::vec3 v = utils::randUnitVec3();
        if (v.y < -0.2f) v.y = -v.y;
        v *= 500.0f;
        m_starPositions.push_back(v.x);
        m_starPositions.push_back(v.y);
        m_starPositions.push_back(v.z);
    }
}

} // namespace t2c