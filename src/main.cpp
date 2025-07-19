#include <iostream>
#include <string>

// Main file, will be used to for main loop and running lua scripts
#include <mocha.hpp>


struct Position 
{
  glm::vec3 pos;
  glm::mat4 trans;
};

int main()
{
  mocha::initWindow(1920, 1080, "hello");

  mocha::Shader s = mocha::loadShader("default");
  mocha::Model m = mocha::loadModel("cube");
  auto e = mocha::ecs::create();
  mocha::ecs::emplace<Position>(e, {{0, 0, 0}, glm::mat4(1)});
  mocha::shaderUse(s);

  MOCHA_LOOP_START

  MOCHA_SYSTEMS_UPDATE

  MOCHA_LOOP_END

  return 0;
}