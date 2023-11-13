#include "window.hpp"
#include <iostream>
#include <glad/gl.h>

namespace gc {
    void WindowSystem::init() {
        if (!s_window_system)
            s_window_system = new WindowSystem();
    }

    void WindowSystem::terminate() {
        delete s_window_system;
        s_window_system = nullptr;
    }

    WindowSystem *WindowSystem::get() noexcept { return s_window_system; }

    WindowSystem::WindowSystem() {
        glfwInit();
        reload_monitors();
        glfwSetMonitorCallback([](GLFWmonitor *, int) { s_window_system->reload_monitors(); });
    }

    WindowSystem::~WindowSystem() { glfwTerminate(); }

    std::optional<Monitor> WindowSystem::try_get_monitor(unsigned int monitorIndex) const {
        if (monitorIndex < monitors.size())
            return Monitor(monitors[monitorIndex]);
        return std::nullopt;
    }

    Monitor WindowSystem::get_monitor_or(unsigned int monitorIndex, const Monitor &other) const {
        return try_get_monitor(monitorIndex).value_or(other);
    }

    Monitor WindowSystem::get_monitor_or_primary(unsigned int monitorIndex) const { return get_monitor_or(monitorIndex, Monitor(monitors[0])); }

    Monitor WindowSystem::get_monitor_or_last(unsigned int monitorIndex) const {
        return get_monitor_or(monitorIndex, Monitor(monitors[monitorIndex - 1]));
    }

    std::string Monitor::get_name() const { return glfwGetMonitorName(handle); }

    void Monitor::set_gamma(float gamma) const { glfwSetGamma(handle, gamma); }

    glm::vec2 Monitor::get_content_scale() const {
        glm::vec2 cs;
        glfwGetMonitorContentScale(handle, &cs.x, &cs.y);
        return cs;
    }

    glm::ivec2 Monitor::get_pos() const {
        glm::ivec2 p;
        glfwGetMonitorPos(handle, &p.x, &p.y);
        return p;
    }

    void WindowSystem::reload_monitors() {
        int monitorCount;
        auto **pp_monitors = glfwGetMonitors(&monitorCount);
        monitors.resize(monitorCount);
        monitors.assign(pp_monitors, pp_monitors + monitorCount);
    }

    VideoMode Monitor::get_video_mode() const {
        const auto *vm = glfwGetVideoMode(handle);
        return VideoMode::from(vm);
    }

    VideoMode VideoMode::from(const GLFWvidmode *video_mode) {
        return {
            {
                static_cast<unsigned int>(video_mode->width),
                static_cast<unsigned int>(video_mode->height),
            },
            {
                static_cast<unsigned int>(video_mode->redBits),
                static_cast<unsigned int>(video_mode->greenBits),
                static_cast<unsigned int>(video_mode->blueBits),
            },
            static_cast<unsigned int>(video_mode->refreshRate),
        };
    }

    void VideoMode::apply() const {
        color_depth.apply();
        glfwWindowHint(GLFW_REFRESH_RATE, static_cast<int>(refresh_rate));
    }

    void ColorDepth::apply() const {
        glfwWindowHint(GLFW_RED_BITS, static_cast<int>(red));
        glfwWindowHint(GLFW_GREEN_BITS, static_cast<int>(green));
        glfwWindowHint(GLFW_BLUE_BITS, static_cast<int>(blue));
        glfwWindowHint(GLFW_ALPHA_BITS, static_cast<int>(alpha));
        glfwWindowHint(GLFW_DEPTH_BITS, static_cast<int>(depth));
        glfwWindowHint(GLFW_STENCIL_BITS, static_cast<int>(stencil));
    }

