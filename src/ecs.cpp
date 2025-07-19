#define MOCHA_ECS

#include <ecs.hpp>

namespace mocha::ecs
{

Core_ecs core_ecs;

Entity create()
{
  return core_ecs.next_entity++;
}

void remove(Entity e)
{
  for (auto& [t, s] : core_ecs.sets)
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
  core_ecs.systems.push_back(sys);
}

void update()
{
  for (System* s : core_ecs.systems)
  {
    s->update();
  }
}
}