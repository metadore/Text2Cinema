#define GLM_ENABLE_EXPERIMENTAL

#include "SceneObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace t2c {

uint32_t SceneObject::s_nextId = 1;

SceneObject::SceneObject(ObjectType type, const std::string& name)
    : m_id(s_nextId++), m_name(name), m_type(type)
{}

void SceneObject::update(float dt) {
    m_age += dt;
    if (m_lifetime > 0.0f && m_age >= m_lifetime) {
        m_alive = false;
    }
}

glm::mat4 SceneObject::modelMatrix() const {
    glm::mat4 T = glm::translate(glm::mat4(1.0f), m_position);
    glm::mat4 R = glm::toMat4(m_rotation);
    glm::mat4 S = glm::scale(glm::mat4(1.0f), m_scale);
    return T * R * S;
}

} // namespace t2c