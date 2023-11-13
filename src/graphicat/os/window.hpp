#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace gc {

    // You probably don't want to pick this manually, you should depend on the color depth provided by a monitor.
    struct ColorDepth {
        unsigned int red = 8, green = 8, blue = 8;

        // These are more flexible... but if you are changing them you probably are making a mistake in your logic.
        unsigned int alpha = 8;
        unsigned int depth = 24;
        unsigned int stencil = 8;

        void apply() const;
    };

    struct VideoMode {
        glm::uvec2 size;
        ColorDepth color_depth;
        unsigned int refresh_rate;

        static VideoMode from(const GLFWvidmode *video_mode);

        void apply() const;
    };

    namespace wm {
        struct Windowed {
            glm::uvec2 size{800, 600};
            std::optional<glm::ivec2> position;
            bool decorated = true;
        };

        struct FullscreenWindowed {
            unsigned int monitor = 0;
        };

        struct FullscreenExclusive {
            unsigned int monitor = 0;
            std::optional<VideoMode> video_mode;
        };
    } // namespace wm

    using WindowMode = std::variant<wm::Windowed, wm::FullscreenWindowed, wm::FullscreenExclusive>;

    struct Monitor {
        GLFWmonitor *handle;

        [[nodiscard]] VideoMode get_video_mode() const;
        [[nodiscard]] std::string get_name() const;
        void set_gamma(float gamma) const;
        [[nodiscard]] glm::vec2 get_content_scale() const;
        [[nodiscard]] glm::ivec2 get_pos() const;
    };

    class WindowSystem {
        std::vector<GLFWmonitor *> monitors;

        inline static WindowSystem *s_window_system;

        WindowSystem();

        void reload_monitors();

      public:
        static void init();
        static void terminate();

        static WindowSystem *get() noexcept;

        ~WindowSystem();

        [[nodiscard]] std::optional<Monitor> try_get_monitor(unsigned int monitorIndex) const;
        [[nodiscard]] Monitor get_monitor_or(unsigned int monitorIndex, const Monitor &other) const;
        [[nodiscard]] Monitor get_monitor_or_primary(unsigned int monitorIndex) const;
        [[nodiscard]] Monitor get_monitor_or_last(unsigned int monitorIndex) const;
    };

    enum class ContextRobustness {
        None = GLFW_NO_ROBUSTNESS,
        NoResetNotification = GLFW_NO_RESET_NOTIFICATION,
        LoseContextOnReset = GLFW_LOSE_CONTEXT_ON_RESET,
    };

    enum class ContextReleaseBehavior {
        Any = GLFW_ANY_RELEASE_BEHAVIOR,
        Flush = GLFW_RELEASE_BEHAVIOR_FLUSH,
        None = GLFW_RELEASE_BEHAVIOR_NONE,
    };

    struct ContextProperties {
        bool debug = false;
        ContextRobustness robustness = ContextRobustness::None;
        ContextReleaseBehavior release_behavior = ContextReleaseBehavior::Any;

        void apply() const;
    };

    struct WindowProperties {
        std::string title = "Window";
        WindowMode window_mode = wm::Windowed({800, 600});
        bool resizable = false;
        bool visible = true;
        bool auto_iconify = true;
        bool floating = false;
        bool maximized = false;
        bool center_cursor = true;
        bool transparent_framebuffer = false;
        bool focus_on_show = true;
        bool scale_to_monitor = false;

        ContextProperties context_properties{};
    };

    class Window {
        GLFWwindow *window;

      public:
        explicit Window(const WindowProperties &properties);

        void set_pos(const glm::ivec2 &pos);
        void set_size(const glm::uvec2 &size);

        glm::ivec2 get_pos() const;

        [[nodiscard]] bool is_open() const;

        void update();

        void set_decorated(bool decorated);

        void set_window_mode(const WindowMode &wm);

        bool get_key(int key);


    };

} // namespace gc
