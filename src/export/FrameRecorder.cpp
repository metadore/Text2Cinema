
#include "FrameRecorder.h"
#include "../utils/Helpers.h"

#include <glad/glad.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <algorithm>

namespace t2c {

FrameRecorder::~FrameRecorder() {
    if (m_recording) stop();
}

bool FrameRecorder::start(const std::string& outputDir, int width, int height) {
    m_outputDir  = outputDir;
    m_width      = width;
    m_height     = height;
    m_frameCount = 0;
    m_recording  = true;
    utils::ensureDirectory(outputDir);
    m_pixelBuf.resize(static_cast<size_t>(width * height * 3));
    std::cout << "[FrameRecorder] Recording " << width << "x" << height
              << " to " << outputDir << "\n";
    return true;
}

bool FrameRecorder::captureFrame() {
    if (!m_recording) return false;

    glReadPixels(0, 0, m_width, m_height,
                 GL_RGB, GL_UNSIGNED_BYTE, m_pixelBuf.data());

    int rowBytes = m_width * 3;
    std::vector<uint8_t> flipped(m_pixelBuf.size());
    for (int y = 0; y < m_height; ++y) {
        const uint8_t* src = m_pixelBuf.data() + (m_height-1-y)*rowBytes;
        uint8_t*       dst = flipped.data()    + y*rowBytes;
        std::memcpy(dst, src, rowBytes);
    }

    std::ostringstream ss;
    ss << m_outputDir << "/frame_"
       << std::setw(6) << std::setfill('0') << m_frameCount << ".png";

    bool ok = savePNG(ss.str(), flipped.data(), m_width, m_height);
    if (!ok) {
        std::cerr << "[FrameRecorder] Failed to save frame " << m_frameCount << "\n";
        return false;
    }
    ++m_frameCount;
    return true;
}

void FrameRecorder::stop() {
    m_recording = false;
    std::cout << "[FrameRecorder] Saved " << m_frameCount << " frames\n";
}

bool FrameRecorder::savePNG(const std::string& path,
                             const uint8_t* pixels,
                             int width, int height)
{
    return stbi_write_png(path.c_str(), width, height, 3,
                          pixels, width*3) != 0;
}

} // namespace t2c