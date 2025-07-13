#ifndef MOCHA_HPP
#define MOCHA_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef __linux__
  #define LINUX
#endif

#ifdef _WIN32
  #define WINDOWS
#endif

// Combines every submodule into one big hpp

namespace mocha
{

// -------------------- STRUCTS
struct Vector2 {
  float x;
  float y;
};

struct Vector3 {
  float x;
  float y;
  float z;
};

struct Vector4 {
  float x;
  float y;
  float z;
  float w;
};

struct Matrix {
  float m0, m4, m8, m12;
  float m1, m5, m9, m13;
  float m2, m6, m10, m14;
  float m3, m7, m11, m15;
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

struct Mesh {
  int   vertex_count;
  
  unsigned int VAO;
  unsigned int VBO;
};

struct Model {
  //Matrix            transform;
  //std::vector<Mesh> meshes;
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

enum CameraMode {
  FREE,         // no target
  ORBITAL,      // around a target
  FIRST_PERSON, // eyes view of target
  THIRD_PERSOM, // third person of target
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

// drawing
void clearColor(Color color);

// timing
void setFPS(int fps);
int getFPS();
float getDT();

// input
bool getKeyPressed(int key);
bool getKeyDown(int key);
bool getKeyReleased(int key);
bool getKeyUp(int key);

// resources
const char* loadFile(const char* path);
Shader loadShader(const char* name);
Model loadModel(const char* name);
}

// Syntax defines

#define MOCHA_LOOP_START while(mocha::Begin()) {
#define MOCHA_LOOP_END mocha::End();}

#endif