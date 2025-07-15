#include <iostream>
#include <string>

// Main file, will be used to for main loop and running lua scripts
#include <mocha.hpp>

int main()
{
  mocha::initWindow(1920, 1080, "hello");

  mocha::Shader s = mocha::loadShader("default");
  mocha::Model m = mocha::loadModel("cube");

  // reg all components
  mocha::registerComp(std::type_index(typeid(mocha::TransformationC)), "Transformation");
  mocha::registerComp(std::type_index(typeid(mocha::MovementC)), "Movement");

  mocha::Entity* e = mocha::addEntity();
  mocha::TransformationC t;
  e->operator<<(t);
  mocha::shaderUse(s);

  MOCHA_LOOP_START
  
  mocha::shaderSet(s, "uColor", mocha::BLUE);
  mocha::drawModel(m);
  mocha::shaderSet(s, "uColor", mocha::BLACK);
  mocha::drawCube({1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {10, 10, 10, 10});

  MOCHA_LOOP_END

  return 0;
}