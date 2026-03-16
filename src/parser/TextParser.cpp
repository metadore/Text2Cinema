// FILE: src/parser/TextParser.cpp
#include "TextParser.h"
#include "../utils/Helpers.h"
#include "../utils/Config.h"

#include <iostream>
#include <algorithm>

namespace t2c {

const std::vector<TextParser::KWObject> TextParser::OBJECT_KEYWORDS = {
    { "meteor",      "meteor",     "glowing",   1.2f },
    { "meteorite",   "meteor",     "rocky",     1.2f },
    { "comet",       "comet",      "icy",       1.5f },
    { "asteroid",    "asteroid",   "rocky",     2.0f },
    { "fireball",    "fireball",   "fiery",     1.0f },
    { "ball",        "sphere",     "solid",     1.0f },
    { "sphere",      "sphere",     "solid",     1.0f },
    { "rock",        "rock",       "rocky",     1.5f },
    { "boulder",     "rock",       "rocky",     2.5f },
    { "ship",        "spacecraft", "metallic",  3.0f },
    { "spaceship",   "spacecraft", "metallic",  3.0f },
    { "ufo",         "spacecraft", "glowing",   2.5f },
    { "plane",       "aircraft",   "metallic",  4.0f },
    { "aircraft",    "aircraft",   "metallic",  4.0f },
};

const std::vector<TextParser::KWEffect> TextParser::EFFECT_KEYWORDS = {
    { "explode",     "explosion",  "impact"  },
    { "explosion",   "explosion",  "impact"  },
    { "blast",       "explosion",  "impact"  },
    { "spark",       "sparks",     "impact"  },
    { "sparks",      "sparks",     "impact"  },
    { "fire",        "fire",       "always"  },
    { "flame",       "fire",       "always"  },
    { "burning",     "fire",       "always"  },
    { "smoke",       "smoke",      "always"  },
    { "splash",      "splash",     "impact"  },
    { "splatter",    "splash",     "impact"  },
    { "rain",        "rain",       "always"  },
    { "glow",        "glow",       "always"  },
    { "glowing",     "glow",       "always"  },
    { "shockwave",   "shockwave",  "impact"  },
    { "crash",       "explosion",  "impact"  },
    { "impact",      "shockwave",  "impact"  },
};

SceneDescriptor TextParser::parse(const std::string& text) const {
    SceneDescriptor desc;
    desc.rawText = text;
    std::string lower = utils::toLower(text);

    extractObjects    (lower, desc);
    extractEnvironment(lower, desc);
    extractEvents     (lower, desc);
    extractMotion     (lower, desc);

    if (utils::containsCI(lower, "crash")   ||
        utils::containsCI(lower, "explode") ||
        utils::containsCI(lower, "blast")   ||
        utils::containsCI(lower, "impact")  ||
        utils::containsCI(lower, "massive"))
    {
        desc.hasCameraShake = true;
    }

    if (desc.objects.empty()) {
        ParsedObject obj;
        obj.type     = "sphere";
        obj.material = "solid";
        obj.scale    = 1.0f;
        desc.objects.push_back(obj);
    }

    if (t2c::VERBOSE_PARSER) dump(desc);
    return desc;
}

void TextParser::extractObjects(const std::string& lower,
                                 SceneDescriptor& out) const
{
    for (const auto& kw : OBJECT_KEYWORDS) {
        if (utils::containsCI(lower, kw.keyword)) {
            bool already = false;
            for (const auto& o : out.objects)
                if (o.type == kw.type) { already = true; break; }
            if (!already) {
                ParsedObject obj;
                obj.type     = kw.type;
                obj.material = kw.defaultMaterial;
                obj.scale    = kw.defaultScale;
                if (utils::containsCI(lower, "glowing"))  obj.material = "glowing";
                if (utils::containsCI(lower, "fiery"))    obj.material = "fiery";
                if (utils::containsCI(lower, "giant") ||
                    utils::containsCI(lower, "massive"))  obj.scale *= 2.0f;
                if (utils::containsCI(lower, "tiny")  ||
                    utils::containsCI(lower, "small")) obj.scale *= 0.5f;
                out.objects.push_back(obj);
            }
        }
    }
}

void TextParser::extractEnvironment(const std::string& lower,
                                     SceneDescriptor& out) const
{
    if      (utils::containsCI(lower, "night"))  out.environment.sky = "night";
    else if (utils::containsCI(lower, "stormy")) out.environment.sky = "stormy";
    else if (utils::containsCI(lower, "sunset")) out.environment.sky = "sunset";
    else if (utils::containsCI(lower, "space"))  out.environment.sky = "space";
    else                                          out.environment.sky = "night";

    if      (utils::containsCI(lower, "ocean") ||
             utils::containsCI(lower, "sea")   ||
             utils::containsCI(lower, "water"))    out.environment.ground = "ocean";
    else if (utils::containsCI(lower, "desert"))   out.environment.ground = "desert";
    else if (utils::containsCI(lower, "space"))    out.environment.ground = "void";
    else                                            out.environment.ground = "terrain";

    out.environment.hasFog  = utils::containsCI(lower, "fog") ||
                              utils::containsCI(lower, "mist");
    out.environment.hasRain = utils::containsCI(lower, "rain") ||
                              utils::containsCI(lower, "storm");
}

void TextParser::extractEvents(const std::string& lower,
                                SceneDescriptor& out) const
{
    for (const auto& kw : EFFECT_KEYWORDS) {
        if (utils::containsCI(lower, kw.keyword)) {
            bool already = false;
            for (const auto& e : out.events)
                if (e.effect == kw.effect) { already = true; break; }
            if (!already) {
                ParsedEvent ev;
                ev.effect  = kw.effect;
                ev.trigger = kw.trigger;
                ev.time    = 0.0f;
                out.events.push_back(ev);
            }
        }
    }

    bool hasImpact = false;
    for (const auto& e : out.events)
        if (e.trigger == "impact") { hasImpact = true; break; }
    if (!hasImpact)
        out.events.push_back({"always", "glow", 0.0f});
}

void TextParser::extractMotion(const std::string& lower,
                                SceneDescriptor& out) const
{
    if      (utils::containsCI(lower, "fall")  ||
             utils::containsCI(lower, "crash") ||
             utils::containsCI(lower, "plummet")) {
        out.motion.verb      = "fall";
        out.motion.direction = "down";
    }
    else if (utils::containsCI(lower, "fly")  ||
             utils::containsCI(lower, "soar") ||
             utils::containsCI(lower, "streak")) {
        out.motion.verb      = "fly";
        out.motion.direction = "across";
    }
    else if (utils::containsCI(lower, "rise") ||
             utils::containsCI(lower, "ascend")) {
        out.motion.verb      = "rise";
        out.motion.direction = "up";
    }
    else {
        out.motion.verb      = "fall";
        out.motion.direction = "down";
    }
}

void TextParser::dump(const SceneDescriptor& d) {
    std::cout << "\n===== TextParser Output =====\n";
    std::cout << "Input  : \"" << d.rawText << "\"\n";
    std::cout << "Motion : " << d.motion.verb << " " << d.motion.direction << "\n";
    std::cout << "Sky    : " << d.environment.sky
              << "  Ground: " << d.environment.ground << "\n";
    std::cout << "Objects (" << d.objects.size() << "):\n";
    for (const auto& o : d.objects)
        std::cout << "  - " << o.type << " ["
                  << o.material << "] scale=" << o.scale << "\n";
    std::cout << "Events (" << d.events.size() << "):\n";
    for (const auto& e : d.events)
        std::cout << "  - " << e.effect << " on=" << e.trigger << "\n";
    std::cout << "Camera shake: "
              << (d.hasCameraShake ? "yes" : "no") << "\n";
    std::cout << "=============================\n\n";
}

} // namespace t2c