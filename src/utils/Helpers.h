#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <random>

namespace t2c {
namespace utils {

float randFloat(float min, float max);
int   randInt(int min, int max);
glm::vec3 randUnitVec3();
glm::vec3 randHemisphere(const glm::vec3& normal);
glm::vec4 randColor(float alphaMin = 0.6f, float alphaMax = 1.0f);

glm::vec3 hsvToRgb(float h, float s, float v);
glm::vec4 lerpColor(const glm::vec4& a, const glm::vec4& b, float t);

float smoothstep(float edge0, float edge1, float x);

template<typename T>
T clamp(T x, T lo, T hi) { return (x < lo) ? lo : (x > hi) ? hi : x; }

float deg2rad(float degrees);
float rad2deg(float radians);

std::string toLower(std::string s);
std::vector<std::string> split(const std::string& s, char delimiter);
bool containsCI(const std::string& haystack, const std::string& needle);

bool ensureDirectory(const std::string& path);
std::string readFile(const std::string& path);

} // namespace utils
} // namespace t2c