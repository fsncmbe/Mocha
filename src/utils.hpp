#ifndef MOCHAUTILS_HPP
#define MOCHAUTILS_HPP

#include <mocha.hpp>

namespace mocha
{

void log(int level, const char* text);
void log(int level, std::string text);
void log(int level, int i);
void log(int level, double d);
void log(int level, float f);
void log(int level, Vector2 v);
void log(int level, Vector3 v);
}


#endif