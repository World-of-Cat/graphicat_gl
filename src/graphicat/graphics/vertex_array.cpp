#include "vertex_array.hpp"
#include "shader.hpp"

namespace gc {

    VertexArray::VertexArray(unsigned int handle, bool owned) : handle(handle), owned(owned) {
    }

    std::unique_ptr<VertexArray> VertexArray::create() {
        unsigned int h;
        glCreateVertexArrays(1, &h);
        return wrap(h, true);
    }

    std::unique_ptr<VertexArray> VertexArray::wrap(unsigned int handle, bool take_ownership) {
        return std::unique_ptr<VertexArray>(new VertexArray(handle, take_ownership));
    }


    std::shared_ptr<VertexArray> VertexArray::create_shared() {
        unsigned int h;
        glCreateVertexArrays(1, &h);
        return wrap_shared(h, true);
    }

    std::shared_ptr<VertexArray> VertexArray::wrap_shared(unsigned int handle, bool take_ownership) {
        return std::shared_ptr<VertexArray>(new VertexArray(handle, take_ownership));
    }

    void VertexArray::bind() const {
        glBindVertexArray(handle);
    }

    void VertexArray::bind(const std::shared_ptr<Shader> &shader) const {
        bind();
        for (const auto& name : attribute_names) {
            shader->bind_attrib_location(name.first, name.second);
        }
    }

    void VertexArray::bind(const std::unique_ptr<Shader> &shader) const {
        bind();
        for (const auto& name : attribute_names) {
            shader->bind_attrib_location(name.first, name.second);
        }
    }

    void VertexArray::bind(const Shader *shader) const {
        bind();
        for (const auto& name : attribute_names) {
            shader->bind_attrib_location(name.first, name.second);
        }
    }

    void VertexArray::vertex_buffer(const std::shared_ptr<Buffer> &buffer, const std::vector<std::pair<size_t,std::string>> &attributes) {
        vertex_buffer(buffer->get_handle(), attributes);
    }

    void
    VertexArray::vertex_buffer(const std::shared_ptr<Buffer> &buffer, const std::vector<VertexAttribute> &attributes,
                               size_t stride, size_t offset) {
        vertex_buffer(buffer->get_handle(), attributes, stride);
    }

    void VertexArray::vertex_buffer(const std::unique_ptr<Buffer> &buffer, const std::vector<std::pair<size_t,std::string>> &attributes) {
        vertex_buffer(buffer->get_handle(), attributes);
    }

    void
    VertexArray::vertex_buffer(const std::unique_ptr<Buffer> &buffer, const std::vector<VertexAttribute> &attributes,
                               size_t stride, size_t offset) {
        vertex_buffer(buffer->get_handle(), attributes, stride);
    }

    void VertexArray::vertex_buffer(const Buffer *buffer, const std::vector<std::pair<size_t,std::string>> &attributes) {
        vertex_buffer(buffer->get_handle(), attributes);
    }

    void
    VertexArray::vertex_buffer(const Buffer *buffer, const std::vector<VertexAttribute> &attributes, size_t stride, size_t offset) {
        vertex_buffer(buffer->get_handle(), attributes, stride);
    }

    void VertexArray::vertex_buffer(unsigned int buffer, const std::vector<std::pair<size_t,std::string>> &attributes) {
        int stride = 0;

        for (const auto& pair : attributes) {
            glVertexArrayAttribBinding(handle, next_attribute, next_binding);
            glVertexArrayAttribFormat(handle, next_attribute, static_cast<int>(pair.first), GL_FLOAT, false, stride);
            glEnableVertexArrayAttrib(handle, next_attribute);
            attribute_names[pair.second] = next_attribute++;

            stride += static_cast<int>(pair.first * sizeof(float));
        }

        glVertexArrayVertexBuffer(handle, next_binding++, buffer, 0, stride);
    }

    void
    VertexArray::vertex_buffer(unsigned int buffer, const std::vector<VertexAttribute> &attributes, size_t stride, size_t offset) {
        for (const auto& attrib : attributes) {
            glVertexArrayAttribBinding(handle, next_attribute, next_binding);
            glVertexArrayAttribFormat(handle, next_attribute, static_cast<int>(attrib.size), GL_FLOAT, false, attrib.offset);
            glEnableVertexArrayAttrib(handle, next_attribute);
            attribute_names[attrib.name] = next_attribute++;

            stride += static_cast<int>(attrib.size * sizeof(float));
        }

        glVertexArrayVertexBuffer(handle, next_binding++, buffer, static_cast<int>(offset), static_cast<int>(stride));
    }

    VertexArray::~VertexArray() {
        if (owned) glDeleteVertexArrays(1, &handle);
    }
} // gc