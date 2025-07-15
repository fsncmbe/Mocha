#define MOCHA_CORE

#include <core.hpp>
#include "mocha.hpp"

namespace mocha 
{
// global define core
Core core;

// struct functions
bool Vector2::operator==(const Vector2& other)
{
  return x == other.x && y == other.y;
}

bool Vector3::operator==(const Vector3& other)
{
  return x == other.x && y == other.y && z == other.z;
}

bool Vertex::operator==(const Vertex& other)
{
  return position == other.position 
      && tex_coord == other.tex_coord 
      && normal == other.normal;
}

bool Entity::operator==(const Entity& other)
{
  return id == other.id;
}

void Entity::operator<<(std::any component)
{
  c_map[core.ecs.components[std::type_index(typeid(component))]] = component;
}
}