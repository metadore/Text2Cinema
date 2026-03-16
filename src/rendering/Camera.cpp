#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.h"
#include "../scene/SceneObject.h"
#include "../utils/Helpers.h"
#include "../utils/Config.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace t2c {

Camera::Camera() {}

void Camera::triggerShake(float intensity, float duration) {
    m_shakeIntensity = intensity;
    m_shakeDuration  = duration;
    m_shakeTimer     = 0.0f;
}

void Camera::panTo(const glm::vec3& newTarget, float duration) {
    m_panStart    = m_target;
    m_panEnd      = newTarget;
    m_panDuration = duration;
    m_panTimer    = 0.0f;
    m_panning     = true;
}

void Camera::zoomTo(float newFOV, float duration) {
    m_zoomStart    = m_fov;
    m_zoomEnd      = newFOV;
    m_zoomDuration = duration;
    m_zoomTimer    = 0.0f;
    m_zooming      = true;
}

void Camera::orbitBy(float angleRad, float duration) {
    m_orbitSpeed = angleRad / duration;
}

void Camera::update(float dt) {
    if (m_trackedObject && m_trackedObject->alive()) {
        glm::vec3 objPos = m_trackedObject->position();
        if (m_mode == CameraMode::Tracking) {
            glm::vec3 desiredPos = objPos + m_trackOffset;
            m_position = glm::mix(m_position, desiredPos, std::min(dt * 3.0f, 1.0f));
            m_target   = glm::mix(m_target,   objPos,     std::min(dt * 5.0f, 1.0f));
        }
    }

    if (m_mode == CameraMode::Orbit || m_orbitSpeed != 0.0f) {
        m_orbitAngle += m_orbitSpeed * dt;
        m_position = {
            m_target.x + std::cos(m_orbitAngle) * m_orbitRadius,
            m_target.y + m_orbitHeight,
            m_target.z + std::sin(m_orbitAngle) * m_orbitRadius
        };
    }

    if (m_panning) {
        m_panTimer += dt;
        float t   = utils::smoothstep(0.0f, m_panDuration, m_panTimer);
        m_target  = m_panStart + (m_panEnd - m_panStart) * t;
        if (m_panTimer >= m_panDuration) m_panning = false;
    }

    if (m_zooming) {
        m_zoomTimer += dt;
        float t = utils::smoothstep(0.0f, m_zoomDuration, m_zoomTimer);
        m_fov   = m_zoomStart + (m_zoomEnd - m_zoomStart) * t;
        if (m_zoomTimer >= m_zoomDuration) m_zooming = false;
    }

    if (m_shakeTimer < m_shakeDuration) {
        m_shakeTimer += dt;
        float fade    = 1.0f - (m_shakeTimer / m_shakeDuration);
        m_shakeOffset = {
            utils::randFloat(-1.0f, 1.0f) * m_shakeIntensity * fade,
            utils::randFloat(-1.0f, 1.0f) * m_shakeIntensity * fade,
            0.0f
        };
    } else {
        m_shakeOffset = { 0.0f, 0.0f, 0.0f };
    }
}

glm::mat4 Camera::viewMatrix() const {
    glm::vec3 pos = m_position + m_shakeOffset;
    return glm::lookAt(pos, m_target + m_shakeOffset * 0.5f, m_up);
}

glm::mat4 Camera::projectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(m_fov), aspectRatio, CAM_NEAR, CAM_FAR);
}

} // namespace t2c