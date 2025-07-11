#include <iostream>
#include <string>

// Main file, will be used to for main loop and running lua scripts
#include <mocha.hpp>

int main()
{
  mocha::initWindow(1920, 1080, "hello");

  mocha::Begin();

  mocha::End();
  return 0;
}