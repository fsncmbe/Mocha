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
#include <typeindex>
#include <functional>

#include <lua/lua.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef __linux__
  #define LINUX
#endif

#ifdef _WIN32
  #define WINDOWS
#endif

// Combines every submodule into one big hpp

namespace mocha
{

// -------------------- STRUCT
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

// non positional rectangle, add position vector if needed
struct Rectangle {
  float width;
  float height;
};

struct Shader {
  int id;
};

struct Model {
  int indices_count;
  unsigned int vao;
};

// ecs

struct ComponentMap
{
  std::unordered_map<std::string, std::any> components;

  ComponentMap(const std::vector<std::any>& vec = {});
  ~ComponentMap(){components.clear();};

  std::any& operator[](std::string& s);
  std::any& operator[](const char* s);
};

struct Entity {
  ComponentMap c_map;
  int id;

  bool operator==(const Entity& other);
  void operator<<(std::any component);
};

// Needs to be implemented by every system
template<typename T>
struct ISystem {
  virtual void update(float dt) = 0;

  void addNode(T n)
  {
    nodes.push_back(n);
  }

  void delNode(T n)
  {
    auto it = std::find(nodes.begin(), nodes.end(), n);

    if (it != nodes.end())
    {
      nodes.erase(it);
    }
  }

  std::vector<T> nodes;
};

// components
struct PositionC {
  glm::vec3 pos;
  glm::vec3 up;
  glm::vec3 front;
  glm::vec3 right;
};

struct MovementC {
  glm::vec3 direction;
};

struct RenderC {
  Model model;
};

struct CameraC {
  float yaw   = -90.0f;
  float pitch =   0.0f;
  float speed =   2.5f;
  float sens  =   0.1f;
  float zoom  =  45.0f;

  glm::mat4 view;
  glm::mat4 projection;
};

struct BoundToC {
  Entity* entity;
};

// nodes

struct Node {
  int e_id;
  bool operator==(Node& other);
};

struct RenderN : Node {
  PositionC* trans;
  RenderC*   render;
};

struct CameraN : Node {
  PositionC* pos;
  CameraC*   cam;
  BoundToC*  bound;
};

// -------------------- DEFINES
const Color BLACK   = {0, 0, 0, 255};
const Color BLUE    = {0, 0, 255, 255};
const Color RED     = {255, 0, 0, 255};
const Color GREEN   = {0, 255, 0, 255};
const Color YELLOW  = {255, 255, 0, 255};
const Color PINK    = {255, 0, 255, 255};
const Color TURQ    = {0, 255, 255, 255};
const Color WHITE   = {255, 255, 255, 255};

// -------------------- ENUMS

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

// resources
std::string loadFile(const std::string& path);
Shader      loadShader(const std::string& name);
Model       loadModel(const std::string& name);

// ecs
void    registerComp(std::type_index t, const std::string& name);
Entity* addEntity();
Entity* addEntity(Entity e);
void    delEntity(Entity e);
void    clearEntities();
void    updateSystems();

// lua functions and bindings
void luaBindings();
void runScripts();
}

// Syntax defines
#define MOCHA_LOOP_START while(mocha::Begin()) {
#define MOCHA_LOOP_END mocha::End();}

#endif