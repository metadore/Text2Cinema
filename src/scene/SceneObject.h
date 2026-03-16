#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <memory>

namespace t2c {

enum class ObjectType {
    Sphere,
    Meteor,
    Comet,
    Asteroid,
    Spacecraft,
    Aircraft,
    Rock,
    Plane,
    LightSource,
    ParticleEmitter,
    Custom
};

struct Material {
    glm::vec4  color        = { 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec4  emissive     = { 0.0f, 0.0f, 0.0f, 0.0f };
    float      metallic     = 0.0f;
    float      roughness    = 0.5f;
    bool       glowing      = false;
    float      glowStrength = 0.0f;
};

class SceneObject {
public:
    explicit SceneObject(ObjectType type, const std::string& name = "object");
    virtual ~SceneObject() = default;

    uint32_t           id()   const { return m_id; }
    const std::string& name() const { return m_name; }
    ObjectType         type() const { return m_type; }

    glm::vec3 position() const { return m_position; }
    glm::vec3 velocity() const { return m_velocity; }
    glm::quat rotation() const { return m_rotation; }
    glm::vec3 scale()    const { return m_scale; }

    void setPosition(const glm::vec3& p) { m_position = p; }
    void setVelocity(const glm::vec3& v) { m_velocity = v; }
    void setRotation(const glm::quat& q) { m_rotation = q; }
    void setScale   (const glm::vec3& s) { m_scale    = s; }
    void setScale   (float s)            { m_scale    = glm::vec3(s); }

    Material&       material()       { return m_material; }
    const Material& material() const { return m_material; }

    bool  alive()    const { return m_alive; }
    void  kill()           { m_alive = false; }
    float age()      const { return m_age; }
    float lifetime() const { return m_lifetime; }
    void  setLifetime(float l) { m_lifetime = l; }

    void* physicsBody() const       { return m_physicsBody; }
    void  setPhysicsBody(void* pb)  { m_physicsBody = pb; }

    virtual void update(float dt);

    glm::mat4 modelMatrix() const;

protected:
    uint32_t    m_id;
    std::string m_name;
    ObjectType  m_type;

    glm::vec3   m_position { 0.0f };
    glm::vec3   m_velocity { 0.0f };
    glm::quat   m_rotation { 1.0f, 0.0f, 0.0f, 0.0f };
    glm::vec3   m_scale    { 1.0f };

    Material    m_material;

    bool        m_alive    { true };
    float       m_age      { 0.0f };
    float       m_lifetime { -1.0f };

    void*       m_physicsBody { nullptr };

private:
    static uint32_t s_nextId;
};

} // namespace t2c