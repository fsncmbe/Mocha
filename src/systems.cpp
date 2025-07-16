#define MOCHA_SYSTEMS

#include <mocha.hpp>
#include <utils.hpp>
#include <core.hpp>

namespace mocha
{
class RenderSys : System
{
 public:
  void update()
  {
    for (Entity e : ecs::view<ecs::Render, ecs::Position>())
    {
      const auto& model = ecs::get<ecs::Render>(e);
      const auto& pos  = ecs::get<ecs::Position>(e);

      drawModel(model, pos.trans);
    }
  }
};

class CameraSys : System
{
 public:
  void update()
  {
    for (Entity e : ecs::view<ecs::Camera3D, ecs::Position>())
    {
      const auto& cam = ecs::get<ecs::Camera3D>(e);
      const auto& pos = ecs::get<ecs::Position>(e);
      
    }
  }
};

}