#pragma once

#include "graphicat/graphicat.hpp"
#include "graphicat/graphics/buffer.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace gc {

    class Shader;

    struct VertexAttribute {
        size_t size;
        size_t offset;
        std::string name;
    };

    class VertexArray {
        unsigned int handle;
        bool owned;

        unsigned int next_binding = 0;
        unsigned int next_attribute = 0;

        std::unordered_map<std::string, unsigned int> attribute_names;

        VertexArray(unsigned int handle, bool owned);

    public:

        virtual ~VertexArray();

        static std::unique_ptr<VertexArray> create();
        static std::unique_ptr<VertexArray> wrap(unsigned int handle, bool take_ownership=true);

        static std::shared_ptr<VertexArray> create_shared();
        static std::shared_ptr<VertexArray> wrap_shared(unsigned int handle, bool take_ownership=true);

        void bind() const;

        void bind(const std::shared_ptr<Shader>& shader) const;
        void bind(const std::unique_ptr<Shader>& shader) const;
        void bind(const Shader* shader) const;

        void vertex_buffer(const std::shared_ptr<Buffer>& buffer, const std::vector<std::pair<size_t,std::string>>& attributes);
        void vertex_buffer(const std::shared_ptr<Buffer>& buffer, const std::vector<VertexAttribute>& attributes, size_t stride, size_t offset = 0);

        void vertex_buffer(const std::unique_ptr<Buffer>& buffer, const std::vector<std::pair<size_t,std::string>>& attributes);
        void vertex_buffer(const std::unique_ptr<Buffer>& buffer, const std::vector<VertexAttribute>& attributes, size_t stride, size_t offset = 0);

        void vertex_buffer(const Buffer* buffer, const std::vector<std::pair<size_t,std::string>>& attributes);
        void vertex_buffer(const Buffer* buffer, const std::vector<VertexAttribute>& attributes, size_t stride, size_t offset = 0);

        void vertex_buffer(unsigned int buffer, const std::vector<std::pair<size_t,std::string>>& attributes);
        void vertex_buffer(unsigned int buffer, const std::vector<VertexAttribute>& attributes, size_t stride, size_t offset = 0);

    };

} // gc
