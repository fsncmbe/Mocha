#define MOCHA_CORE

#include <core.hpp>
#include "mocha.hpp"

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

std::any& ComponentMap::operator[](std::string& s)
{
  return components[s];
}

std::any& ComponentMap::operator[](const char* s)
{
  return components[s];
}

bool Entity::operator==(const Entity& other)
{
  return id == other.id;
}

void Entity::operator<<(std::any component)
{
  c_map[typeid(component).name()] = component;
}

bool Node::operator==(Node& other)
{
  return e_id = other.e_id;
}

}