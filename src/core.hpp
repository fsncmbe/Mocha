#ifndef MOCHA_CORE_HPP
#define MOCHA_CORE_HPP

#include <mocha.hpp>

#define MAX_KEYS          512
#define MAX_MOUSE_BUTTONS 8
#define MAX_GAMEPADS      4

namespace mocha 
{
struct Core {
  struct {
    std::string title;
    GLFWwindow* glfw_window;

    glm::vec2 size;         // whole screen
    glm::vec2 render_offset;  // offset from screen to render

    double current;
    double previous;
    double delta;
    double fps;
  } window;

  struct {
    // Shader
    Shader    current_shader;

    // Cam
    glm::vec3 world_up;
    float yaw;
    float pitch;
    Entity*   cam_debug;
    Entity*   cam_current;

  } render;

  struct {
    const char* path;
  } assets;

  struct {
    bool current_key_states[MAX_KEYS];
    bool previous_key_states[MAX_KEYS];

    bool current_mouse_button_states[MAX_MOUSE_BUTTONS];
    bool previous_mouse_button_states[MAX_MOUSE_BUTTONS];
  } input;

  struct {
    std::unordered_map<std::type_index, void*> sets;
    Entity next_entity = 0;
    std::vector<System*> systems;
  } ecs;
};
// Define global core
extern Core core;
}

#endif