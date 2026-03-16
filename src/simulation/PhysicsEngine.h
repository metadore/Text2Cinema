#pragma once

#include "../scene/Scene.h"
#include <memory>
#include <vector>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btCollisionShape;
class btRigidBody;

namespace t2c {

class PhysicsEngine {
public:
    PhysicsEngine();
    ~PhysicsEngine();

    bool init();
    void attachScene(Scene& scene);
    void stepSimulation(float dt);
    void syncTransforms(Scene& scene);
    void clear();

private:
    btRigidBody* createSphereBody(float radius, float mass,
                                   const glm::vec3& initialPos);
    btRigidBody* createPlaneBody();

    std::unique_ptr<btDefaultCollisionConfiguration>     m_collisionConfig;
    std::unique_ptr<btCollisionDispatcher>               m_dispatcher;
    std::unique_ptr<btBroadphaseInterface>               m_broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld>             m_world;

    std::vector<btCollisionShape*> m_shapes;
    std::vector<btRigidBody*>      m_bodies;
};

} // namespace t2c