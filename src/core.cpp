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

  // ecs
  struct {
    std::vector<Entity> entites;
    std::unordered_map<std::type_index, std::string> components;
    std::vector<System> sytems;
  } ecs;
} core;
} // private namespace

// struct functions

bool Vector2::operator==(const Vector2& other)
{
  return x == other.x && y == other.y;
}

bool Vector3::operator==(const Vector3& other)
{
  return x == other.x && y == other.y && z == other.z;
}

bool Vertex::operator==(const Vertex& other)
{
  return position == other.position 
      && tex_coord == other.tex_coord 
      && normal == other.normal;
}

// callbacks

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
  clearColor(BLACK);
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

void drawModel(Model m)
{
  glBindVertexArray(m.vao);
  glDrawElements(GL_TRIANGLES, m.indices_count, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
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

std::string loadFile(const std::string& path)
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
  return out;
}

Shader loadShader(const std::string& name)
{
  std::string v_path = "shaders/" + name + ".vs";
  std::string v_string = loadFile(v_path);
  const char* v_shader = v_string.c_str();

  std::string f_path = "shaders/" + name + ".fs";
  std::string f_string = loadFile(f_path);
  const char* f_shader = f_string.c_str();

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

Model loadModel(const std::string& name)
{

  // file handling
  std::string path = "models/" + (std::string)name + ".obj";

  // add a break at the end so the loop recognizes it 
  std::string obj_string = loadFile(path) + '\n';
  const char* obj_file = obj_string.c_str();

  // vectors for data
  std::vector<Vertex>       vertices;
  std::vector<unsigned int> indices;
  std::vector<Vector3>      temp_positions;
  std::vector<Vector3>      temp_normals;
  std::vector<Vector2>      temp_uvs;

  // fill vertex_indices... with data
  const char* line_start = obj_file;

  for (const char* p = obj_file; *p != '\0'; ++p)
  {
    if (*p == '\n')
    {
      std::string line(line_start, p);

      std::istringstream line_stream(line);
      std::string type;

      line_stream >> type;

      // vertex
      if (type == "v")
      {
        Vector3 pos;
        line_stream >> pos.x >> pos.y >> pos.z;
        temp_positions.push_back(pos);
      }
      // vertex texture
      else if (type == "vt")
      {
        Vector2 uv;
        line_stream >> uv.x >> uv.y;
        temp_uvs.push_back(uv);
      }
      // vertex normal
      else if (type == "vn")
      {
        Vector3 normal;
        line_stream >> normal.x >> normal.y >> normal.z;
        temp_normals.push_back(normal);
      }
      // material
      else if (type == "usemtl")
      {
        
      }
      // face
      else if (type == "f")
      {
        const int f_nums = 3;
        std::string strings[f_nums];
        for (int i=0; i<f_nums; i++)
        {
          line_stream >> strings[i];
          unsigned int vertex_index, uv_index, normal_index;
          sscanf(strings[i].c_str(), "%d/%d/%d", &vertex_index, &uv_index, &normal_index);

          log(LogLevel::DEBUG, strings[i]);
          Vertex v;

          v.position  = temp_positions[vertex_index-1];
          v.tex_coord = temp_uvs[uv_index-1];
          v.normal    = temp_normals[normal_index-1];

          auto it = std::find(vertices.begin(), vertices.end(), v);

          if (it != vertices.end())
          {
            int index = std::distance(vertices.begin(), it);
            indices.push_back(index);
          } else {
            vertices.push_back(v);
            indices.push_back(vertices.size()-1);
          }
        }
      }
      line_start = p + 1;
    }
  }

  unsigned int vao, vbo, ebo;

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  glBindVertexArray(0);

  Model m;
  m.vao = vao;
  m.indices_count = indices.size();
  return m;
}

// shader

void shaderSet(Shader shader, const std::string& name, bool b)
{
  glUniform1i(glGetUniformLocation(shader.id, name.c_str()), (int)b);
}

void shaderSet(Shader shader, const std::string& name, int i)
{
  glUniform1i(glGetUniformLocation(shader.id, name.c_str()), i);
}

void shaderSet(Shader shader, const std::string& name, float f)
{
  glUniform1f(glGetUniformLocation(shader.id, name.c_str()), f);
}

void shaderSet(Shader shader, const std::string& name, Vector2 v2)
{
  glUniform2f(glGetUniformLocation(shader.id, name.c_str()), v2.x, v2.y);
}

void shaderSet(Shader shader, const std::string& name, Vector3 v3)
{
  glUniform3f(glGetUniformLocation(shader.id, name.c_str()), v3.x, v3.y, v3.z);
}

void shaderUse(Shader shader)
{
  glUseProgram(shader.id);
}

// ecs

ComponentMap::ComponentMap(const std::vector<std::any>& vec)
{
  if (!vec.empty())
  {
    for (auto i : vec)
    {
      components[core.ecs.components[std::type_index(typeid(i.type()))]] = i;
    }
  }
};

void registerComp(std::type_index t, const std::string& name)
{
  core.ecs.components[t] = name;
}

Entity* addEntity()
{
  core.ecs.entites.push_back({});
  return &core.ecs.entites.back();
}

Entity* addEntity(Entity e)
{
  core.ecs.entites.push_back(e);
  return &core.ecs.entites.back();
}

void updateSystems()
{
  for (System s : core.ecs.sytems)
  {
    s.update(core.timing.delta);
  }
}

}