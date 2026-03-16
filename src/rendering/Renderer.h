// FILE: src/rendering/Renderer.h
#pragma once

#include "Camera.h"
#include "Lighting.h"
#include "Shader.h"
#include "../simulation/Environment.h"

#include <vector>

namespace t2c {

class Scene;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init(int width, int height);

    void renderFrame(const Scene&       scene,
                     const Camera&      camera,
                     const Environment& env,
                     float              time);

    void resize(int width, int height);

    Camera&   camera()   { return m_camera; }
    Lighting& lighting() { return m_lighting; }

private:
    void renderSky      (const Environment& env, const Camera& cam);
    void renderStars    (const Environment& env, const Camera& cam);
    void renderGround   (const Scene& scene,     const Camera& cam);
    void renderObjects  (const Scene& scene,     const Camera& cam);
    void renderParticles(const Scene& scene,     const Camera& cam);

    void buildSphereMesh ();
    void buildQuadMesh   ();
    void buildParticleVBO();
    void buildStarVBO    (const std::vector<float>& positions);

    Shader m_objectShader;
    Shader m_groundShader;
    Shader m_particleShader;
    Shader m_skyShader;
    Shader m_starShader;

    unsigned int m_sphereVAO        = 0;
    unsigned int m_sphereVBO        = 0;
    unsigned int m_sphereEBO        = 0;
    int          m_sphereIndexCount = 0;

    unsigned int m_quadVAO          = 0;
    unsigned int m_quadVBO          = 0;

    unsigned int m_particleVAO      = 0;
    unsigned int m_particlePosVBO   = 0;
    unsigned int m_particleColVBO   = 0;
    unsigned int m_particleSzVBO    = 0;

    unsigned int m_starVAO          = 0;
    unsigned int m_starVBO          = 0;
    int          m_starCount        = 0;
    bool         m_starsBuilt       = false;

    Camera   m_camera;
    Lighting m_lighting;
    int      m_width  = 0;
    int      m_height = 0;
};

} // namespace t2c