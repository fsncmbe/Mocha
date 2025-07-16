#define MOCHA_ECS

#include <mocha.hpp>
#include <utils.hpp>
#include <core.hpp>

namespace mocha
{
ComponentMap::ComponentMap(const std::vector<std::any>& vec)
{
  if (!vec.empty())
  {
    for (auto i : vec)
    {
      components[typeid(i.type()).name()] = i;
    }
  }
};

Entity* addEntity()
{
  core.ecs.entites.push_back({});
  return &core.ecs.entites.back();
}

Entity* addEntity(Entity e)
{
  core.ecs.entites.push_back(e);
  return &core.ecs.entites.back();
}

void delEntity(Entity e)
{
  auto it = std::find(core.ecs.entites.begin(), core.ecs.entites.end(), e);

  if (it != core.ecs.entites.end())
  {
    core.ecs.entites.erase(it);
  }
}

void clearEntities()
{
  core.ecs.entites.clear();
}

void updateSystems()
{
  for (ISystem<std::any>* s : core.ecs.sytems)
  {
    s->update(core.window.delta);
  }
}

}