#include "Shader.h"
#include "../utils/Helpers.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdexcept>

namespace t2c {

Shader::~Shader() {
    if (m_programID) glDeleteProgram(m_programID);
}

bool Shader::loadFromSource(const std::string& vertSrc,
                             const std::string& fragSrc)
{
    unsigned int vert = compileShader(GL_VERTEX_SHADER,   vertSrc);
    unsigned int frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!vert || !frag) return false;

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vert);
    glAttachShader(m_programID, frag);
    glLinkProgram (m_programID);

    int success = 0;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, log);
        std::cerr << "[Shader] Link error:\n" << log << "\n";
        glDeleteProgram(m_programID);
        m_programID = 0;
        glDeleteShader(vert);
        glDeleteShader(frag);
        return false;
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    return true;
}

bool Shader::loadFromFiles(const std::string& vertPath,
                            const std::string& fragPath)
{
    try {
        std::string vs = utils::readFile(vertPath);
        std::string fs = utils::readFile(fragPath);
        return loadFromSource(vs, fs);
    } catch (const std::exception& e) {
        std::cerr << "[Shader] Cannot load files: " << e.what() << "\n";
        return false;
    }
}

void Shader::bind()   const { glUseProgram(m_programID); }
void Shader::unbind() const { glUseProgram(0); }

int Shader::getUniformLocation(const std::string& name) const {
    return glGetUniformLocation(m_programID, name.c_str());
}

void Shader::setInt  (const std::string& n, int v)            const { glUniform1i (getUniformLocation(n), v); }
void Shader::setFloat(const std::string& n, float v)          const { glUniform1f (getUniformLocation(n), v); }
void Shader::setVec2 (const std::string& n, const glm::vec2& v)const { glUniform2fv(getUniformLocation(n),1,glm::value_ptr(v)); }
void Shader::setVec3 (const std::string& n, const glm::vec3& v)const { glUniform3fv(getUniformLocation(n),1,glm::value_ptr(v)); }
void Shader::setVec4 (const std::string& n, const glm::vec4& v)const { glUniform4fv(getUniformLocation(n),1,glm::value_ptr(v)); }
void Shader::setMat4 (const std::string& n, const glm::mat4& v)const { glUniformMatrix4fv(getUniformLocation(n),1,GL_FALSE,glm::value_ptr(v)); }

unsigned int Shader::compileShader(unsigned int type,
                                    const std::string& src)
{
    unsigned int id = glCreateShader(type);
    const char*  c  = src.c_str();
    glShaderSource(id, 1, &c, nullptr);
    glCompileShader(id);

    int success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        std::cerr << "[Shader] Compile error ("
                  << (type == GL_VERTEX_SHADER ? "vert" : "frag")
                  << "):\n" << log << "\n";
        glDeleteShader(id);
        return 0;
    }
    return id;
}

} // namespace t2c