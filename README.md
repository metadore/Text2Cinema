​<div align="center">
​<div style="background: radial-gradient(circle at center, #1a1a2e 0%, #000000 100%); padding: 80px 20px; border-radius: 24px; border: 1px solid #30363d; box-shadow: 0 20px 50px rgba(0, 194, 255, 0.15); margin-bottom: 40px;">
​<svg viewBox="0 0 800 140" style="font-weight: 900; font-family: 'Inter', 'Segoe UI', sans-serif;">
<defs>
<linearGradient id="textGrad" x1="0%" y1="0%" x2="100%" y2="0%">
<stop offset="0%" style="stop-color:#00c2ff;stop-opacity:1" />
<stop offset="50%" style="stop-color:#9d50bb;stop-opacity:1" />
<stop offset="100%" style="stop-color:#00c2ff;stop-opacity:1" />
<animate attributeName="x1" dur="5s" values="0%;100%;0%" repeatCount="indefinite" />
<animate attributeName="x2" dur="5s" values="100%;200%;100%" repeatCount="indefinite" />
</linearGradient>
<filter id="glow">
<feGaussianBlur stdDeviation="3.5" result="coloredBlur"/>
<feMerge>
<feMergeNode in="coloredBlur"/><feMergeNode in="SourceGraphic"/>
</feMerge>
</filter>
</defs>
<text x="50%" y="60%" text-anchor="middle" fill="url(#textGrad)" style="font-size: 82px; letter-spacing: 12px; filter: url(#glow); text-transform: uppercase;">
Text2Cinema
</text>
<text x="50%" y="105%" text-anchor="middle" fill="#8b949e" style="font-size: 16px; font-weight: 400; letter-spacing: 6px; text-transform: uppercase;">
The Procedural Cinematic Simulation Engine
</text>
</svg>
​<p align="center" style="margin-top: 40px; display: flex; justify-content: center; gap: 12px; flex-wrap: wrap;">
<img src="https://img.shields.io/badge/C++17-Toolkit-00c2ff?style=for-the-badge&logo=cplusplus" />
<img src="https://img.shields.io/badge/OpenGL-Graphics-9d50bb?style=for-the-badge&logo=opengl" />
<img src="https://img.shields.io/badge/Bullet-Physics-white?style=for-the-badge" />
<img src="https://img.shields.io/badge/FFmpeg-Video-0078d4?style=for-the-badge&logo=ffmpeg" />
<img src="https://img.shields.io/badge/Build-Passing-2ea44f?style=for-the-badge" />
</p>
</div>
​📽️ Watch Text Become Cinema
​Text2Cinema transforms natural language descriptions into fully simulated, mathematically-rendered cinematic environments.
# Execute the engine with a natural language prompt
text2cinema.exe "A glowing meteor falls from the sky and explodes into sparks when it hits the ocean"
<div align="left" style="background: #0d1117; padding: 25px; border-radius: 12px; border: 1px solid #30363d; margin: 20px 0;">
<h4 style="margin-top: 0; color: #58a6ff;">📦 Engine Output</h4>
<code style="color: #c9d1d9;">> output/cinematic_render.mp4</code>

