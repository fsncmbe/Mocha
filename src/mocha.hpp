#ifndef MOCHA_HPP
#define MOCHA_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <any>
#include <unordered_map>
#include <map>
#include <typeindex>
#include <functional>

#include <lua/lua.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ecs.hpp>

#ifdef __linux__
  #define LINUX
#endif

#ifdef _WIN32
  #define WINDOWS
#endif

namespace mocha
{

// ENUMS
enum LogLevel {
  ALL = 0,
  DEBUG,    // internal debugging, disable on release
  INFO,     // program execution info
  WARNING,  // recoverable errors
  ERROR,    // unrecoverable errors
  FATAL,    // program needs to exit
  NONE
};

enum Key {
kNULL = 0,
SPACE = 32,
k0 = 48,
k1 = 49,
k2 = 50,
k3 = 51,
k4 = 52,
k5 = 53,
k6 = 54,
k7 = 55,
k8 = 56,
k9 = 57,
kA = 65,
kB = 66,
kC = 67,
kD = 68,
kE = 69,
kF = 70,
kG = 71,
kH = 72,
kI = 73,
kJ = 74,
kK = 75,
kL = 76,
kM = 77,
kN = 78,
kO = 79,
kP = 80,
kQ = 81,
kR = 82,
kS = 83,
kT = 84,
kU = 85,
kV = 86,
kW = 87,
kX = 88,
kY = 89,
kZ = 90,
kESCAPE = 256,
kENTER = 257,
kTAB = 258,
kBACKSPACE = 259,
kRIGHT = 262,
kLEFT = 263,
kDOWN = 264,
kUP = 265,
};

enum KeyState {
  kPressed = 0,
  kDown,
  kReleased,
  kUp,
};

// STRUCTS
struct Vertex {
  glm::vec3 position;
  glm::vec2 tex_coord;
  glm::vec3 normal;

  bool operator==(const Vertex& other);
};

struct Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
};

struct Rectangle {
  float width;
  float height;
};

struct Box {
  float width;
  float height;
  float depth;
};

struct Shader {
  int id;
};

struct Model {
  int indices_count;
  unsigned int vao;
};

struct Camera {
  float     speed;
  float     sens;
  float     zoom;
  glm::mat4 view;
  glm::mat4 projection;
};

// ECS 

using Entity = unsigned int;

struct System {
  virtual void update() = 0;
};

struct Command {
  std::function<void(Entity&)> use;
  Command(std::function<void(Entity&)> u) : use(u) {};
};

// COMPONENTS in ecs namespace
// differentiate better and reuse names
namespace ecs
{
using Render   =  Model;
using Camera3D =  Camera;
struct Physics {
  float     speed;
  glm::vec3 velocity;
};
using InputBindings = std::map<std::pair<int, KeyState>, Command*>;
}

// -------------------- FUNCTIONS

// window
void initWindow(int width, int height, const char* title);
void closeWindow();
bool windowShouldClose();
bool Begin();
void End();

void  setFPS(int fps);
int   getFPS();
float getDT();

// drawing
void clearColor(Color color);
void drawModel(Model m);
void drawModel(Model m, glm::mat4 trans);
void drawCube(glm::vec3 pos, glm::vec3 size, Color color);

void shaderSet(Shader shader, const std::string& name, bool b);
void shaderSet(Shader shader, const std::string& name, int i);
void shaderSet(Shader shader, const std::string& name, float f);
void shaderSet(Shader shader, const std::string& name, glm::vec2 v2);
void shaderSet(Shader shader, const std::string& name, glm::vec3 v3);
void shaderSet(Shader shader, const std::string& name, Color c);
void shaderSet(Shader shader, const std::string& name, glm::mat4 m);
void shaderUse(Shader shader);

// input
bool getKeyPressed(int key);
bool getKeyDown(int key);
bool getKeyReleased(int key);
bool getKeyUp(int key);
KeyState getKeyState(int key);

// resources
std::string loadFile(const std::string& path);
Shader      loadShader(const std::string& name);
Model       loadModel(const std::string& name);

// ecs
#define COMPONENT template<typename Component>
#define COMPONENTS template<typename... Component>


// lua
void luaBindings();
void runScripts();

// const
const Color BLACK   = {0, 0, 0, 255};
const Color BLUE    = {0, 0, 255, 255};
const Color RED     = {255, 0, 0, 255};
const Color GREEN   = {0, 255, 0, 255};
const Color YELLOW  = {255, 255, 0, 255};
const Color PINK    = {255, 0, 255, 255};
const Color TURQ    = {0, 255, 255, 255};
const Color WHITE   = {255, 255, 255, 255};
}
// Syntax defines
#define MOCHA_LOOP_START while(mocha::Begin()) {
#define MOCHA_SYSTEMS_UPDATE mocha::ecs::update();
#define MOCHA_LOOP_END mocha::End();}

#endif