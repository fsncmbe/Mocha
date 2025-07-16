#define MOCHA_WINDOW

#include <mocha.hpp>
#include <utils.hpp>
#include <core.hpp>

#ifdef LINUX
  #include <unistd.h>
#endif

#ifdef WINDOWS
  #include <windows.h>
#endif

namespace mocha
{

// Predefine 
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void windowSizeCallback(GLFWwindow* window, int width, int height);

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

  // core settings
  core.window.fps = 1.0/60.0;
  core.window.previous = 0;
  core.assets.path = "assets/";
  core.render.world_up = {0.0f, 1.0f, 0.0f};


  // add cameras to core
  Entity cam = {};
  glm::vec3 pos =   {0, 0, 0};
  glm::vec3 up =    {0, 1, 0};
  glm::vec3 front = {0, 0,-1};
  glm::vec3 right = {1, 0, 0};
  cam << PositionC{pos, up, front, right};
  cam << CameraC();
  cam << BoundToC();
  Entity* cam_debug = addEntity(cam);
}

bool windowShouldClose()
{
  return getKeyDown(Key::kESCAPE);
}

bool Begin()
{
  // update delta time
  core.window.current = glfwGetTime();
  core.window.delta = core.window.current - core.window.previous;
  core.window.previous = core.window.current;

  if (core.window.delta < core.window.fps)
  {
    #ifdef LINUX
      sleep(core.window.fps - core.window.delta);
    #endif
    core.window.delta = core.window.fps;
  }

  // handle inputs
  glfwPollEvents();

  // clear screen
  clearColor(BLACK);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return !windowShouldClose();
}

void End()
{
  glfwSwapBuffers(core.window.glfw_window);
}

// Callbacks
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  core.input.previous_key_states[key] = core.input.current_key_states[key];
  core.input.current_key_states[key] = action == GLFW_PRESS;
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

//window

void setFPS(int fps)
{
  core.window.fps = 1.0/(double)fps;
}

int getFPS()
{
  return (int)(1.0/core.window.fps);
}

float getDT()
{
  return core.window.delta;
}

}