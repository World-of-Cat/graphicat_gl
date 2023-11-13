#include "shader.hpp"
#include <fstream>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

namespace gc {
    Shader::Shader(unsigned int handle, bool owned) : handle(handle), owned(owned) {
    }

    Shader::~Shader() {
        if (owned) glDeleteProgram(handle);
    }

    static unsigned int create_shader_module(const ShaderSource& source) {
        unsigned int shader = glCreateShader(static_cast<GLenum>(source.type));
        const char* src = source.source.c_str();
        glShaderSource(shader, 1, &src, nullptr);

        glCompileShader(shader);

        int status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE) {
            int log_length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
            char* buf = new char[log_length];

            glGetShaderInfoLog(shader, log_length, &status, buf);
            spdlog::error("Failed to compile shader: {}", buf);

            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    static unsigned int create_shader(const std::vector<ShaderSource> &sources) {
        unsigned int program = glCreateProgram();
        std::vector<unsigned int> shaders;
        shaders.reserve(sources.size());
        for (const auto& source : sources) {
            unsigned int module = create_shader_module(source);

            if (module == 0) {
                spdlog::error("Shader module failed to compile; not proceeding to link program.");

                for (const auto& module_ : shaders) {
                    glDeleteShader(module_);
                }

                return 0; // definitely not a program handle
            }

            shaders.push_back(module);

            glAttachShader(program, module);
        }

        glLinkProgram(program);
        int status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) {
            int log_length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
            char* buf = new char[log_length];

            glGetProgramInfoLog(program, log_length, &status, buf);

            spdlog::error("Failed to link shader: {}", buf);

            for (const auto& module : shaders) {
                glDeleteShader(module);
            }

            return 0; // definitely not a program handle
        }

        for (const auto& module : shaders) {
            glDeleteShader(module);
        }

        return program;
    }

    static std::string read_file(const std::filesystem::path& path) {
        std::ifstream f(path, std::ios::ate | std::ios::in);

        std::streampos end = f.tellg();
        std::string buf;
        buf.resize(end);
        f.seekg(0);
        f.read(buf.data(), end);
        return buf;
    }

    static unsigned int load_shader(const std::vector<std::pair<ShaderType, std::filesystem::path>> &sources) {
        std::vector<ShaderSource> loaded_sources;
        loaded_sources.reserve(sources.size());
        for (const auto& source : sources) {
            loaded_sources.push_back(ShaderSource{source.first, read_file(source.second)});
        }

        return create_shader(loaded_sources);
    }

    std::unique_ptr<Shader> Shader::wrap(unsigned int handle, bool take_ownership) {
        return std::unique_ptr<Shader>(new Shader(handle, take_ownership));
    }

    std::unique_ptr<Shader> Shader::create(const std::vector<ShaderSource> &sources) {
        return wrap(create_shader(sources), true);
    }

    std::unique_ptr<Shader> Shader::load(const std::vector<std::pair<ShaderType, std::filesystem::path>> &sources) {
        return wrap(load_shader(sources), true);
    }

    std::shared_ptr<Shader> Shader::wrap_shared(unsigned int handle, bool take_ownership) {
        return std::shared_ptr<Shader>(new Shader(handle, take_ownership));
    }

    std::shared_ptr<Shader> Shader::create_shared(const std::vector<ShaderSource> &sources) {
        return wrap_shared(create_shader(sources), true);
    }

    std::shared_ptr<Shader>
    Shader::load_shared(const std::vector<std::pair<ShaderType, std::filesystem::path>> &sources) {
        return wrap_shared(load_shader(sources), true);
    }

    void Shader::bind() const {
        glUseProgram(handle);
    }

    int Shader::get_uniform_location(const std::string &name) const {
        return glGetUniformLocation(handle, name.c_str());
    }

    void Shader::uniform_1f(const std::string& name, const float &x) const {
        uniform_1f(get_uniform_location(name), x);
    }

    void Shader::uniform_1f(int location, const float &x) const {
        glProgramUniform1f(handle, location, x);
    }

    void Shader::uniform_1f(const std::string& name, const glm::vec1 &v) const {
        uniform_1f(get_uniform_location(name), v);
    }

    void Shader::uniform_1f(int location, const glm::vec1 &v) const {
        uniform_1f(location, v.x);
    }

    void Shader::uniform_2f(const std::string& name, const float &x, const float &y) const {
        uniform_2f(get_uniform_location(name), x, y);
    }

    void Shader::uniform_2f(int location, const float &x, const float &y) const {
        glProgramUniform2f(handle, location, x, y);
    }

    void Shader::uniform_2f(const std::string& name, const glm::vec2 &v) const {
        uniform_2f(get_uniform_location(name), v);
    }

