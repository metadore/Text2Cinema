# FILE: CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(Text2Cinema VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)
find_package(glfw3  REQUIRED)
find_package(glm    REQUIRED)
find_package(Bullet REQUIRED)

set(FFMPEG_LIB_DIR "C:/vcpkg/installed/x64-windows/lib")
set(FFMPEG_INC_DIR "C:/vcpkg/installed/x64-windows/include")

set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/third_party/glad/src/glad.c")

set(SOURCES
    src/main.cpp
    src/core/Engine.cpp
    src/parser/TextParser.cpp
    src/scene/Scene.cpp
    src/scene/SceneObject.cpp
    src/simulation/PhysicsEngine.cpp
    src/simulation/ParticleSystem.cpp
    src/simulation/Environment.cpp
    src/rendering/Renderer.cpp
    src/rendering/Camera.cpp
    src/rendering/Lighting.cpp
    src/rendering/Shader.cpp
    src/export/FrameRecorder.cpp
    src/export/VideoExporter.cpp
    src/generated/GeneratedScene.cpp
    src/utils/Helpers.cpp
)

add_executable(text2cinema ${SOURCES} ${GLAD_SOURCE})

set_source_files_properties(${GLAD_SOURCE} PROPERTIES LANGUAGE C)

target_include_directories(text2cinema PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${OPENGL_INCLUDE_DIR}
    ${BULLET_INCLUDE_DIRS}
    ${FFMPEG_INC_DIR}
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
    ${CMAKE_SOURCE_DIR}/third_party/nlohmann
    ${CMAKE_SOURCE_DIR}/third_party/stb
)

target_link_libraries(text2cinema PRIVATE
    ${OPENGL_LIBRARIES}
    glfw
    glm::glm
    ${BULLET_LIBRARIES}
    ${FFMPEG_LIB_DIR}/avcodec.lib
    ${FFMPEG_LIB_DIR}/avformat.lib
    ${FFMPEG_LIB_DIR}/avutil.lib
    ${FFMPEG_LIB_DIR}/swscale.lib
)

target_compile_definitions(text2cinema PRIVATE USE_GLAD)

add_custom_command(TARGET text2cinema POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/generated/frames
)

message(STATUS "Text2Cinema configured successfully")