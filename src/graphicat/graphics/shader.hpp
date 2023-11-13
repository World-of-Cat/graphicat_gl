#pragma once

#include "graphicat/graphicat.hpp"
#include <memory>
#include <string>
#include <filesystem>
#include <vector>
#include <glm/glm.hpp>

namespace gc {

    enum class ShaderType {
        Vertex = GL_VERTEX_SHADER,
        Fragment = GL_FRAGMENT_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        TessControl = GL_TESS_CONTROL_SHADER,
        TessEval = GL_TESS_EVALUATION_SHADER,
        Compute = GL_COMPUTE_SHADER,
    };

    struct ShaderSource {
        ShaderType type;
        std::string source;
    };

    class Shader {
        unsigned int handle;
        bool owned;

        Shader(unsigned int handle, bool owned);

    public:
        virtual ~Shader();

        static std::unique_ptr<Shader> wrap(unsigned int handle, bool take_ownership = true);
        static std::unique_ptr<Shader> create(const std::vector<ShaderSource>& sources);
        static std::unique_ptr<Shader> load(const std::vector<std::pair<ShaderType, std::filesystem::path>>& sources);

        static std::shared_ptr<Shader> wrap_shared(unsigned int handle, bool take_ownership = true);
        static std::shared_ptr<Shader> create_shared(const std::vector<ShaderSource>& sources);
        static std::shared_ptr<Shader> load_shared(const std::vector<std::pair<ShaderType, std::filesystem::path>>& sources);

        void bind() const;

        int get_uniform_location(const std::string& name) const;

        void uniform_1f(const std::string& name, const float &x) const;
        void uniform_1f(int location, const float &x) const;
        void uniform_1f(const std::string& name, const glm::vec1 &v) const;
        void uniform_1f(int location, const glm::vec1 &v) const;

        void uniform_2f(const std::string& name, const float &x, const float &y) const;
        void uniform_2f(int location, const float &x, const float &y) const;
        void uniform_2f(const std::string& name, const glm::vec2 &v) const;
        void uniform_2f(int location, const glm::vec2 &v) const;

        void uniform_3f(const std::string& name, const float &x, const float &y, const float &z) const;
        void uniform_3f(int location, const float &x, const float &y, const float &z) const;
        void uniform_3f(const std::string& name, const glm::vec3 &v) const;
        void uniform_3f(int location, const glm::vec3 &v) const;

        void uniform_4f(const std::string& name, const float &x, const float &y, const float &z, const float &w) const;
        void uniform_4f(int location, const float &x, const float &y, const float &z, const float &w) const;
        void uniform_4f(const std::string& name, const glm::vec4 &v) const;
        void uniform_4f(int location, const glm::vec4 &v) const;

        void uniform_1i(const std::string& name, const int &x) const;
        void uniform_1i(int location, const int &x) const;
        void uniform_1i(const std::string& name, const glm::ivec1 &v) const;
        void uniform_1i(int location, const glm::ivec1 &v) const;

        void uniform_2i(const std::string& name, const int &x, const int &y) const;
        void uniform_2i(int location, const int &x, const int &y) const;
        void uniform_2i(const std::string& name, const glm::ivec2 &v) const;
        void uniform_2i(int location, const glm::ivec2 &v) const;

        void uniform_3i(const std::string& name, const int &x, const int &y, const int &z) const;
        void uniform_3i(int location, const int &x, const int &y, const int &z) const;
        void uniform_3i(const std::string& name, const glm::ivec3 &v) const;
        void uniform_3i(int location, const glm::ivec3 &v) const;

        void uniform_4i(const std::string& name, const int &x, const int &y, const int &z, const int &w) const;
        void uniform_4i(int location, const int &x, const int &y, const int &z, const int &w) const;
        void uniform_4i(const std::string& name, const glm::ivec4 &v) const;
        void uniform_4i(int location, const glm::ivec4 &v) const;

        void uniform_1ui(const std::string& name, const unsigned int &x) const;
        void uniform_1ui(int location, const unsigned int &x) const;
        void uniform_1ui(const std::string& name, const glm::uvec1 &v) const;
        void uniform_1ui(int location, const glm::uvec1 &v) const;

        void uniform_2ui(const std::string& name, const unsigned int &x, const unsigned int &y) const;
        void uniform_2ui(int location, const unsigned int &x, const unsigned int &y) const;
        void uniform_2ui(const std::string& name, const glm::uvec2 &v) const;
        void uniform_2ui(int location, const glm::uvec2 &v) const;

        void uniform_3ui(const std::string& name, const unsigned int &x, const unsigned int &y, const unsigned int &z) const;
        void uniform_3ui(int location, const unsigned int &x, const unsigned int &y, const unsigned int &z) const;
        void uniform_3ui(const std::string& name, const glm::uvec3 &v) const;
        void uniform_3ui(int location, const glm::uvec3 &v) const;

