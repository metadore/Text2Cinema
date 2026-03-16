#include "ParticleSystem.h"
#include "../scene/SceneObject.h"
#include "../utils/Helpers.h"
#include "../utils/Config.h"

#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

namespace t2c {

ParticleSystem::ParticleSystem(ParticleType type, const glm::vec3& origin)
    : m_type(type), m_origin(origin)
{
    m_particles.reserve(MAX_PARTICLES);
}

void ParticleSystem::burst(int count) {
    glm::vec3 origin = m_followTarget
        ? m_followTarget->position()
        : m_origin;

    for (int i = 0; i < count; ++i) {
        if (static_cast<int>(m_particles.size()) >= MAX_PARTICLES) break;
        spawnParticle(origin);
    }
    rebuildGPUBuffers();
}

void ParticleSystem::update(float dt) {
    if (m_followTarget && m_followTarget->alive()) {
        m_origin = m_followTarget->position();
    }

    if (m_emissionRate > 0.0f) {
        m_emissionAccum += m_emissionRate * dt;
        int toSpawn = static_cast<int>(m_emissionAccum);
        m_emissionAccum -= toSpawn;
        for (int i = 0; i < toSpawn; ++i) {
            if (static_cast<int>(m_particles.size()) < MAX_PARTICLES) {
                spawnParticle(m_origin);
            }
        }
    }

    constexpr float G = -9.81f;
    m_liveCount = 0;

    for (auto& p : m_particles) {
        if (!p.alive) continue;

        p.life -= dt;
        if (p.life <= 0.0f) { p.alive = false; continue; }

        p.velocity.y += G * p.gravityMul * dt;
        p.velocity   *= (1.0f - 0.02f * dt * 60.0f);
        p.position   += p.velocity * dt;

        if (p.position.y < -2.0f) { p.alive = false; continue; }

        ++m_liveCount;
    }

    if (m_particles.size() > 2048) {
        m_particles.erase(
            std::remove_if(m_particles.begin(), m_particles.end(),
                           [](const Particle& p){ return !p.alive; }),
            m_particles.end());
    }

    rebuildGPUBuffers();
}

void ParticleSystem::spawnParticle(const glm::vec3& origin) {
    Particle p;
    p.position = origin;
    applyTypeDefaults(p);
    m_particles.push_back(p);
}

void ParticleSystem::applyTypeDefaults(Particle& p) const {
    using namespace utils;

    switch (m_type) {

    case ParticleType::Fire:
        p.velocity   = { randFloat(-0.5f,0.5f), randFloat(2.0f,5.0f),
                         randFloat(-0.5f,0.5f) };
        p.colorStart = { 1.0f, randFloat(0.3f,0.7f), 0.0f, 1.0f };
        p.colorEnd   = { 0.6f, 0.0f, 0.0f, 0.0f };
        p.size       = randFloat(0.3f, 0.7f);
        p.sizeEnd    = 0.0f;
        p.maxLife    = randFloat(0.4f, 1.0f);
        p.life       = p.maxLife;
        p.gravityMul = -0.2f;
        break;

    case ParticleType::Smoke:
        p.velocity   = { randFloat(-0.3f,0.3f), randFloat(0.5f,1.5f),
                         randFloat(-0.3f,0.3f) };
        p.colorStart = { 0.4f, 0.4f, 0.4f, 0.6f };
        p.colorEnd   = { 0.2f, 0.2f, 0.2f, 0.0f };
        p.size       = randFloat(0.4f, 0.8f);
        p.sizeEnd    = randFloat(1.2f, 2.5f);
        p.maxLife    = randFloat(1.5f, 3.5f);
        p.life       = p.maxLife;
        p.gravityMul = -0.05f;
        break;

    case ParticleType::Sparks:
        p.velocity   = randUnitVec3() * randFloat(3.0f, 10.0f);
        p.colorStart = { 1.0f, randFloat(0.6f,1.0f), 0.0f, 1.0f };
        p.colorEnd   = { 1.0f, 0.0f, 0.0f, 0.0f };
        p.size       = randFloat(0.1f, 0.3f);
        p.sizeEnd    = 0.0f;
        p.maxLife    = randFloat(0.4f, 1.2f);
        p.life       = p.maxLife;
        p.gravityMul = 0.6f;
        break;

    case ParticleType::Explosion:
        p.velocity   = randUnitVec3() * randFloat(5.0f, 18.0f);
        p.colorStart = { 1.0f, randFloat(0.3f,0.8f), 0.0f, 1.0f };
        p.colorEnd   = { 0.3f, 0.0f, 0.0f, 0.0f };
        p.size       = randFloat(0.2f, 0.8f);
        p.sizeEnd    = 0.0f;
        p.maxLife    = randFloat(0.5f, 2.0f);
        p.life       = p.maxLife;
        p.gravityMul = 0.4f;
        break;

    case ParticleType::Rain:
        p.position  += glm::vec3{ randFloat(-20.f,20.f), 0.0f,
                                   randFloat(-20.f,20.f) };
        p.velocity   = { randFloat(-0.3f,0.3f), randFloat(-8.0f,-5.0f),
                         randFloat(-0.3f,0.3f) };
        p.colorStart = { 0.6f, 0.7f, 1.0f, 0.7f };
        p.colorEnd   = { 0.6f, 0.7f, 1.0f, 0.0f };
        p.size       = 0.08f;
        p.sizeEnd    = 0.08f;
        p.maxLife    = randFloat(0.8f, 2.0f);
        p.life       = p.maxLife;
        p.gravityMul = 1.0f;
        break;

    case ParticleType::Splash:
        p.velocity   = { randFloat(-3.0f,3.0f), randFloat(3.0f,8.0f),
                         randFloat(-3.0f,3.0f) };
        p.colorStart = { 0.4f, 0.7f, 1.0f, 0.9f };
        p.colorEnd   = { 0.2f, 0.5f, 1.0f, 0.0f };
        p.size       = randFloat(0.15f, 0.4f);
        p.sizeEnd    = 0.0f;
        p.maxLife    = randFloat(0.5f, 1.5f);
        p.life       = p.maxLife;
        p.gravityMul = 1.0f;
        break;

    case ParticleType::Shockwave:
        {
            float angle = utils::randFloat(0.0f, 6.2832f);
            float speed = utils::randFloat(8.0f, 16.0f);
            p.velocity  = { std::cos(angle)*speed, 0.2f,
                            std::sin(angle)*speed };
            p.colorStart= { 0.9f, 0.9f, 1.0f, 0.8f };
            p.colorEnd  = { 0.5f, 0.5f, 1.0f, 0.0f };
            p.size      = randFloat(0.1f, 0.3f);
            p.sizeEnd   = 0.0f;
            p.maxLife   = randFloat(0.3f, 0.8f);
            p.life      = p.maxLife;
            p.gravityMul= 0.0f;
        }
        break;

    default:
        p.velocity   = randUnitVec3() * 2.0f;
        p.colorStart = { 1,1,1,1 };
        p.colorEnd   = { 1,1,1,0 };
        p.size       = 0.3f;
        p.maxLife    = 1.0f;
        p.life       = 1.0f;
        break;
    }
}

void ParticleSystem::rebuildGPUBuffers() {
    m_posBuffer .clear();
    m_colBuffer .clear();
    m_sizeBuffer.clear();

    m_posBuffer .reserve(m_liveCount * 3);
    m_colBuffer .reserve(m_liveCount * 4);
    m_sizeBuffer.reserve(m_liveCount);

    for (const auto& p : m_particles) {
        if (!p.alive) continue;

        float t       = 1.0f - (p.life / p.maxLife);
        glm::vec4 col = p.colorStart + (p.colorEnd - p.colorStart) * t;
        float size    = p.size       + (p.sizeEnd  - p.size)       * t;

        m_posBuffer .push_back(p.position.x);
        m_posBuffer .push_back(p.position.y);
        m_posBuffer .push_back(p.position.z);

        m_colBuffer .push_back(col.r);
        m_colBuffer .push_back(col.g);
        m_colBuffer .push_back(col.b);
        m_colBuffer .push_back(col.a);

        m_sizeBuffer.push_back(size);
    }
}

} // namespace t2c