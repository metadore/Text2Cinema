#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace t2c {

class Shader {
public:
    Shader()  = default;
    ~Shader();

    bool loadFromSource(const std::string& vertSrc,
                        const std::string& fragSrc);

    bool loadFromFiles(const std::string& vertPath,
                       const std::string& fragPath);

    void bind()   const;
    void unbind() const;

    unsigned int id() const { return m_programID; }

    void setInt  (const std::string& name, int v)               const;
    void setFloat(const std::string& name, float v)             const;
    void setVec2 (const std::string& name, const glm::vec2& v)  const;
    void setVec3 (const std::string& name, const glm::vec3& v)  const;
    void setVec4 (const std::string& name, const glm::vec4& v)  const;
    void setMat4 (const std::string& name, const glm::mat4& v)  const;

private:
    unsigned int compileShader(unsigned int type,
                                const std::string& src);
    int getUniformLocation(const std::string& name) const;

    unsigned int m_programID = 0;
};

} // namespace t2c