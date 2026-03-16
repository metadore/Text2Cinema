#include "Helpers.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>

#ifdef _WIN32
#  include <direct.h>
#  define MKDIR(p) _mkdir(p)
#else
#  include <sys/stat.h>
#  include <sys/types.h>
#  define MKDIR(p) mkdir((p), 0755)
#endif

namespace t2c {
namespace utils {

static thread_local std::mt19937 rng{ std::random_device{}() };

float randFloat(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

int randInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

glm::vec3 randUnitVec3() {
    float theta = randFloat(0.0f, 2.0f * 3.14159265f);
    float z     = randFloat(-1.0f, 1.0f);
    float r     = std::sqrt(1.0f - z * z);
    return { r * std::cos(theta), r * std::sin(theta), z };
}

glm::vec3 randHemisphere(const glm::vec3& normal) {
    glm::vec3 v = randUnitVec3();
    return (glm::dot(v, normal) < 0.0f) ? -v : v;
}

glm::vec4 randColor(float alphaMin, float alphaMax) {
    float h       = randFloat(0.0f, 1.0f);
    float s       = randFloat(0.5f, 1.0f);
    float v       = randFloat(0.7f, 1.0f);
    glm::vec3 rgb = hsvToRgb(h, s, v);
    return { rgb.r, rgb.g, rgb.b, randFloat(alphaMin, alphaMax) };
}

glm::vec3 hsvToRgb(float h, float s, float v) {
    float c  = v * s;
    float x  = c * (1.0f - std::fabs(std::fmod(h * 6.0f, 2.0f) - 1.0f));
    float m  = v - c;
    glm::vec3 rgb(0.0f);
    int hi = static_cast<int>(h * 6.0f) % 6;
    switch (hi) {
        case 0: rgb = {c, x, 0}; break;
        case 1: rgb = {x, c, 0}; break;
        case 2: rgb = {0, c, x}; break;
        case 3: rgb = {0, x, c}; break;
        case 4: rgb = {x, 0, c}; break;
        default: rgb = {c, 0, x}; break;
    }
    return rgb + glm::vec3(m);
}

glm::vec4 lerpColor(const glm::vec4& a, const glm::vec4& b, float t) {
    return a + (b - a) * t;
}

float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

float deg2rad(float degrees) { return degrees * (3.14159265f / 180.0f); }
float rad2deg(float radians) { return radians * (180.0f / 3.14159265f); }

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return s;
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(s);
    std::string token;
    while (std::getline(iss, token, delimiter))
        if (!token.empty()) tokens.push_back(token);
    return tokens;
}

bool containsCI(const std::string& haystack, const std::string& needle) {
    return toLower(haystack).find(toLower(needle)) != std::string::npos;
}

bool ensureDirectory(const std::string& path) {
    std::string current;
    for (char c : path) {
        current += c;
        if (c == '/' || c == '\\')
            MKDIR(current.c_str());
    }
    MKDIR(path.c_str());
    return true;
}

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + path);
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

} // namespace utils
} // namespace t2c