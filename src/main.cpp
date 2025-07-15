#include <iostream>
#include <string>

// Main file, will be used to for main loop and running lua scripts
#include <mocha.hpp>

int main()
{
  mocha::initWindow(1920, 1080, "hello");

  mocha::Shader s = mocha::loadShader("default");
  mocha::Model m = mocha::loadModel("cube");

  mocha::registerComp(std::type_index(typeid(mocha::Transformation)), "Transformation");
  mocha::registerComp(std::type_index(typeid(mocha::Movement)), "Movement");

  mocha::Transformation t{{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};
  mocha::addEntity({mocha::ComponentMap({t})});
  mocha::shaderUse(s);

  MOCHA_LOOP_START
  mocha::drawModel(m);

  MOCHA_LOOP_END

  return 0;
}