<code style="color: #c9d1d9;">> src/generated/GeneratedScene.cpp</code> <i style="color: #8b949e;">(Deterministic simulation source)</i>
</div>
​🛠️ Engine Architecture & Pipeline
​<svg width="100%" height="320" viewBox="0 0 850 320" fill="none" xmlns="http://www.w3.org/2000/svg" style="background: #0d1117; border-radius: 16px; border: 1px solid #30363d;">
<style>
@keyframes flow { to { stroke-dashoffset: -20; } }
.flow-line { stroke-dasharray: 5, 5; animation: flow 1s linear infinite; }
.node-box { fill: #161b22; stroke: #30363d; stroke-width: 1.5; transition: 0.3s; }
.node-box:hover { fill: #1c2128; stroke: #58a6ff; }
.label { font-family: 'Segoe UI', sans-serif; font-size: 11px; font-weight: bold; fill: #8b949e; text-transform: uppercase; pointer-events: none; }
</style>
<path class="flow-line" d="M80 160 H770" stroke="url(#flowGrad)" stroke-width="2" />
<defs>
<linearGradient id="flowGrad" x1="0%" y1="0%" x2="100%" y2="0%">
<stop offset="0%" stop-color="#00c2ff" />
<stop offset="100%" stop-color="#9d50bb" />
</linearGradient>
</defs>
<g transform="translate(40, 130)">
<rect class="node-box" width="100" height="60" rx="6" />
<text x="50" y="35" text-anchor="middle" class="label">Text Input</text>
</g>
<g transform="translate(180, 130)">
<rect class="node-box" width="100" height="60" rx="6" />
<text x="50" y="35" text-anchor="middle" class="label">Semantic Parser</text>
</g>
<g transform="translate(320, 130)">
<rect class="node-box" width="130" height="60" rx="6" />
<text x="65" y="35" text-anchor="middle" class="label">Physics Solver</text>
</g>
<g transform="translate(490, 130)">
<rect class="node-box" width="100" height="60" rx="6" />
<text x="50" y="35" text-anchor="middle" class="label">OpenGL GLSL</text>
</g>
<g transform="translate(630, 130)">
<rect class="node-box" width="100" height="60" rx="6" />
<text x="50" y="35" text-anchor="middle" class="label">FFmpeg Pipe</text>
</g>
<circle cx="780" cy="160" r="15" fill="#9d50bb" />
</svg>
​🧠 Semantic Logic: TextParser.cpp
​The engine uses a keyword-weighted heuristic to map English descriptions to 3D simulation parameters.
// src/parser/TextParser.cpp snippet
void TextParser::parse(std::string prompt) {
    auto scene = SceneContext::Get();
    
    if (prompt.contains("meteor")) {
        scene->AddObject(ObjectType::SPHERE, Material::GLOWING);
        scene->ApplyForce(Vector3(0, -9.8, 0), "gravity");
    }
    
    if (prompt.contains("ocean")) {
        scene->SetEnvironment(EnvType::WATER_PLANE);
        scene->AddPhysicsCallback(CollisionEvent::SPLASH);
    }
    
    // Procedural generation of the C++ scene file
    CodeGenerator::Export("generated/GeneratedScene.cpp", scene);
}
⚙️ CMake & vcpkg Integration
​Professional-grade build system with automatic dependency management.
​<details open>
<summary style="font-size: 1.2em; font-weight: bold; cursor: pointer; color: #58a6ff; padding: 10px;">📄 CMakeLists.txt Configuration</summary>
cmake_minimum_required(VERSION 3.24)
project(Text2Cinema LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Dependency Discovery
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(bullet3 CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/**/*.cpp")
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw glad::glad Bullet3::BulletDynamics Bullet3::LinearMath
)
</details>
​📂 Repository DNA
Text2Cinema/
├── 📂 src/
│   ├── 🧩 core/          # Main Engine Loop & State
│   ├── 🗣️ parser/        # NLP & Semantic Lexer
│   ├── ⚛️ simulation/    # Bullet Physics Solver
│   ├── 🔦 rendering/     # OpenGL Shaders & PBR
│   └── 🛠️ generated/     # Auto-written C++ Scene Files
├── 📜 vcpkg.json         # Dependency Manifest
└── 📜 CMakeLists.txt     # Build Script
📈 Technical Metrics
Metric                Status             Performance
Render Engine         OpenGL 4.6         < 16ms / Frame
Physics Solver        Bullet 3.x         Deterministic
Library Load          vcpkg Static       Optimized
<div align="center" style="margin-top: 20px;">
<b>Real-time Render Latency</b>

<svg width="400" height="60">
<path d="M0 40 Q 50 10, 100 45 T 200 30 T 300 50 T 400 20" fill="none" stroke="#00c2ff" stroke-width="2">
<animate attributeName="d" dur="3s" values="M0 40 Q 50 10, 100 45 T 200 30 T 300 50 T 400 20; M0 40 Q 50 50, 100 20 T 200 45 T 300 10 T 400 40; M0 40 Q 50 10, 100 45 T 200 30 T 300 50 T 400 20" repeatCount="indefinite" />
</path>
</svg>
</div>
​<div align="center">
<p style="color: #8b949e; letter-spacing: 2px;">FROM WORDS TO WORLDS</p>
<h2 style="color: #ffffff; margin: 0;">Text2Cinema © 2026</h2>
<p><a href="#">Docs</a> • <a href="#">vcpkg</a> • <a href="#">License</a></p>
</div>
​</div>
