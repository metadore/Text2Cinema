#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace t2c {

class FrameRecorder {
public:
    FrameRecorder()  = default;
    ~FrameRecorder();

    bool start(const std::string& outputDir, int width, int height);
    bool captureFrame();
    void stop();

    int  frameCount()  const { return m_frameCount; }
    bool isRecording() const { return m_recording; }

private:
    bool savePNG(const std::string& path,
                 const uint8_t* pixels,
                 int width, int height);

    std::string          m_outputDir;
    int                  m_width      = 0;
    int                  m_height     = 0;
    int                  m_frameCount = 0;
    bool                 m_recording  = false;
    std::vector<uint8_t> m_pixelBuf;
};

} // namespace t2c