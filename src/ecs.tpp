#ifndef MOCHAECS_TPP
#define MOCHAECS_TPP

#include <mocha.hpp>
#include <core.hpp>


namespace mocha::ecs
{

template<typename Component>
ComponentSet<Component>& getSet()
{
  std::type_index t = typeid(Component);
  if (!core.ecs.sets.contains(t))
  {
    core.ecs.sets[t] = new ComponentSet<Component>();
  }

  return *static_cast<ComponentSet<Component>*>(core.ecs.sets[t]);
}

template<typename Component>
void emplace(Entity e, const Component& c)
{
  getSet<Component>().insert(e, c);
}

template<typename Component>
Component get(Entity e)
{
  return getSet<Component>().get(e);
}

template<typename Component>
bool has(Entity e)
{
  return getSet<Component>().has(e);
}

template<typename ...Component>
std::vector<Entity> view()
{
  return View<Component...>(getSet<Component>()...).getMatching();
}

}

#endif