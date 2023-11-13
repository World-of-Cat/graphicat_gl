#include "utils.hpp"

namespace gc {

    void clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void clear(const glm::vec4 &color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void clear(const glm::vec3 &color) {
        glClearColor(color.r, color.g, color.b, 1.0f);
    }
}