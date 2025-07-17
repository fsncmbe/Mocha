#define MOCHA_SYSTEMS

#include <mocha.hpp>
#include <utils.hpp>
#include <core.hpp>

namespace mocha
{
class RenderSys : public System
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

class PhysicsSys : public System
{
  void update()
  {
    for (Entity e : ecs::view<ecs::Position, ecs::Physics>())
    {
      const auto& pos = ecs::get<ecs::Position>(e);
      const auto& phys = ecs::get<ecs::Physics>(e);
      
      glm::vec3 newpos;
      newpos = pos.pos + phys.velocity;
      glm::mat4 newtrans = glm::mat4(1.0f);
      newtrans = glm::translate(newtrans, newpos);

      ecs::Position p = {newpos, newtrans};
      ecs::Physics ph = {phys.speed, {0.0f, 0.0f, 0.0f}};

      ecs::emplace<ecs::Position>(e, p);
      ecs::emplace<ecs::Physics>(e, ph);
    }
  }
};

class InputSys : public System
{
  void update()
  {
    for (Entity e : ecs::view<ecs::InputBindings>())
    {
      std::map<std::pair<int, KeyState>, Command*> ib = ecs::get<ecs::InputBindings>(e);

      for (auto& [pair, command] : ib)
      {
        if (getKeyState(pair.first) == pair.second)
        {
          command->use(e);
        }
      }
    }
  }
};

class CameraSys : public System
{
 public:
  void update()
  {
    for (Entity e : ecs::view<ecs::Camera3D, ecs::Position>())
    {
      const auto& cam = ecs::get<ecs::Camera3D>(e);
      const auto& pos = ecs::get<ecs::Position>(e);
      
      glm::vec3 front;
      glm::vec3 right;
      glm::vec3 up;

      front.x = cos(glm::radians(core.render.yaw)) * cos(glm::radians(core.render.pitch));
      front.y = sin(glm::radians(core.render.pitch));
      front.z = sin(glm::radians(core.render.yaw)) * cos(glm::radians(core.render.pitch));
      front = glm::normalize(front);
      
      right = glm::normalize(glm::cross(front, core.render.world_up));
      up = glm::normalize(glm::cross(right, front));

      glm::mat4 view = glm::lookAt(pos.pos, pos.pos + front, up);
      glm::mat4 projection = glm::perspective(glm::radians(cam.zoom), 
      core.window.size.x/core.window.size.y, 0.1f, 100.0f);

      ecs::Camera3D newcam;
      newcam.speed = cam.speed;
      newcam.sens = cam.sens;
      newcam.zoom = cam.zoom;
      newcam.view = view;
      newcam.projection = projection;
      ecs::emplace<ecs::Camera3D>(e, newcam);
    }
  }
};
}