    void Shader::uniform_2f(int location, const glm::vec2 &v) const {
        uniform_2f(location, v.x, v.y);
    }

    void Shader::uniform_3f(const std::string& name, const float &x, const float &y, const float &z) const {
        uniform_3f(get_uniform_location(name), x, y, z);
    }

    void Shader::uniform_3f(int location, const float &x, const float &y, const float &z) const {
        glProgramUniform3f(handle, location, x, y, z);
    }

    void Shader::uniform_3f(const std::string& name, const glm::vec3 &v) const {
        uniform_3f(get_uniform_location(name), v);
    }

    void Shader::uniform_3f(int location, const glm::vec3 &v) const {
        uniform_3f(location, v.x, v.y, v.z);
    }

    void Shader::uniform_4f(const std::string& name, const float &x, const float &y, const float &z, const float &w) const {
        uniform_4f(get_uniform_location(name), x, y, z, w);
    }

    void Shader::uniform_4f(int location, const float &x, const float &y, const float &z, const float &w) const {
        glProgramUniform4f(handle, location, x, y, z, w);
    }

    void Shader::uniform_4f(const std::string& name, const glm::vec4 &v) const {
        uniform_4f(get_uniform_location(name), v);
    }

    void Shader::uniform_4f(int location, const glm::vec4 &v) const {
        uniform_4f(location, v.x, v.y, v.z, v.w);
    }

    void Shader::uniform_1i(const std::string& name, const int &x) const {
        uniform_1i(get_uniform_location(name), x);
    }

    void Shader::uniform_1i(int location, const int &x) const {
        glProgramUniform1i(handle, location, x);
    }

    void Shader::uniform_1i(const std::string& name, const glm::ivec1 &v) const {
        uniform_1i(get_uniform_location(name), v);
    }

    void Shader::uniform_1i(int location, const glm::ivec1 &v) const {
        uniform_1i(location, v.x);
    }

    void Shader::uniform_2i(const std::string& name, const int &x, const int &y) const {
        uniform_2i(get_uniform_location(name), x, y);
    }

    void Shader::uniform_2i(int location, const int &x, const int &y) const {
        glProgramUniform2i(handle, location, x, y);
    }

    void Shader::uniform_2i(const std::string& name, const glm::ivec2 &v) const {
        uniform_2i(get_uniform_location(name), v);
    }

    void Shader::uniform_2i(int location, const glm::ivec2 &v) const {
        uniform_2i(location, v.x, v.y);
    }

    void Shader::uniform_3i(const std::string& name, const int &x, const int &y, const int &z) const {
        uniform_3i(get_uniform_location(name), x, y, z);
    }

    void Shader::uniform_3i(int location, const int &x, const int &y, const int &z) const {
        glProgramUniform3i(handle, location, x, y, z);
    }

    void Shader::uniform_3i(const std::string& name, const glm::ivec3 &v) const {
        uniform_3i(get_uniform_location(name), v);
    }

    void Shader::uniform_3i(int location, const glm::ivec3 &v) const {
        uniform_3i(location, v.x, v.y, v.z);
    }

    void Shader::uniform_4i(const std::string& name, const int &x, const int &y, const int &z, const int &w) const {
        uniform_4i(get_uniform_location(name), x, y, z, w);
    }

    void Shader::uniform_4i(int location, const int &x, const int &y, const int &z, const int &w) const {
        glProgramUniform4i(handle, location, x, y, z, w);
    }

    void Shader::uniform_4i(const std::string& name, const glm::ivec4 &v) const {
        uniform_4i(get_uniform_location(name), v);
    }

    void Shader::uniform_4i(int location, const glm::ivec4 &v) const {
        uniform_4i(location, v.x, v.y, v.z, v.w);
    }

    void Shader::uniform_1ui(const std::string& name, const unsigned int &x) const {
        uniform_1ui(get_uniform_location(name), x);
    }

    void Shader::uniform_1ui(int location, const unsigned int &x) const {
        glProgramUniform1ui(handle, location, x);
    }

    void Shader::uniform_1ui(const std::string& name, const glm::uvec1 &v) const {
        uniform_1ui(get_uniform_location(name), v);
    }

    void Shader::uniform_1ui(int location, const glm::uvec1 &v) const {
        uniform_1ui(location, v.x);
    }

    void Shader::uniform_2ui(const std::string& name, const unsigned int &x, const unsigned int &y) const {
        uniform_2ui(get_uniform_location(name), x, y);
    }

    void Shader::uniform_2ui(int location, const unsigned int &x, const unsigned int &y) const {
        glProgramUniform2ui(handle, location, x, y);
    }

