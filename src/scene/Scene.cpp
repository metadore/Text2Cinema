// FILE: src/scene/Scene.cpp
#include "Scene.h"
#include "../utils/Helpers.h"
#include "../utils/Config.h"

#include <glm/glm.hpp>
#include <iostream>

namespace t2c {

void Scene::buildFromDescriptor(const SceneDescriptor& desc) {
    m_environment = desc.environment;
    m_cameraShake = desc.hasCameraShake;
    m_simTime     = 0.0f;
    m_running     = true;
    m_impactFired = false;

    buildPrimaryObject(desc);
    buildEnvironment(desc);
    scheduleEvents(desc);

    std::cout << "[Scene] Built with " << m_objects.size()
              << " objects, " << m_particleSystems.size()
              << " particle systems.\n";
}

void Scene::buildPrimaryObject(const SceneDescriptor& desc) {
    if (desc.objects.empty()) return;

    const ParsedObject& po = desc.objects.front();

    auto toEnum = [](const std::string& t) -> ObjectType {
        if (t == "meteor"    ) return ObjectType::Meteor;
        if (t == "comet"     ) return ObjectType::Comet;
        if (t == "asteroid"  ) return ObjectType::Asteroid;
        if (t == "spacecraft") return ObjectType::Spacecraft;
        if (t == "aircraft"  ) return ObjectType::Aircraft;
        if (t == "rock"      ) return ObjectType::Rock;
        return ObjectType::Sphere;
    };

    auto obj = std::make_unique<SceneObject>(toEnum(po.type), po.type);
    obj->setScale(po.scale);

    if (desc.motion.verb == "fall") {
        obj->setPosition({ 0.0f, 18.0f, 0.0f });
        obj->setVelocity({
            utils::randFloat(-0.5f, 0.5f),
            -4.0f,
            utils::randFloat(-0.5f, 0.5f)
        });
    } else if (desc.motion.verb == "fly") {
        obj->setPosition({ -15.0f, 8.0f, 0.0f });
        obj->setVelocity({ 4.0f, utils::randFloat(-0.5f, 0.5f), 0.0f });
    } else if (desc.motion.verb == "rise") {
        obj->setPosition({ 0.0f, -2.0f, 0.0f });
        obj->setVelocity({ 0.0f, 3.0f, 0.0f });
    } else {
        obj->setPosition({ 8.0f, 0.0f, 0.0f });
        obj->setVelocity({ 0.0f, 0.0f, 2.5f });
    }

    Material& mat = obj->material();
    if (po.material == "glowing") {
        mat.color        = { 1.0f, 0.6f, 0.1f, 1.0f };
        mat.emissive     = { 1.0f, 0.4f, 0.05f, 1.0f };
        mat.glowing      = true;
        mat.glowStrength = 2.0f;
    } else if (po.material == "fiery") {
        mat.color        = { 1.0f, 0.3f, 0.0f, 1.0f };
        mat.emissive     = { 0.8f, 0.2f, 0.0f, 1.0f };
        mat.glowing      = true;
        mat.glowStrength = 1.5f;
    } else if (po.material == "icy") {
        mat.color        = { 0.7f, 0.9f, 1.0f, 1.0f };
        mat.emissive     = { 0.1f, 0.3f, 0.5f, 1.0f };
        mat.glowing      = true;
        mat.glowStrength = 0.8f;
    } else if (po.material == "rocky") {
        mat.color     = { 0.55f, 0.45f, 0.35f, 1.0f };
        mat.roughness = 0.9f;
    } else if (po.material == "metallic") {
        mat.color     = { 0.8f, 0.8f, 0.9f, 1.0f };
        mat.metallic  = 0.9f;
        mat.roughness = 0.2f;
    } else {
        mat.color = { 0.9f, 0.9f, 0.9f, 1.0f };
    }

    addObject(std::move(obj));
}

void Scene::buildEnvironment(const SceneDescriptor& desc) {
    auto ground = std::make_unique<SceneObject>(ObjectType::Plane, "ground");
    ground->setPosition({ 0.0f, m_groundY, 0.0f });
    ground->setScale(200.0f);

    Material& gm = ground->material();
    if (desc.environment.ground == "ocean") {
        gm.color     = { 0.05f, 0.2f, 0.45f, 0.85f };
        gm.roughness = 0.3f;
    } else if (desc.environment.ground == "desert") {
        gm.color     = { 0.85f, 0.72f, 0.45f, 1.0f };
        gm.roughness = 0.95f;
    } else if (desc.environment.ground == "void") {
        gm.color = { 0.0f, 0.0f, 0.0f, 0.0f };
    } else {
        gm.color     = { 0.15f, 0.35f, 0.15f, 1.0f };
        gm.roughness = 0.85f;
    }
    addObject(std::move(ground));

    if (desc.environment.hasRain) {
        auto rain = std::make_shared<ParticleSystem>(
            ParticleType::Rain, glm::vec3(0.0f, 20.0f, 0.0f));
        rain->setEmissionRate(800.0f);
        addParticleSystem(rain);
    }
}

void Scene::scheduleEvents(const SceneDescriptor& desc) {
    for (const auto& ev : desc.events) {
        if (ev.trigger == "always" && !m_objects.empty()) {
            if (ev.effect == "fire") {
                auto ps = std::make_shared<ParticleSystem>(
                    ParticleType::Fire,
                    m_objects.front()->position());
                ps->setEmissionRate(150.0f);
                ps->setFollowTarget(m_objects.front().get());
                addParticleSystem(ps);
            } else if (ev.effect == "smoke") {
                auto ps = std::make_shared<ParticleSystem>(
                    ParticleType::Smoke,
                    m_objects.front()->position());
                ps->setEmissionRate(60.0f);
                ps->setFollowTarget(m_objects.front().get());
                addParticleSystem(ps);
            }
        }
    }
    for (const auto& ev : desc.events)
        if (ev.trigger == "impact")
            m_pendingImpactEffects.push_back(ev.effect);
}

SceneObject* Scene::addObject(std::unique_ptr<SceneObject> obj) {
    m_objects.push_back(std::move(obj));
    return m_objects.back().get();
}

SceneObject* Scene::findObject(const std::string& name) const {
    for (const auto& o : m_objects)
        if (o->name() == name) return o.get();
    return nullptr;
}

SceneObject* Scene::findObjectByType(ObjectType t) const {
    for (const auto& o : m_objects)
        if (o->type() == t) return o.get();
    return nullptr;
}

void Scene::update(float dt) {
    if (!m_running) return;
    m_simTime += dt;

    for (auto& obj : m_objects) {
        if (!obj->alive() || obj->type() == ObjectType::Plane) continue;
        if (obj->physicsBody() == nullptr) {
            glm::vec3 vel = obj->velocity();
            vel.y += GRAVITY * dt * 0.5f;
            obj->setVelocity(vel);
            obj->setPosition(obj->position() + obj->velocity() * dt);
        }
        obj->update(dt);
    }

    if (!m_impactFired) {
        SceneObject* primary = nullptr;
        for (const auto& o : m_objects)
            if (o->type() != ObjectType::Plane &&
                o->type() != ObjectType::LightSource) {
                primary = o.get();
                break;
            }
        if (primary &&
            primary->position().y <= m_groundY + m_impactTriggerY) {
            m_impactFired = true;
            fireEvent("impact", primary);
        }
    }

    for (auto& ps : m_particleSystems)
        ps->update(dt);

    if (m_simTime >= SIMULATION_DURATION)
        m_running = false;
}

void Scene::fireEvent(const std::string& type, SceneObject* src) {
    std::cout << "[Scene] Event fired: " << type << "\n";

    if (type == "impact" && src) {
        glm::vec3 impactPos  = src->position();
        impactPos.y = m_groundY;

        for (const auto& effect : m_pendingImpactEffects) {
            if (effect == "explosion") {
                auto ps = std::make_shared<ParticleSystem>(
                    ParticleType::Explosion, impactPos);
                ps->burst(2000);
                addParticleSystem(ps);
            } else if (effect == "sparks") {
                auto ps = std::make_shared<ParticleSystem>(
                    ParticleType::Sparks, impactPos);
                ps->burst(800);
                addParticleSystem(ps);
            } else if (effect == "splash") {
                auto ps = std::make_shared<ParticleSystem>(
                    ParticleType::Splash, impactPos);
                ps->burst(1200);
                addParticleSystem(ps);
            } else if (effect == "shockwave") {
                auto ps = std::make_shared<ParticleSystem>(
                    ParticleType::Shockwave, impactPos);
                ps->burst(300);
                addParticleSystem(ps);
            }
        }
        src->kill();
    }

    for (auto& cb : m_eventCallbacks)
        cb(type, src);
}

} // namespace t2c