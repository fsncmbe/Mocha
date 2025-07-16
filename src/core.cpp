#define MOCHA_CORE

#include <core.hpp>

namespace mocha 
{
// global define core
Core core;

// struct functions

bool Vertex::operator==(const Vertex& other)
{
  return position == other.position 
      && tex_coord == other.tex_coord 
      && normal == other.normal;
}

}