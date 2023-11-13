#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace gc {
    struct GraphicatProperties {};

    class GlobalState {
        inline static GlobalState *s_global_state;

        GlobalState(const GraphicatProperties &properties = {});

    public : ~GlobalState();

        static void init(const GraphicatProperties &properties = {});
        static void terminate();

        static GlobalState *get();
    };


} // namespace gc