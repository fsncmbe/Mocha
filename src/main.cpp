#include <iostream>
#include <string>

// Main file, will be used to for main loop and running lua scripts
#include <mocha.hpp>

int main()
{
  mocha::initWindow(1920, 1080, "hello");

  mocha::Shader s = mocha::loadShader("default");
  mocha::Model m = mocha::loadModel("cube");

  MOCHA_LOOP_START

  MOCHA_LOOP_END

  return 0;
}