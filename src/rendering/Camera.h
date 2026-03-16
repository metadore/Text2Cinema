#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace t2c {

class SceneObject;

enum class CameraMode {
    Fixed,
    Tracking,
    Orbit,
    Cinematic
};

class Camera {
public:
    Camera();

    void setMode    (CameraMode mode)      { m_mode = mode; }
    void setTarget  (const glm::vec3& t)   { m_target = t; }
    void setPosition(const glm::vec3& p)   { m_position = p; }
    void setFOV     (float fovDegrees)     { m_fov = fovDegrees; }
    void trackObject(SceneObject* obj)     { m_trackedObject = obj; }

    void triggerShake(float intensity, float duration);
    void panTo   (const glm::vec3& newTarget, float duration);
    void zoomTo  (float newFOV, float duration);
    void orbitBy (float angleRad, float duration);

    void update(float dt);

    glm::mat4 viewMatrix()                    const;
    glm::mat4 projectionMatrix(float aspect)  const;

    const glm::vec3& position() const { return m_position; }
    const glm::vec3& target()   const { return m_target;   }
    float            fov()      const { return m_fov;      }

private:
    CameraMode   m_mode     = CameraMode::Tracking;
    glm::vec3    m_position { 0.0f, 5.0f, 20.0f };
    glm::vec3    m_target   { 0.0f, 0.0f,  0.0f };
    glm::vec3    m_up       { 0.0f, 1.0f,  0.0f };
    float        m_fov      = 60.0f;

    SceneObject* m_trackedObject = nullptr;
    glm::vec3    m_trackOffset   { 0.0f, 2.0f, 12.0f };

    float        m_orbitAngle  = 0.0f;
    float        m_orbitRadius = 20.0f;
    float        m_orbitSpeed  = 0.0f;
    float        m_orbitHeight = 5.0f;

    glm::vec3    m_panStart   { 0.0f };
    glm::vec3    m_panEnd     { 0.0f };
    float        m_panTimer   = 0.0f;
    float        m_panDuration= 0.0f;
    bool         m_panning    = false;

    float        m_zoomStart   = 60.0f;
    float        m_zoomEnd     = 60.0f;
    float        m_zoomTimer   = 0.0f;
    float        m_zoomDuration= 0.0f;
    bool         m_zooming     = false;

    float        m_shakeIntensity = 0.0f;
    float        m_shakeDuration  = 0.0f;
    float        m_shakeTimer     = 0.0f;
    glm::vec3    m_shakeOffset    { 0.0f };
};

} // namespace t2c