#pragma once

#include <cstdint>
#include <string>

namespace t2c {

constexpr int   SCREEN_WIDTH      = 1280;
constexpr int   SCREEN_HEIGHT     = 720;
constexpr float ASPECT_RATIO      = static_cast<float>(SCREEN_WIDTH) /
                                    static_cast<float>(SCREEN_HEIGHT);
constexpr bool  FULLSCREEN        = false;
constexpr char  WINDOW_TITLE[]    = "Text2Cinema Engine";

constexpr int   TARGET_FPS        = 30;
constexpr float FRAME_DT          = 1.0f / TARGET_FPS;
constexpr int   MSAA_SAMPLES      = 4;

constexpr float GRAVITY           = -9.81f;
constexpr int   PHYSICS_SUB_STEPS = 3;
constexpr float FIXED_TIME_STEP   = 1.0f / 60.0f;

constexpr float SIMULATION_DURATION = 6.0f;
constexpr float WORLD_SCALE         = 10.0f;

constexpr int   MAX_PARTICLES       = 50000;
constexpr float PARTICLE_POINT_SIZE = 4.0f;

constexpr float CAM_FOV            = 60.0f;
constexpr float CAM_NEAR           = 0.1f;
constexpr float CAM_FAR            = 1000.0f;

constexpr char  OUTPUT_FRAME_DIR[] = "generated/frames";
constexpr char  OUTPUT_VIDEO_PATH[]= "generated/output_video.mp4";
constexpr char  OUTPUT_CODE_PATH[] = "generated/GeneratedScene.cpp";
constexpr int   VIDEO_BITRATE_KBPS = 4000;
constexpr char  VIDEO_CODEC[]      = "libx264";

constexpr bool  SHOW_IMGUI_OVERLAY  = false;
constexpr bool  SHOW_PHYSICS_DEBUG  = false;
constexpr bool  VERBOSE_PARSER      = true;

} // namespace t2c