#include <mocha.hpp>
#include <utils.hpp>

#define MOCHA_CORE

#ifdef LINUX
  #include <unistd.h>
#endif

#ifdef WINDOWS
  #include <windows.h>
#endif

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
  double fps;
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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  core.input.previous_key_states[key] = core.input.current_key_states[key];
  core.input.current_key_states[key] = action == GLFW_PRESS;
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}


// window

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
  core.timing.fps = 1.0/60.0;
  core.timing.previous = 0;
  core.assets.path = "assets/";
}

bool windowShouldClose()
{
  return getKeyDown(Key::kESCAPE);
}

bool Begin()
{
  // update delta time
  core.timing.current = glfwGetTime();
  core.timing.delta = core.timing.current - core.timing.previous;
  core.timing.previous = core.timing.current;

  if (core.timing.delta < core.timing.fps)
  {
    #ifdef LINUX
      sleep(core.timing.fps - core.timing.delta);
    #endif
    core.timing.delta = core.timing.fps;
  }

  // handle inputs
  glfwPollEvents();

  // clear screen
  clearColor(WHITE);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return !windowShouldClose();
}

void End()
{
  glfwSwapBuffers(core.window.glfw_window);
}

// drawing

void clearColor(Color color)
{
  glClearColor(color.r, color.g, color.b, color.a);
}


// timing

void setFPS(int fps)
{
  core.timing.fps = 1.0/(double)fps;
}

int getFPS()
{
  return (int)(1.0/core.timing.fps);
}

float getDT()
{
  return core.timing.delta;
}


// input

bool checkKeyOutOfBounds(int key)
{
  if (key > MAX_KEYS)
  {
    log(LogLevel::ERROR, "Key out of bounds!");
    return false;
  }
  return true;
}

bool getKeyPressed(int key)
{
  return  core.input.current_key_states[key] 
  &&      !core.input.previous_key_states[key] 
  &&      checkKeyOutOfBounds(key);
}

bool getKeyDown(int key)
{
  return  core.input.current_key_states[key]
  &&      checkKeyOutOfBounds(key);
}

bool getKeyReleased(int key)
{
  return  !core.input.current_key_states[key]
  &&      core.input.previous_key_states[key]
  &&      checkKeyOutOfBounds(key);
}

bool getKeyUp(int key)
{
  return  !core.input.current_key_states[key]
  &&      checkKeyOutOfBounds(key);
}


// resources

const char* loadFile(const char* path)
{
  std::string s = core.assets.path;
  s = s.append(path);
  std::ifstream file(s);

  if(!file.is_open())
  {
    std::string error_msg = "Failed to load: ";
    error_msg = error_msg.append(s);
    log(LogLevel::ERROR, error_msg.c_str());
  }

  std::string out((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());

  file.close();
  log(LogLevel::DEBUG, out.c_str());
  return out.c_str();
}

Shader loadShader(const char* name)
{
  std::string vpath = "shaders/" + (std::string)name + ".vs";
  const char* v_shader = loadFile(vpath.c_str());

  std::string fpath = "shaders/" + (std::string)name + ".fs";
  const char* f_shader = loadFile(fpath.c_str());

  unsigned int vertex, fragment;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &v_shader, NULL);
  glCompileShader(vertex);

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &f_shader, NULL);
  glCompileShader(fragment);

  int id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);
  
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  log(LogLevel::DEBUG, "Shader loaded!");
  return {id};
}

Model loadModel(const char* name)
{
  std::string path = "models/" + (std::string)name + ".obj";
  const char* obj_file = loadFile(path.c_str());
  
  const char* line_start = obj_file;

  for (const char* p = obj_file; *p != '\0'; ++p)
  {
    if (*p == '\n')
    {
      std::string line(line_start, p);
      // Do something with the line...
      log(LogLevel::DEBUG, line.c_str());
      line_start = p + 1;
    }
  }

  return {};
}

}