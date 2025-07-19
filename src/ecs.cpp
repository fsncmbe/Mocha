#define MOCHA_ECS

#include <core.hpp>

namespace mocha::ecs
{

Entity create()
{
  return core.ecs.next_entity++;
}

void remove(Entity e)
{
  for (auto& [t, s] : core.ecs.sets)
  {
    ComponentSet<std::any>* c_set = static_cast<ComponentSet<std::any>*>(s);
    if (c_set->has(e))
    {
      c_set->remove(e);
    }
  }
}

void addSystem(System *sys)
{
  core.ecs.systems.push_back(sys);
}

void update()
{
  for (System* s : core.ecs.systems)
  {
    s->update();
  }
}

}