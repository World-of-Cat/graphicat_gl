#include "buffer.hpp"

namespace gc {

    Buffer::Buffer(unsigned int handle, bool owned) : handle(handle), owned(owned) {

    }

    Buffer::~Buffer() {
        if (owned)
            glDeleteBuffers(1, &handle);
    }

    static unsigned int raw_buffer_create() {
        unsigned int b;
        glCreateBuffers(1, &b);
        return b;
    }

    static unsigned int raw_load_buffer(size_t size, const void* data, BufferUsage usage) {
        unsigned int b = raw_buffer_create();

        glNamedBufferData(b, static_cast<GLsizeiptr>(size), data, static_cast<GLenum>(usage));

        return b;
    }

    static unsigned int raw_allocate_buffer(size_t size, BufferUsage usage) {
        return raw_load_buffer(size, nullptr, usage);
    }



    std::unique_ptr<Buffer> Buffer::wrap(unsigned int handle, bool take_ownership) {
        return std::unique_ptr<Buffer>(new Buffer(handle, take_ownership));
    }

    std::unique_ptr<Buffer> Buffer::create() {
        return wrap(raw_buffer_create(), true);
    }

    std::unique_ptr<Buffer> Buffer::allocate(size_t size, BufferUsage usage) {
        return wrap(raw_allocate_buffer(size, usage), true);
    }

    std::unique_ptr<Buffer> Buffer::load(size_t size, const void *data, BufferUsage usage) {
        return wrap(raw_load_buffer(size, data, usage), true);
    }

    std::shared_ptr<Buffer> Buffer::wrap_shared(unsigned int handle, bool take_ownership) {
        return std::shared_ptr<Buffer>(new Buffer(handle, take_ownership));
    }

    std::shared_ptr<Buffer> Buffer::create_shared() {
        return wrap_shared(raw_buffer_create(), true);
    }

    std::shared_ptr<Buffer> Buffer::allocate_shared(size_t size, BufferUsage usage) {
        return wrap_shared(raw_allocate_buffer(size, usage), true);
    }

    std::shared_ptr<Buffer> Buffer::load_shared(size_t size, const void *data, BufferUsage usage) {
        return wrap_shared(raw_load_buffer(size, data, usage), true);
    }

    void Buffer::bind(BufferTarget target) const {
        glBindBuffer(static_cast<GLenum>(target), handle);
    }

    unsigned int Buffer::get_handle() const noexcept {
        return handle;
    }
} // gc