        void uniform_4ui(const std::string& name, const unsigned int &x, const unsigned int &y, const unsigned int &z, const unsigned int &w) const;
        void uniform_4ui(int location, const unsigned int &x, const unsigned int &y, const unsigned int &z, const unsigned int &w) const;
        void uniform_4ui(const std::string& name, const glm::uvec4 &v) const;
        void uniform_4ui(int location, const glm::uvec4 &v) const;

        void uniform_1d(const std::string& name, const double &x) const;
        void uniform_1d(int location, const double &x) const;
        void uniform_1d(const std::string& name, const glm::dvec1 &v) const;
        void uniform_1d(int location, const glm::dvec1 &v) const;

        void uniform_2d(const std::string& name, const double &x, const double &y) const;
        void uniform_2d(int location, const double &x, const double &y) const;
        void uniform_2d(const std::string& name, const glm::dvec2 &v) const;
        void uniform_2d(int location, const glm::dvec2 &v) const;

        void uniform_3d(const std::string& name, const double &x, const double &y, const double &z) const;
        void uniform_3d(int location, const double &x, const double &y, const double &z) const;
        void uniform_3d(const std::string& name, const glm::dvec3 &v) const;
        void uniform_3d(int location, const glm::dvec3 &v) const;

        void uniform_4d(const std::string& name, const double &x, const double &y, const double &z, const double &w) const;
        void uniform_4d(int location, const double &x, const double &y, const double &z, const double &w) const;
        void uniform_4d(const std::string& name, const glm::dvec4 &v) const;
        void uniform_4d(int location, const glm::dvec4 &v) const;

        void uniform_mat2f(const std::string& name, const glm::mat2 &m) const;
        void uniform_mat2f(int location, const glm::mat2 &m) const;
        void uniform_mat2x3f(const std::string& name, const glm::mat2x3 &m) const;
        void uniform_mat2x3f(int location, const glm::mat2x3 &m) const;
        void uniform_mat2x4f(const std::string& name, const glm::mat2x4 &m) const;
        void uniform_mat2x4f(int location, const glm::mat2x4 &m) const;
        void uniform_mat3f(const std::string& name, const glm::mat3 &m) const;
        void uniform_mat3f(int location, const glm::mat3 &m) const;
        void uniform_mat3x2f(const std::string& name, const glm::mat3x2 &m) const;
        void uniform_mat3x2f(int location, const glm::mat3x2 &m) const;
        void uniform_mat3x4f(const std::string& name, const glm::mat3x4 &m) const;
        void uniform_mat3x4f(int location, const glm::mat3x4 &m) const;
        void uniform_mat4f(const std::string& name, const glm::mat4 &m) const;
        void uniform_mat4f(int location, const glm::mat4 &m) const;
        void uniform_mat4x2f(const std::string& name, const glm::mat4x2 &m) const;
        void uniform_mat4x2f(int location, const glm::mat4x2 &m) const;
        void uniform_mat4x3f(const std::string& name, const glm::mat4x3 &m) const;
        void uniform_mat4x3f(int location, const glm::mat4x3 &m) const;

        void uniform_mat2d(const std::string& name, const glm::dmat2 &m) const;
        void uniform_mat2d(int location, const glm::dmat2 &m) const;
        void uniform_mat2x3d(const std::string& name, const glm::dmat2x3 &m) const;
        void uniform_mat2x3d(int location, const glm::dmat2x3 &m) const;
        void uniform_mat2x4d(const std::string& name, const glm::dmat2x4 &m) const;
        void uniform_mat2x4d(int location, const glm::dmat2x4 &m) const;
        void uniform_mat3d(const std::string& name, const glm::dmat3 &m) const;
        void uniform_mat3d(int location, const glm::dmat3 &m) const;
        void uniform_mat3x2d(const std::string& name, const glm::dmat3x2 &m) const;
        void uniform_mat3x2d(int location, const glm::dmat3x2 &m) const;
        void uniform_mat3x4d(const std::string& name, const glm::dmat3x4 &m) const;
        void uniform_mat3x4d(int location, const glm::dmat3x4 &m) const;
        void uniform_mat4d(const std::string& name, const glm::dmat4 &m) const;
        void uniform_mat4d(int location, const glm::dmat4 &m) const;
        void uniform_mat4x2d(const std::string& name, const glm::dmat4x2 &m) const;
        void uniform_mat4x2d(int location, const glm::dmat4x2 &m) const;
        void uniform_mat4x3d(const std::string& name, const glm::dmat4x3 &m) const;
        void uniform_mat4x3d(int location, const glm::dmat4x3 &m) const;

        void bind_attrib_location(const std::string& name, int location) const;
    };

} // gc
