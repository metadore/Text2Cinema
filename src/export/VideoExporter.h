#pragma once

#include <string>

namespace t2c {

class VideoExporter {
public:
    VideoExporter() = default;

    bool exportToMP4(const std::string& frameDir,
                     const std::string& framePattern,
                     const std::string& outputPath,
                     int fps = 30,
                     int bitrate_kbps = 4000);

    const std::string& lastError() const { return m_lastError; }

private:
    std::string m_lastError;
};

} // namespace t2c