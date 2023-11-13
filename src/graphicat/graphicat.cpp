#include "graphicat.hpp"

#include "graphicat/os/window.hpp"

namespace gc {

    void GlobalState::init(const GraphicatProperties &properties) {
        if (!s_global_state)
            s_global_state = new GlobalState(properties);
    }

    void GlobalState::terminate() {
        delete s_global_state;
        s_global_state = nullptr;
    }

    GlobalState *GlobalState::get() { return s_global_state; }

    GlobalState::GlobalState(const GraphicatProperties &properties) {
        gc::WindowSystem::init();
    }

    GlobalState::~GlobalState() { gc::WindowSystem::terminate(); }

} // namespace gc