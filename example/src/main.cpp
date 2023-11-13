#include <iostream>

#include <graphicat/graphicat.hpp>
#include <graphicat/os/window.hpp>

#include <glad/gl.h>
#include "graphicat/graphics/utils.hpp"
#include "graphicat/graphics/shader.hpp"
#include "graphicat/graphics/vertex_array.hpp"
#include "graphicat/graphics/buffer.hpp"

int main() {
    gc::GlobalState::init();

    gc::WindowProperties window_properties{};
    window_properties.window_mode = gc::wm::Windowed({800, 600});

    auto window = std::make_unique<gc::Window>(window_properties);

    std::string vsh = "#version 460 core\n"
                      "in vec3 posIn;"
                      "in vec2 uvIn;"
                      "in vec4 colorIn;"
                      "out vec2 fUV;"
                      "out vec4 fColor;"
                      "uniform mat4 uTransform;"
                      "void main() {"
                      "  gl_Position = uTransform * vec4(posIn, 1.0);"
                      "  fUV = uvIn;"
                      "  fColor = colorIn;"
                      "}";

    std::string fsh = "#version 460 core\n"
                      "in vec2 fUV;"
                      "in vec4 fColor;"
                      "out vec4 colorOut;"
                      "void main() {"
                      "  colorOut = fColor;"
                      "}";


    auto shader = gc::Shader::create({{gc::ShaderType::Vertex, vsh}, {gc::ShaderType::Fragment, fsh}});

    auto vao = gc::VertexArray::create();

    std::vector<float> vd = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    };

    auto vbo = gc::Buffer::load(vd, gc::BufferUsage::StaticDraw);

    vao->vertex_buffer(vbo, { { 3, "posIn" }, { 2, "uvIn" }, { 4, "colorIn" } });

    while (window->is_open()) {
        glfwPollEvents();

        gc::clear({1.0f, 0.0f, 0.0f});

        shader->bind();
        shader->uniform_mat4f("uTransform", glm::mat4(1.0f));

        vao->bind(shader);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        window->update();
    }

}
