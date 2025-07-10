#include <mocha.hpp>
#include <utils.hpp>

#define MOCHA_CORE

namespace mocha 
{

#define MAX_KEYS          512
#define MAX_MOUSE_BUTTONS 8
#define MAX_GAMEPADS      4

namespace 
{
struct
{
  // window
  struct {
  const char* title;
  bool should_close;
  GLFWwindow* glfw_window;

  Rectangle screen;         // whole screen
  Rectangle render_screen;  // render area
  Vector2   render_offset;  // offset from screen to render
  } window;

  // timing
  struct {
  double current;
  double previous;
  double delta;
  } timing;

  // assets
  struct {
  const char* path;         // path to assets
  } assets;

  // input
  struct {
  bool current_key_states[MAX_KEYS];
  bool previous_key_states[MAX_KEYS];

  bool current_mouse_button_states[MAX_MOUSE_BUTTONS];
  bool previous_mouse_button_states[MAX_MOUSE_BUTTONS];
  } input;
} core;
} // private namespace

void initWindow(int width, int height, const char* title)
{

  log(LogLevel::INFO, "Starting Mocha Engine...");
  if (!glfwInit())
  {
    log(LogLevel::FATAL, "GLFW INIT FAILED!");
    glfwTerminate();
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  core.window.glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);

  if (!core.window.glfw_window)
  {
    log(LogLevel::FATAL, "GLFW WINDOW CREATION FAILED!");
    glfwTerminate();
  }

  glfwMakeContextCurrent(core.window.glfw_window);
  glfwSetFramebufferSizeCallback(core.window.glfw_window, windowSizeCallback);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    log(LogLevel::ERROR, "GLAD FAILED TO LOAD!");
  }

  // opengl settings
  glfwSetInputMode(core.window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);

  // input callbacks
  glfwSetKeyCallback(core.window.glfw_window, keyCallback);
  // mouse button callback
  // mouse pos callback
}

void Begin()
{
  // update delta time
  core.timing.current = glfwGetTime();
  core.timing.delta = core.timing.current - core.timing.previous;
  core.timing.previous = core.timing.current;

  // handle inputs
  glfwPollEvents();

  // clear screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void End()
{
  glfwSwapBuffers(core.window.glfw_window);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  core.input.previous_key_states[key] = core.input.current_key_states[key];
  core.input.current_key_states[key] = action == GLFW_PRESS;
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void clearColor(Color color)
{
  glClearColor(color.r, color.g, color.b, color.a);
}

}