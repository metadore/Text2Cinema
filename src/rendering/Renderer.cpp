// FILE: src/rendering/Renderer.cpp
#include "Renderer.h"
#include "../scene/Scene.h"
#include "../simulation/ParticleSystem.h"
#include "../utils/Config.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <iostream>
#include <vector>

namespace t2c {

static const char* VERT_OBJ = R"(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
uniform mat4 u_Model,u_View,u_Proj,u_NormalMatrix;
out vec3 vWorldPos,vNormal;
void main(){
    vec4 w=u_Model*vec4(aPos,1.0);
    vWorldPos=w.xyz;
    vNormal=normalize(mat3(u_NormalMatrix)*aNormal);
    gl_Position=u_Proj*u_View*w;
})";

static const char* FRAG_OBJ = R"(
#version 330 core
in vec3 vWorldPos,vNormal;
out vec4 FragColor;
uniform vec3 u_AmbientColor,u_DirLightDir,u_DirLightColor,u_CamPos;
uniform vec4 u_ObjectColor,u_Emissive;
uniform float u_GlowStrength;
uniform int u_NumPointLights;
struct PLight{vec3 position,color;float radius;};
uniform PLight u_PointLights[8];
void main(){
    vec3 N=normalize(vNormal);
    vec3 V=normalize(u_CamPos-vWorldPos);
    float diff=max(dot(N,normalize(u_DirLightDir)),0.0);
    vec3 lighting=u_AmbientColor+diff*u_DirLightColor;
    vec3 refl=reflect(-normalize(u_DirLightDir),N);
    float spec=pow(max(dot(V,refl),0.0),32.0);
    lighting+=spec*0.5*u_DirLightColor;
    for(int i=0;i<u_NumPointLights;i++){
        float d=length(u_PointLights[i].position-vWorldPos);
        float att=1.0/(1.0+(d*d)/(u_PointLights[i].radius*u_PointLights[i].radius));
        vec3 Lp=normalize(u_PointLights[i].position-vWorldPos);
        lighting+=max(dot(N,Lp),0.0)*att*u_PointLights[i].color;
    }
    vec3 col=u_ObjectColor.rgb*lighting+u_Emissive.rgb*u_GlowStrength;
    col=col/(col+vec3(1.0));
    col=pow(col,vec3(1.0/2.2));
    FragColor=vec4(col,u_ObjectColor.a);
})";

static const char* VERT_PART = R"(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
layout(location=2) in float aSize;
uniform mat4 u_View,u_Proj;
out vec4 vColor;
void main(){
    vColor=aColor;
    gl_Position=u_Proj*u_View*vec4(aPos,1.0);
    gl_PointSize=aSize*(200.0/gl_Position.w);
})";

static const char* FRAG_PART = R"(
#version 330 core
in vec4 vColor;
out vec4 FragColor;
void main(){
    vec2 uv=gl_PointCoord*2.0-1.0;
    float d=dot(uv,uv);
    if(d>1.0)discard;
    float a=(1.0-d)*vColor.a;
    FragColor=vec4(vColor.rgb,a);
})";

static const char* VERT_QUAD = R"(
#version 330 core
layout(location=0) in vec3 aPos;
uniform mat4 u_Model,u_View,u_Proj;
out vec3 vWorldPos;
void main(){
    vec4 wp=u_Model*vec4(aPos,1.0);
    vWorldPos=wp.xyz;
    gl_Position=u_Proj*u_View*wp;
})";

static const char* FRAG_GROUND = R"(
#version 330 core
in vec3 vWorldPos;
out vec4 FragColor;
uniform vec4 u_ObjectColor;
uniform float u_Time;
uniform int u_IsOcean;
void main(){
    vec3 col=u_ObjectColor.rgb;
    if(u_IsOcean==1){
        float w=sin(vWorldPos.x*0.5+u_Time*1.5)*cos(vWorldPos.z*0.4+u_Time*1.2);
        col+=vec3(0.05,0.08,0.12)*w;
    }
    FragColor=vec4(col,u_ObjectColor.a);
})";

static const char* VERT_SKY = R"(
#version 330 core
layout(location=0) in vec3 aPos;
out float vHeight;
uniform mat4 u_Model,u_View,u_Proj;
void main(){
    vHeight=aPos.y*0.5+0.5;
    gl_Position=u_Proj*u_View*u_Model*vec4(aPos,1.0);
})";

static const char* FRAG_SKY = R"(
#version 330 core
in float vHeight;
out vec4 FragColor;
uniform vec3 u_SkyBottom,u_SkyTop;
void main(){
    FragColor=vec4(mix(u_SkyBottom,u_SkyTop,vHeight),1.0);
})";

static const char* VERT_STAR = R"(
#version 330 core
layout(location=0) in vec3 aPos;
uniform mat4 u_View,u_Proj;
void main(){
    gl_Position=u_Proj*u_View*vec4(aPos,1.0);
    gl_PointSize=2.0;
})";

