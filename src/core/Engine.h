#pragma once

#include "../utils/Config.h"
#include "../parser/TextParser.h"
#include "../scene/Scene.h"
#include "../simulation/PhysicsEngine.h"
#include "../simulation/Environment.h"
#include "../rendering/Renderer.h"
#include "../export/FrameRecorder.h"
#include "../export/VideoExporter.h"
#include "../generated/GeneratedScene.h"

#include <string>

struct GLFWwindow;

namespace t2c {

class Engine {
public:
    Engine();
    ~Engine();

    bool run(const std::string& textDescription);

private:
    bool initWindow();
    bool initRendering();
    bool initPhysics();
    void simulateAndRecord();
    void exportVideo();
    void saveGeneratedCode();

    static void framebufferSizeCB(GLFWwindow* win, int w, int h);
    static Engine* s_instance;

    TextParser      m_parser;
    Scene           m_scene;
    PhysicsEngine   m_physics;
    Environment     m_environment;
    Renderer        m_renderer;
    FrameRecorder   m_recorder;
    VideoExporter   m_exporter;
    GeneratedScene  m_codeGen;

    GLFWwindow*     m_window  = nullptr;
    SceneDescriptor m_descriptor;
    int             m_width   = SCREEN_WIDTH;
    int             m_height  = SCREEN_HEIGHT;
};

} // namespace t2c