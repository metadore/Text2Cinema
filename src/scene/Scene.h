#pragma once

#include "SceneObject.h"
#include "../parser/TextParser.h"
#include "../simulation/ParticleSystem.h"

#include <vector>
#include <memory>
#include <functional>
#include <string>

namespace t2c {

using EventCallback = std::function<void(const std::string& eventType,
                                          SceneObject* sourceObj)>;

class Scene {
public:
    Scene()  = default;
    ~Scene() = default;

    void buildFromDescriptor(const SceneDescriptor& desc);

    SceneObject* addObject(std::unique_ptr<SceneObject> obj);
    SceneObject* findObject(const std::string& name) const;
    SceneObject* findObjectByType(ObjectType type) const;

    const std::vector<std::unique_ptr<SceneObject>>& objects() const {
        return m_objects;
    }

    void  update(float dt);
    float time()    const { return m_simTime; }
    bool  running() const { return m_running; }
    void  stop()          { m_running = false; }

    const ParsedEnvironment& environment() const { return m_environment; }
    bool hasCameraShake() const { return m_cameraShake; }

    void registerEventCallback(EventCallback cb) {
        m_eventCallbacks.push_back(std::move(cb));
    }
    void fireEvent(const std::string& type, SceneObject* src = nullptr);

    void addParticleSystem(std::shared_ptr<ParticleSystem> ps) {
        m_particleSystems.push_back(std::move(ps));
    }
    const std::vector<std::shared_ptr<ParticleSystem>>& particleSystems() const {
        return m_particleSystems;
    }

    float groundY() const { return m_groundY; }

private:
    void buildPrimaryObject(const SceneDescriptor& desc);
    void buildEnvironment  (const SceneDescriptor& desc);
    void scheduleEvents    (const SceneDescriptor& desc);

    std::vector<std::unique_ptr<SceneObject>>    m_objects;
    std::vector<std::shared_ptr<ParticleSystem>> m_particleSystems;
    std::vector<EventCallback>                   m_eventCallbacks;
    std::vector<std::string>                     m_pendingImpactEffects;

    ParsedEnvironment m_environment;
    bool              m_cameraShake  = false;
    float             m_simTime      = 0.0f;
    bool              m_running      = true;
    float             m_groundY      = 0.0f;

    bool  m_impactFired    = false;
    float m_impactTriggerY = 0.5f;
};

} // namespace t2c