static const char* FRAG_STAR = R"(
#version 330 core
out vec4 FragColor;
void main(){FragColor=vec4(1.0,1.0,1.0,0.9);})";

Renderer::Renderer()  = default;
Renderer::~Renderer() {
    if (m_sphereVAO) {
        glDeleteVertexArrays(1, &m_sphereVAO);
        glDeleteBuffers(1, &m_sphereVBO);
        glDeleteBuffers(1, &m_sphereEBO);
    }
    if (m_quadVAO) {
        glDeleteVertexArrays(1, &m_quadVAO);
        glDeleteBuffers(1, &m_quadVBO);
    }
    if (m_particleVAO) {
        glDeleteVertexArrays(1, &m_particleVAO);
        glDeleteBuffers(1, &m_particlePosVBO);
        glDeleteBuffers(1, &m_particleColVBO);
        glDeleteBuffers(1, &m_particleSzVBO);
    }
    if (m_starVAO) {
        glDeleteVertexArrays(1, &m_starVAO);
        glDeleteBuffers(1, &m_starVBO);
    }
}

bool Renderer::init(int width, int height) {
    m_width  = width;
    m_height = height;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_PROGRAM_POINT_SIZE);

    bool ok = true;
    ok &= m_objectShader  .loadFromSource(VERT_OBJ,  FRAG_OBJ);
    ok &= m_groundShader  .loadFromSource(VERT_QUAD, FRAG_GROUND);
    ok &= m_particleShader.loadFromSource(VERT_PART, FRAG_PART);
    ok &= m_skyShader     .loadFromSource(VERT_SKY,  FRAG_SKY);
    ok &= m_starShader    .loadFromSource(VERT_STAR, FRAG_STAR);

    if (!ok) {
        std::cerr << "[Renderer] Shader compile failed!\n";
        return false;
    }

    buildSphereMesh();
    buildQuadMesh();
    buildParticleVBO();

    std::cout << "[Renderer] Initialised (" << width << "x" << height << ")\n";
    return true;
}

void Renderer::resize(int w, int h) {
    m_width = w;
    m_height = h;
    glViewport(0, 0, w, h);
}

void Renderer::renderFrame(const Scene& scene,
                            const Camera& camera,
                            const Environment& env,
                            float time)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.02f, 1.0f);

    if (!m_starsBuilt && env.settings().starsEnabled) {
        buildStarVBO(env.starPositions());
        m_starsBuilt = true;
    }

    m_lighting.update(scene, time);
    const EnvironmentSettings& es = env.settings();
    m_lighting.setAmbient(es.ambientColor);
    m_lighting.setDirectional(es.sunDirection, es.sunColor, es.sunIntensity);

    renderSky      (env, camera);
    renderStars    (env, camera);
    renderGround   (scene, camera);
    renderObjects  (scene, camera);
    renderParticles(scene, camera);
}

void Renderer::renderSky(const Environment& env, const Camera& cam) {
    const EnvironmentSettings& es = env.settings();
    glDepthMask(GL_FALSE);
    m_skyShader.bind();
    m_skyShader.setMat4("u_Proj",  cam.projectionMatrix(ASPECT_RATIO));
    m_skyShader.setMat4("u_View",  cam.viewMatrix());
    m_skyShader.setMat4("u_Model", glm::scale(glm::mat4(1.0f), glm::vec3(900.0f)));
    m_skyShader.setVec3("u_SkyBottom", es.skyColorBottom);
    m_skyShader.setVec3("u_SkyTop",    es.skyColorTop);
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    m_skyShader.unbind();
    glDepthMask(GL_TRUE);
}

void Renderer::renderStars(const Environment& env, const Camera& cam) {
    if (!env.settings().starsEnabled || m_starVAO == 0) return;
    glDepthMask(GL_FALSE);
    m_starShader.bind();
    m_starShader.setMat4("u_Proj", cam.projectionMatrix(ASPECT_RATIO));
    m_starShader.setMat4("u_View", cam.viewMatrix());
    glBindVertexArray(m_starVAO);
    glDrawArrays(GL_POINTS, 0, m_starCount);
    glBindVertexArray(0);
    m_starShader.unbind();
    glDepthMask(GL_TRUE);
}

void Renderer::renderGround(const Scene& scene, const Camera& cam) {
    const SceneObject* ground = scene.findObject("ground");
    if (!ground) return;
    m_groundShader.bind();
    m_groundShader.setMat4("u_Proj",        cam.projectionMatrix(ASPECT_RATIO));
    m_groundShader.setMat4("u_View",        cam.viewMatrix());
    m_groundShader.setMat4("u_Model",       ground->modelMatrix());
    m_groundShader.setVec4("u_ObjectColor", ground->material().color);
    m_groundShader.setFloat("u_Time",       scene.time());
    m_groundShader.setInt("u_IsOcean",
        scene.environment().ground == "ocean" ? 1 : 0);
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    m_groundShader.unbind();
}

