// FILE: src/core/Engine.cpp
#define GLFW_INCLUDE_NONE

#include "Engine.h"
#include "../simulation/PhysicsEngine.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../utils/Config.h"
#include "../utils/Helpers.h"

#include <iostream>

namespace t2c {

Engine* Engine::s_instance = nullptr;

Engine::Engine()  { s_instance = this; }

Engine::~Engine() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
    s_instance = nullptr;
}

bool Engine::run(const std::string& textDescription) {
    std::cout << "\n=== Text2Cinema Engine v1.0 ===\n\n";

    std::cout << "Step 1/6  Parsing...\n";
    m_descriptor = m_parser.parse(textDescription);

    std::cout << "Step 2/6  Building environment...\n";
    m_environment.configure(
        m_descriptor.environment.sky,
        m_descriptor.environment.ground,
        m_descriptor.environment.hasFog,
        m_descriptor.environment.hasRain);

    std::cout << "Step 3/6  Initialising GPU...\n";
    if (!initWindow())    return false;
    if (!initRendering()) return false;

    std::cout << "Step 4/6  Building scene...\n";
    m_codeGen.buildScene(m_scene, m_descriptor);
    initPhysics();

    if (m_descriptor.hasCameraShake) {
        m_scene.registerEventCallback(
            [this](const std::string& type, SceneObject*) {
                if (type == "impact") {
                    m_renderer.camera().triggerShake(0.4f, 1.2f);
                    m_renderer.camera().zoomTo(50.0f, 0.3f);
                }
            });
    }

    SceneObject* primary = nullptr;
    for (const auto& o : m_scene.objects()) {
        if (o->type() != ObjectType::Plane) {
            primary = o.get();
            break;
        }
    }
    if (primary) {
        m_renderer.camera().trackObject(primary);
        m_renderer.camera().setMode(CameraMode::Tracking);
    }

    std::cout << "Step 5/6  Rendering "
              << SIMULATION_DURATION << "s...\n";
    utils::ensureDirectory(OUTPUT_FRAME_DIR);
    simulateAndRecord();

    std::cout << "Step 6/6  Exporting video...\n";
    exportVideo();
    saveGeneratedCode();

    std::cout << "\nDone!\n"
              << "  Video : " << OUTPUT_VIDEO_PATH << "\n"
              << "  Code  : " << OUTPUT_CODE_PATH  << "\n\n";
    return true;
}

bool Engine::initWindow() {
    if (!glfwInit()) {
        std::cerr << "[Engine] GLFW init failed\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES,  MSAA_SAMPLES);
    glfwWindowHint(GLFW_VISIBLE,  GLFW_FALSE);

    m_window = glfwCreateWindow(m_width, m_height,
                                 WINDOW_TITLE, nullptr, nullptr);
    if (!m_window) {
        std::cerr << "[Engine] Window creation failed\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCB);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[Engine] GLAD init failed\n";
        return false;
    }

    std::cout << "  OpenGL: " << glGetString(GL_VERSION)
              << "  GPU: "    << glGetString(GL_RENDERER) << "\n";
    return true;
}

bool Engine::initRendering() {
    if (!m_renderer.init(m_width, m_height)) return false;
    m_recorder.start(OUTPUT_FRAME_DIR, m_width, m_height);
    return true;
}

bool Engine::initPhysics() {
    m_physics.init();
    m_physics.attachScene(m_scene);
    return true;
}

void Engine::simulateAndRecord() {
    const int totalFrames =
        static_cast<int>(SIMULATION_DURATION / FRAME_DT);

    for (int frame = 0;
         frame < totalFrames && m_scene.running();
         ++frame)
    {
        glfwPollEvents();

        m_physics.stepSimulation(FRAME_DT);
        m_physics.syncTransforms(m_scene);
        m_scene.update(FRAME_DT);
        m_renderer.camera().update(FRAME_DT);

        m_renderer.renderFrame(
            m_scene,
            m_renderer.camera(),
            m_environment,
            m_scene.time());

        glfwSwapBuffers(m_window);
        m_recorder.captureFrame();

        if (frame % TARGET_FPS == 0) {
            int pct = static_cast<int>(
                100.0f * frame / totalFrames);
            std::cout << "  Rendering: " << pct << "% ("
                      << frame << "/" << totalFrames
                      << ")\r" << std::flush;
        }
    }

    std::cout << "\n";
    m_recorder.stop();
}

void Engine::exportVideo() {
    utils::ensureDirectory("generated");
    bool ok = m_exporter.exportToMP4(
        OUTPUT_FRAME_DIR,
        "frame_%06d.png",
        OUTPUT_VIDEO_PATH,
        TARGET_FPS,
        VIDEO_BITRATE_KBPS);

    if (!ok) {
        std::cerr << "[Engine] Export failed: "
                  << m_exporter.lastError() << "\n";
    }
}

void Engine::saveGeneratedCode() {
    m_codeGen.saveCode(OUTPUT_CODE_PATH, m_descriptor);
}

void Engine::framebufferSizeCB(GLFWwindow*, int w, int h) {
    if (s_instance)
        s_instance->m_renderer.resize(w, h);
}

} // namespace t2c