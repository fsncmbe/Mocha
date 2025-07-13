#ifndef MOCHAUTILS_HPP
#define MOCHAUTILS_HPP

#include <mocha.hpp>

namespace mocha
{

void log(int level, const char* text);
void log(int level, int i);
void log(int level, double d);
void log(int level, float f);

}


#endif