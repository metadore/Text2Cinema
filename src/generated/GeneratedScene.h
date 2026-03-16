#pragma once

#include "../parser/TextParser.h"
#include <string>

namespace t2c {

class Scene;

class GeneratedScene {
public:
    GeneratedScene() = default;

    void buildScene(Scene& scene, const SceneDescriptor& desc);
    bool saveCode(const std::string& outputPath,
                  const SceneDescriptor& desc) const;

    const std::string& generatedCode() const { return m_code; }

private:
    void generateCodeString(const SceneDescriptor& desc);
    static std::string capitalise(const std::string& s);

    std::string m_code;
};

} // namespace t2c