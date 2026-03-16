// FILE: src/simulation/PhysicsEngine.cpp
#include "PhysicsEngine.h"
#include "../utils/Config.h"

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

namespace t2c {

static btVector3 glmToBtV(const glm::vec3& v) {
    return btVector3(v.x, v.y, v.z);
}

static btQuaternion glmToBtQ(const glm::quat& q) {
    return btQuaternion(q.x, q.y, q.z, q.w);
}

static glm::vec3 btToGlmV(const btVector3& v) {
    return glm::vec3(v.x(), v.y(), v.z());
}

static glm::quat btToGlmQ(const btQuaternion& q) {
    return glm::quat(q.w(), q.x(), q.y(), q.z());
}

PhysicsEngine::PhysicsEngine()  = default;
PhysicsEngine::~PhysicsEngine() { clear(); }

bool PhysicsEngine::init() {
    m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
    m_dispatcher      = std::make_unique<btCollisionDispatcher>(
                            m_collisionConfig.get());
    m_broadphase      = std::make_unique<btDbvtBroadphase>();
    m_solver          = std::make_unique<btSequentialImpulseConstraintSolver>();
    m_world           = std::make_unique<btDiscreteDynamicsWorld>(
                            m_dispatcher.get(),
                            m_broadphase.get(),
                            m_solver.get(),
                            m_collisionConfig.get());

    m_world->setGravity(btVector3(0.0f, GRAVITY, 0.0f));
    std::cout << "[PhysicsEngine] Initialised (gravity=" << GRAVITY << ")\n";
    return true;
}

void PhysicsEngine::attachScene(Scene& scene) {
    for (auto& obj : scene.objects()) {
        if (obj->physicsBody() != nullptr) continue;

        btRigidBody* body = nullptr;

        switch (obj->type()) {
            case ObjectType::Plane:
                body = createPlaneBody();
                break;
            case ObjectType::Meteor:
            case ObjectType::Comet:
            case ObjectType::Asteroid:
            case ObjectType::Rock:
            case ObjectType::Sphere:
            case ObjectType::Spacecraft:
            case ObjectType::Aircraft:
            {
                float radius = obj->scale().x * 0.5f;
                float mass   = 4.0f / 3.0f * 3.14159f *
                               radius * radius * radius * 3000.0f;
                body = createSphereBody(radius, mass, obj->position());
                body->setLinearVelocity(glmToBtV(obj->velocity()));
                break;
            }
            default:
                continue;
        }

        if (body) {
            m_world->addRigidBody(body);
            obj->setPhysicsBody(body);
            m_bodies.push_back(body);
        }
    }
}

void PhysicsEngine::stepSimulation(float dt) {
    m_world->stepSimulation(dt, PHYSICS_SUB_STEPS, FIXED_TIME_STEP);
}

void PhysicsEngine::syncTransforms(Scene& scene) {
    for (auto& obj : scene.objects()) {
        auto* body = static_cast<btRigidBody*>(obj->physicsBody());
        if (!body || !obj->alive()) continue;

        btTransform trans;
        body->getMotionState()->getWorldTransform(trans);
        obj->setPosition(btToGlmV(trans.getOrigin()));
        obj->setRotation(btToGlmQ(trans.getRotation()));
        obj->setVelocity(btToGlmV(body->getLinearVelocity()));
    }
}

void PhysicsEngine::clear() {
    if (m_world) {
        for (auto* b : m_bodies) {
            m_world->removeRigidBody(b);
            delete b->getMotionState();
            delete b;
        }
    }
    m_bodies.clear();
    for (auto* s : m_shapes) delete s;
    m_shapes.clear();
}

btRigidBody* PhysicsEngine::createSphereBody(float radius,
                                              float mass,
                                              const glm::vec3& pos)
{
    auto* shape = new btSphereShape(radius);
    m_shapes.push_back(shape);

    btVector3 inertia(0, 0, 0);
    if (mass > 0.0f)
        shape->calculateLocalInertia(mass, inertia);

    btTransform start;
    start.setIdentity();
    start.setOrigin(glmToBtV(pos));

    auto* ms = new btDefaultMotionState(start);
    btRigidBody::btRigidBodyConstructionInfo ci(mass, ms, shape, inertia);
    ci.m_restitution = 0.1f;
    ci.m_friction    = 0.8f;
    return new btRigidBody(ci);
}

btRigidBody* PhysicsEngine::createPlaneBody() {
    auto* shape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
    m_shapes.push_back(shape);

    btTransform start;
    start.setIdentity();
    auto* ms = new btDefaultMotionState(start);
    btRigidBody::btRigidBodyConstructionInfo ci(0.0f, ms, shape);
    ci.m_restitution = 0.05f;
    ci.m_friction    = 0.9f;
    return new btRigidBody(ci);
}

} // namespace t2c