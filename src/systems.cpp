#define MOCHA_SYSTEMS

#include <mocha.hpp>
#include <utils.hpp>
#include <core.hpp>

namespace mocha
{

struct : ISystem<CameraN>
{
  void update(float dt)
  {
    for (CameraN c : nodes)
    {
      glm::vec3 t_front;
    }
  }
} CameraSys;

}