    Window::Window(const WindowProperties &properties) {
        GLFWmonitor *fs_monitor = nullptr;
        glm::uvec2 size{};
        std::optional<glm::ivec2> pos;

        glfwDefaultWindowHints();

        switch (properties.window_mode.index()) {
        case 0: {
            wm::Windowed w = std::get<wm::Windowed>(properties.window_mode);
            size = w.size;
            pos = w.position;
            glfwWindowHint(GLFW_DECORATED, w.decorated);
        } break;
        case 1: {
            wm::FullscreenWindowed w = std::get<wm::FullscreenWindowed>(properties.window_mode);
            Monitor monitor = WindowSystem::get()->get_monitor_or_primary(w.monitor);
            VideoMode vm = monitor.get_video_mode();
            vm.apply();

            //            glfwWindowHint(GLFW_DECORATED, false);

            size = vm.size;
            pos = monitor.get_pos();
        } break;
        case 2: {
            wm::FullscreenExclusive w = std::get<wm::FullscreenExclusive>(properties.window_mode);
            Monitor monitor = WindowSystem::get()->get_monitor_or_primary(w.monitor);
            VideoMode vm = w.video_mode.value_or(monitor.get_video_mode());
            vm.apply();

            size = vm.size;
            fs_monitor = monitor.handle;
        } break;
        }

        glfwWindowHint(GLFW_RESIZABLE, properties.resizable);
        glfwWindowHint(GLFW_VISIBLE, properties.visible);
        glfwWindowHint(GLFW_AUTO_ICONIFY, properties.auto_iconify);
        glfwWindowHint(GLFW_FLOATING, properties.floating);
        glfwWindowHint(GLFW_MAXIMIZED, properties.maximized);
        glfwWindowHint(GLFW_CENTER_CURSOR, properties.center_cursor);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, properties.transparent_framebuffer);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, properties.focus_on_show);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, properties.scale_to_monitor);

        properties.context_properties.apply();


        window = glfwCreateWindow(static_cast<int>(size.x), static_cast<int>(size.y), properties.title.c_str(), fs_monitor, nullptr);

        if (!window) {
            const char* err;
            glfwGetError(&err);
            std::cerr << "Failed to create_shared window: " << err << std::endl;
        }

        if (pos.has_value()) {
            // Set size again to make sure size didn't get clipped by the window being placed such that part of it was out of the bounds of the
            // available space (important for borderless fullscreen).
            if (properties.window_mode.index() == 1)
                set_decorated(false);

            set_pos(pos.value());
            set_size(size);
        }

        glfwMakeContextCurrent(window);
        gladLoadGL(glfwGetProcAddress);

    }

    void Window::set_pos(const glm::ivec2 &pos) { glfwSetWindowPos(window, pos.x, pos.y); }

    void Window::set_size(const glm::uvec2 &size) { glfwSetWindowSize(window, static_cast<int>(size.x), static_cast<int>(size.y)); }

    bool Window::is_open() const { return !glfwWindowShouldClose(window); }

    void Window::update() {
        glfwSwapBuffers(window);
    }

    void Window::set_decorated(bool decorated) { glfwSetWindowAttrib(window, GLFW_DECORATED, decorated); }

    void Window::set_window_mode(const WindowMode &wm) {
        glm::uvec2 size{};
        std::optional<glm::ivec2> pos;
        GLFWmonitor *fs_monitor = nullptr;
        int refresh_rate = GLFW_DONT_CARE;

        switch (wm.index()) {
        case 0: {
            wm::Windowed w = std::get<wm::Windowed>(wm);
            size = w.size;
            pos = w.position.value_or(glm::ivec2(200, 200));
            set_decorated(w.decorated);
        } break;
        case 1: {
            wm::FullscreenWindowed w = std::get<wm::FullscreenWindowed>(wm);
            Monitor monitor = WindowSystem::get()->get_monitor_or_primary(w.monitor);
            VideoMode vm = monitor.get_video_mode();

            set_decorated(true);

            size = vm.size;
            pos = monitor.get_pos();
            refresh_rate = static_cast<int>(vm.refresh_rate);
        } break;
        case 2: {
            wm::FullscreenExclusive w = std::get<wm::FullscreenExclusive>(wm);
            Monitor monitor = WindowSystem::get()->get_monitor_or_primary(w.monitor);
            VideoMode vm = w.video_mode.value_or(monitor.get_video_mode());

            size = vm.size;
            fs_monitor = monitor.handle;
            refresh_rate = static_cast<int>(vm.refresh_rate);
        } break;
        }

        glm::ivec2 pos_ = pos.value_or(get_pos());

        glfwSetWindowMonitor(window, fs_monitor, pos_.x, pos_.y, static_cast<int>(size.x), static_cast<int>(size.y), refresh_rate);

        if (pos.has_value()) {
            // Set size again to make sure size didn't get clipped by the window being placed such that part of it was out of the bounds of the
            // available space (important for borderless fullscreen).
            if (wm.index() == 1)
                set_decorated(false);

            set_pos(pos.value());
            set_size(size);
        }
    }

    glm::ivec2 Window::get_pos() const {
        glm::ivec2 p;
        glfwGetWindowPos(window, &p.x, &p.y);
        return p;
    }

    bool Window::get_key(int key) { return glfwGetKey(window, key); }

    void ContextProperties::apply() const {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
        glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, static_cast<int>(robustness));
        glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, static_cast<int>(release_behavior));
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug);
    }
} // namespace gc