#pragma once

#include "graphicat/graphicat.hpp"
#include <memory>
#include <vector>

namespace gc {


    enum class BufferUsage : GLenum {
        StaticDraw = GL_STATIC_DRAW,
        StaticRead = GL_STATIC_READ,
        StaticCopy = GL_STATIC_COPY,

        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicRead = GL_DYNAMIC_READ,
        DynamicCopy = GL_DYNAMIC_COPY,

        StreamDraw = GL_STREAM_DRAW,
        StreamRead = GL_STREAM_READ,
        StreamCopy = GL_STREAM_COPY,
    };

    enum class BufferTarget : GLenum {
        Array = GL_ARRAY_BUFFER,
        ElementArray = GL_ELEMENT_ARRAY_BUFFER,
    };

    class Buffer {
        bool owned;
        unsigned int handle;

        explicit Buffer(unsigned int handle, bool owned = true);

    public:

        virtual ~Buffer();

        static std::unique_ptr<Buffer> wrap(unsigned int handle, bool take_ownership = false);
        static std::unique_ptr<Buffer> create();
        static std::unique_ptr<Buffer> allocate(size_t size, BufferUsage usage = BufferUsage::DynamicDraw);
        static std::unique_ptr<Buffer> load(size_t size, const void* data, BufferUsage usage = BufferUsage::DynamicDraw);

        template<typename T> static std::shared_ptr<Buffer> load(const std::vector<T> &data, BufferUsage usage = BufferUsage::DynamicDraw) {
            return load_shared(data.size() * sizeof(T), data.data(), usage);
        };


        static std::shared_ptr<Buffer> wrap_shared(unsigned int handle, bool take_ownership = false);
        static std::shared_ptr<Buffer> create_shared();
        static std::shared_ptr<Buffer> allocate_shared(size_t size, BufferUsage usage = BufferUsage::DynamicDraw);
        static std::shared_ptr<Buffer> load_shared(size_t size, const void* data, BufferUsage usage = BufferUsage::DynamicDraw);


        template<typename T> static std::shared_ptr<Buffer> load_shared(const std::vector<T> &data, BufferUsage usage = BufferUsage::DynamicDraw) {
            return load_shared(data.size() * sizeof(T), data.data(), usage);
        };

        void bind(BufferTarget target) const;

        [[nodiscard]] unsigned int get_handle() const noexcept;
    };

} // gc
