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
      components[core.ecs.components[std::type_index(typeid(i.type()))]] = i;
    }
  }
};

void registerComp(std::type_index t, const std::string& name)
{
  core.ecs.components[t] = name;
}

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

void updateSystems()
{
  for (System s : core.ecs.sytems)
  {
    s.update(core.window.delta);
  }
}
}