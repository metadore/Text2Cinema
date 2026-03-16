#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <cstdint>

namespace t2c {

class SceneObject;

enum class ParticleType {
    Fire,
    Smoke,
    Sparks,
    Explosion,
    Rain,
    Splash,
    Shockwave,
    Custom
};

struct Particle {
    glm::vec3 position   { 0.0f };
    glm::vec3 velocity   { 0.0f };
    glm::vec4 colorStart { 1.0f };
    glm::vec4 colorEnd   { 0.0f };
    float     size       { 1.0f };
    float     sizeEnd    { 0.0f };
    float     life       { 1.0f };
    float     maxLife    { 1.0f };
    float     gravityMul { 1.0f };
    bool      alive      { true };
};

class ParticleSystem {
public:
    explicit ParticleSystem(ParticleType type,
                            const glm::vec3& origin = glm::vec3(0.0f));
    ~ParticleSystem() = default;

    void setEmissionRate(float particlesPerSecond) { m_emissionRate = particlesPerSecond; }
    void setFollowTarget(SceneObject* obj)         { m_followTarget = obj; }
    void setOrigin(const glm::vec3& o)             { m_origin = o; }

    void burst(int count);
    void update(float dt);

    const std::vector<float>& positionBuffer() const { return m_posBuffer; }
    const std::vector<float>& colorBuffer()    const { return m_colBuffer; }
    const std::vector<float>& sizeBuffer()     const { return m_sizeBuffer; }
    int liveCount() const { return m_liveCount; }

    ParticleType type() const { return m_type; }

private:
    void spawnParticle(const glm::vec3& origin);
    void applyTypeDefaults(Particle& p) const;
    void rebuildGPUBuffers();

    ParticleType          m_type;
    glm::vec3             m_origin;
    SceneObject*          m_followTarget  = nullptr;

    float                 m_emissionRate  = 0.0f;
    float                 m_emissionAccum = 0.0f;

    std::vector<Particle> m_particles;
    int                   m_liveCount     = 0;

    std::vector<float>    m_posBuffer;
    std::vector<float>    m_colBuffer;
    std::vector<float>    m_sizeBuffer;
};

} // namespace t2c