    void Shader::uniform_2ui(const std::string& name, const glm::uvec2 &v) const {
        uniform_2ui(get_uniform_location(name), v);
    }

    void Shader::uniform_2ui(int location, const glm::uvec2 &v) const {
        uniform_2ui(location, v.x, v.y);
    }

    void Shader::uniform_3ui(const std::string& name, const unsigned int &x, const unsigned int &y, const unsigned int &z) const {
        uniform_3ui(get_uniform_location(name), x, y, z);
    }

    void Shader::uniform_3ui(int location, const unsigned int &x, const unsigned int &y, const unsigned int &z) const {
        glProgramUniform3ui(handle, location, x, y, z);
    }

    void Shader::uniform_3ui(const std::string& name, const glm::uvec3 &v) const {
        uniform_3ui(get_uniform_location(name), v);
    }

    void Shader::uniform_3ui(int location, const glm::uvec3 &v) const {
        uniform_3ui(location, v.x, v.y, v.z);
    }

    void Shader::uniform_4ui(const std::string& name, const unsigned int &x, const unsigned int &y, const unsigned int &z, const unsigned int &w) const {
        uniform_4ui(get_uniform_location(name), x, y, z, w);
    }

    void Shader::uniform_4ui(int location, const unsigned int &x, const unsigned int &y, const unsigned int &z, const unsigned int &w) const {
        glProgramUniform4ui(handle, location, x, y, z, w);
    }

    void Shader::uniform_4ui(const std::string& name, const glm::uvec4 &v) const {
        uniform_4ui(get_uniform_location(name), v);
    }

    void Shader::uniform_4ui(int location, const glm::uvec4 &v) const {
        uniform_4ui(location, v.x, v.y, v.z, v.w);
    }

    void Shader::uniform_1d(const std::string& name, const double &x) const {
        uniform_1d(get_uniform_location(name), x);
    }

    void Shader::uniform_1d(int location, const double &x) const {
        glProgramUniform1d(handle, location, x);
    }

    void Shader::uniform_1d(const std::string& name, const glm::dvec1 &v) const {
        uniform_1d(get_uniform_location(name), v);
    }

    void Shader::uniform_1d(int location, const glm::dvec1 &v) const {
        uniform_1d(location, v.x);
    }

    void Shader::uniform_2d(const std::string& name, const double &x, const double &y) const {
        uniform_2d(get_uniform_location(name), x, y);
    }

    void Shader::uniform_2d(int location, const double &x, const double &y) const {
        glProgramUniform2d(handle, location, x, y);
    }

    void Shader::uniform_2d(const std::string& name, const glm::dvec2 &v) const {
        uniform_2d(get_uniform_location(name), v);
    }

    void Shader::uniform_2d(int location, const glm::dvec2 &v) const {
        uniform_2d(location, v.x, v.y);
    }

    void Shader::uniform_3d(const std::string& name, const double &x, const double &y, const double &z) const {
        uniform_3d(get_uniform_location(name), x, y, z);
    }

    void Shader::uniform_3d(int location, const double &x, const double &y, const double &z) const {
        glProgramUniform3d(handle, location, x, y, z);
    }

    void Shader::uniform_3d(const std::string& name, const glm::dvec3 &v) const {
        uniform_3d(get_uniform_location(name), v);
    }

    void Shader::uniform_3d(int location, const glm::dvec3 &v) const {
        uniform_3d(location, v.x, v.y, v.z);
    }

    void Shader::uniform_4d(const std::string& name, const double &x, const double &y, const double &z, const double &w) const {
        uniform_4d(get_uniform_location(name), x, y, z, w);
    }

    void Shader::uniform_4d(int location, const double &x, const double &y, const double &z, const double &w) const {
        glProgramUniform4d(handle, location, x, y, z, w);
    }

    void Shader::uniform_4d(const std::string& name, const glm::dvec4 &v) const {
        uniform_4d(get_uniform_location(name), v);
    }

    void Shader::uniform_4d(int location, const glm::dvec4 &v) const {
        uniform_4d(location, v.x, v.y, v.z, v.w);
    }

    void Shader::uniform_mat2f(const std::string& name, const glm::mat2 &m) const {
        uniform_mat2f(get_uniform_location(name), m);
    }

