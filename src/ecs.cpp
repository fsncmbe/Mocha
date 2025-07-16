#define MOCHA_ECS

#include <mocha.hpp>
#include <utils.hpp>
#include <core.hpp>

namespace mocha
{

namespace 
{

struct IStorage {
  virtual void remove(Entity) = 0;
  virtual bool has(Entity) = 0;
};

template <typename Component>
class ComponentSet : IStorage
{
 public:
  bool has(Entity e) const
  {
    return connection.find(e) != connection.end();
  }

  void insert(Entity e, const Component& c)
  {
    if (has(e)) return;
    connection[e] = components.size();
    entities.push_back(e);
    components.push_back(c);
  }

  void remove(Entity e)
  {
    if (!has(e)) return;
    int index = connection[e];
    int last = entities.size()-1;

    std::swap(entities[index], entities[last]);
    std::swap(components[index], components[last]);

    connection[entities[index]] = index;

    entities.pop_back();
    components.pop_back();
    connection.erase(e);
  }

  Component& get(Entity e)
  {
    return components[connection[e]];
  }

  const std::vector<Entity>& getEntities()
  {
    return entities;
  }

 private:
  std::vector<Entity>             entities;
  std::vector<Component>          components;
  std::unordered_map<Entity, int> connection;
};

template<typename ...Component>
class View
{
 public:
  View(ComponentSet<Component>&... args) : sets(args...) {}

  std::vector<Entity> getMatching()
  {
    std::vector<Entity> out;
    const auto& base = getSmallest();

    for (Entity e : base)
    {
      if ((std::get<ComponentSet<Component>&>(sets).has(e) && ...))
      {
        out.push_back(e);
      }
    }

    return out;
  }

 private:
  std::tuple<ComponentSet<Component>&...> sets;

  const std::vector<Entity>& getSmallest() const
  {
    const std::vector<Entity>* smallest = &std::get<0>(sets).getEntities();
    ((smallest = (std::get<ComponentSet<Component>&>(sets).getEntities().size() < smallest->size() 
                  ? &std::get<ComponentSet<Component>&>(sets).getEntities() : smallest)), ...);

    return *smallest;
  }
};

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

} // private namespace

namespace ecs {

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

void update()
{
  for (System* s : core.ecs.systems)
  {
    s->update();
  }
}

} // ecs
} // mocha