void Renderer::renderObjects(const Scene& scene, const Camera& cam) {
    m_objectShader.bind();
    m_objectShader.setMat4("u_View",    cam.viewMatrix());
    m_objectShader.setMat4("u_Proj",    cam.projectionMatrix(ASPECT_RATIO));
    m_objectShader.setVec3("u_CamPos",  cam.position());
    m_lighting.applyToShader(m_objectShader);

    for (const auto& obj : scene.objects()) {
        if (!obj->alive()) continue;
        if (obj->type() == ObjectType::Plane)       continue;
        if (obj->type() == ObjectType::LightSource) continue;

        glm::mat4 model = obj->modelMatrix();
        glm::mat4 nm    = glm::mat4(glm::transpose(glm::inverse(glm::mat3(model))));

        m_objectShader.setMat4 ("u_Model",       model);
        m_objectShader.setMat4 ("u_NormalMatrix", nm);
        m_objectShader.setVec4 ("u_ObjectColor",  obj->material().color);
        m_objectShader.setVec4 ("u_Emissive",     obj->material().emissive);
        m_objectShader.setFloat("u_GlowStrength", obj->material().glowStrength);

        glBindVertexArray(m_sphereVAO);
        glDrawElements(GL_TRIANGLES, m_sphereIndexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
    m_objectShader.unbind();
}

void Renderer::renderParticles(const Scene& scene, const Camera& cam) {
    if (scene.particleSystems().empty()) return;

    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    m_particleShader.bind();
    m_particleShader.setMat4("u_View", cam.viewMatrix());
    m_particleShader.setMat4("u_Proj", cam.projectionMatrix(ASPECT_RATIO));

    glBindVertexArray(m_particleVAO);

    for (const auto& ps : scene.particleSystems()) {
        int n = ps->liveCount();
        if (n == 0) continue;

        glBindBuffer(GL_ARRAY_BUFFER, m_particlePosVBO);
        glBufferData(GL_ARRAY_BUFFER,
            ps->positionBuffer().size() * sizeof(float),
            ps->positionBuffer().data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_particleColVBO);
        glBufferData(GL_ARRAY_BUFFER,
            ps->colorBuffer().size() * sizeof(float),
            ps->colorBuffer().data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, m_particleSzVBO);
        glBufferData(GL_ARRAY_BUFFER,
            ps->sizeBuffer().size() * sizeof(float),
            ps->sizeBuffer().data(), GL_DYNAMIC_DRAW);

        glDrawArrays(GL_POINTS, 0, n);
    }

    glBindVertexArray(0);
    m_particleShader.unbind();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);
}

void Renderer::buildSphereMesh() {
    constexpr int RINGS = 24, SLICES = 24;
    std::vector<float>        verts;
    std::vector<unsigned int> idx;

    for (int r = 0; r <= RINGS; ++r) {
        float phi = glm::pi<float>() * r / RINGS;
        for (int s = 0; s <= SLICES; ++s) {
            float theta = 2.0f * glm::pi<float>() * s / SLICES;
            float x = std::sin(phi) * std::cos(theta);
            float y = std::cos(phi);
            float z = std::sin(phi) * std::sin(theta);
            verts.insert(verts.end(), {x, y, z, x, y, z});
        }
    }
    for (int r = 0; r < RINGS; ++r)
        for (int s = 0; s < SLICES; ++s) {
            unsigned c = r*(SLICES+1)+s;
            unsigned n = c+SLICES+1;
            idx.insert(idx.end(), {c,n,c+1,n,n+1,c+1});
        }

    m_sphereIndexCount = static_cast<int>(idx.size());

    glGenVertexArrays(1, &m_sphereVAO);
    glGenBuffers(1, &m_sphereVBO);
    glGenBuffers(1, &m_sphereEBO);
    glBindVertexArray(m_sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphereVBO);
    glBufferData(GL_ARRAY_BUFFER,
        verts.size()*sizeof(float), verts.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        idx.size()*sizeof(unsigned int), idx.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Renderer::buildQuadMesh() {
    float v[] = {
        -1,0,-1,  1,0,-1,  1,0,1,
        -1,0,-1,  1,0, 1, -1,0,1
    };
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Renderer::buildParticleVBO() {
    glGenVertexArrays(1, &m_particleVAO);
    glGenBuffers(1, &m_particlePosVBO);
    glGenBuffers(1, &m_particleColVBO);
    glGenBuffers(1, &m_particleSzVBO);
    glBindVertexArray(m_particleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_particlePosVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleColVBO);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_particleSzVBO);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Renderer::buildStarVBO(const std::vector<float>& positions) {
    m_starCount = static_cast<int>(positions.size()) / 3;
    if (m_starCount == 0) return;
    glGenVertexArrays(1, &m_starVAO);
    glGenBuffers(1, &m_starVBO);
    glBindVertexArray(m_starVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_starVBO);
    glBufferData(GL_ARRAY_BUFFER,
        positions.size()*sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

} // namespace t2c