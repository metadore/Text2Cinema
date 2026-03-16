#pragma once

#include <string>
#include <vector>

namespace t2c {

struct ParsedObject {
    std::string type;
    std::string material;
    float       scale  = 1.0f;
};

struct ParsedEnvironment {
    std::string sky    = "night";
    std::string ground = "terrain";
    bool        hasFog  = false;
    bool        hasRain = false;
};

struct ParsedEvent {
    std::string trigger;
    std::string effect;
    float       time = 0.0f;
};

struct ParsedMotion {
    std::string verb      = "fall";
    std::string direction = "down";
};

struct SceneDescriptor {
    std::string                  rawText;
    std::vector<ParsedObject>    objects;
    ParsedEnvironment            environment;
    std::vector<ParsedEvent>     events;
    ParsedMotion                 motion;
    bool                         hasCameraShake = false;
};

class TextParser {
public:
    TextParser()  = default;
    ~TextParser() = default;

    SceneDescriptor parse(const std::string& text) const;
    static void dump(const SceneDescriptor& desc);

private:
    void extractObjects    (const std::string& lower, SceneDescriptor& out) const;
    void extractEnvironment(const std::string& lower, SceneDescriptor& out) const;
    void extractEvents     (const std::string& lower, SceneDescriptor& out) const;
    void extractMotion     (const std::string& lower, SceneDescriptor& out) const;

    struct KWObject {
        std::string keyword;
        std::string type;
        std::string defaultMaterial;
        float       defaultScale;
    };

    struct KWEffect {
        std::string keyword;
        std::string effect;
        std::string trigger;
    };

    static const std::vector<KWObject> OBJECT_KEYWORDS;
    static const std::vector<KWEffect> EFFECT_KEYWORDS;
};

} // namespace t2c