    void Shader::uniform_mat2f(int location, const glm::mat2 &m) const {
        glProgramUniformMatrix2fv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat2x3f(const std::string& name, const glm::mat2x3 &m) const {
        uniform_mat2x3f(get_uniform_location(name), m);
    }

    void Shader::uniform_mat2x3f(int location, const glm::mat2x3 &m) const {
        glProgramUniformMatrix2x3fv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat2x4f(const std::string& name, const glm::mat2x4 &m) const {
        uniform_mat2x4f(get_uniform_location(name), m);
    }

    void Shader::uniform_mat2x4f(int location, const glm::mat2x4 &m) const {
        glProgramUniformMatrix2x4fv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat3f(const std::string& name, const glm::mat3 &m) const {
        uniform_mat3f(get_uniform_location(name), m);
    }

    void Shader::uniform_mat3f(int location, const glm::mat3 &m) const {
        glProgramUniformMatrix3fv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat3x2f(const std::string& name, const glm::mat3x2 &m) const {
        uniform_mat3x2f(get_uniform_location(name), m);
    }

    void Shader::uniform_mat3x2f(int location, const glm::mat3x2 &m) const {
        glProgramUniformMatrix3x2fv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat3x4f(const std::string& name, const glm::mat3x4 &m) const {
        uniform_mat3x4f(get_uniform_location(name), m);
    }

    void Shader::uniform_mat3x4f(int location, const glm::mat3x4 &m) const {
        glProgramUniformMatrix3x4fv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat4f(const std::string& name, const glm::mat4 &m) const {
        uniform_mat4f(get_uniform_location(name), m);
    }

    void Shader::uniform_mat4f(int location, const glm::mat4 &m) const {
        glProgramUniformMatrix4fv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat4x2f(const std::string& name, const glm::mat4x2 &m) const {
        uniform_mat4x2f(get_uniform_location(name), m);
    }

    void Shader::uniform_mat4x2f(int location, const glm::mat4x2 &m) const {
        glProgramUniformMatrix4x2fv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat4x3f(const std::string& name, const glm::mat4x3 &m) const {
        uniform_mat4x3f(get_uniform_location(name), m);
    }

    void Shader::uniform_mat4x3f(int location, const glm::mat4x3 &m) const {
        glProgramUniformMatrix4x3fv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat2d(const std::string& name, const glm::dmat2 &m) const {
        uniform_mat2d(get_uniform_location(name), m);
    }

    void Shader::uniform_mat2d(int location, const glm::dmat2 &m) const {
        glProgramUniformMatrix2dv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat2x3d(const std::string& name, const glm::dmat2x3 &m) const {
        uniform_mat2x3d(get_uniform_location(name), m);
    }

    void Shader::uniform_mat2x3d(int location, const glm::dmat2x3 &m) const {
        glProgramUniformMatrix2x3dv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat2x4d(const std::string& name, const glm::dmat2x4 &m) const {
        uniform_mat2x4d(get_uniform_location(name), m);
    }

    void Shader::uniform_mat2x4d(int location, const glm::dmat2x4 &m) const {
        glProgramUniformMatrix2x4dv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat3d(const std::string& name, const glm::dmat3 &m) const {
        uniform_mat3d(get_uniform_location(name), m);
    }

    void Shader::uniform_mat3d(int location, const glm::dmat3 &m) const {
        glProgramUniformMatrix3dv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat3x2d(const std::string& name, const glm::dmat3x2 &m) const {
        uniform_mat3x2d(get_uniform_location(name), m);
    }

    void Shader::uniform_mat3x2d(int location, const glm::dmat3x2 &m) const {
        glProgramUniformMatrix3x2dv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat3x4d(const std::string& name, const glm::dmat3x4 &m) const {
        uniform_mat3x4d(get_uniform_location(name), m);
    }

    void Shader::uniform_mat3x4d(int location, const glm::dmat3x4 &m) const {
        glProgramUniformMatrix3x4dv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat4d(const std::string& name, const glm::dmat4 &m) const {
        uniform_mat4d(get_uniform_location(name), m);
    }

    void Shader::uniform_mat4d(int location, const glm::dmat4 &m) const {
        glProgramUniformMatrix4dv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat4x2d(const std::string& name, const glm::dmat4x2 &m) const {
        uniform_mat4x2d(get_uniform_location(name), m);
    }

    void Shader::uniform_mat4x2d(int location, const glm::dmat4x2 &m) const {
        glProgramUniformMatrix4x2dv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::uniform_mat4x3d(const std::string& name, const glm::dmat4x3 &m) const {
        uniform_mat4x3d(get_uniform_location(name), m);
    }

    void Shader::uniform_mat4x3d(int location, const glm::dmat4x3 &m) const {
        glProgramUniformMatrix4x3dv(handle, location, 1, false, glm::value_ptr(m));
    }

    void Shader::bind_attrib_location(const std::string &name, int location) const {
        glBindAttribLocation(handle, location, name.c_str());
    }
} // gc