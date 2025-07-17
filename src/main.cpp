#include <iostream>
#include <string>

// Main file, will be used to for main loop and running lua scripts
#include <mocha.hpp>

int main()
{
  mocha::initWindow(1920, 1080, "hello");

  mocha::Shader s = mocha::loadShader("default");
  mocha::Model m = mocha::loadModel("cube");

  mocha::Entity e = mocha::ecs::create();
  mocha::ecs::Position p;
  p.pos = glm::vec3(1.0f);
  p.trans = glm::mat4(1.0f);
  mocha::ecs::emplace<mocha::ecs::Position>(e, p);

  // reg all components
  mocha::shaderUse(s);

  MOCHA_LOOP_START

  MOCHA_SYSTEMS_UPDATE


  MOCHA_LOOP_END

  return 0;
}