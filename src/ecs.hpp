#ifndef MOCHAECS_HPP
#define MOCHAECS_HPP

#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <any>

namespace mocha::ecs
{
using Entity = unsigned int;

struct System {
  virtual void update() = 0;
};

struct Command {
  std::function<void(Entity&)> use;
  Command(std::function<void(Entity&)> u) : use(u) {};
};

struct IStorage {
  virtual void remove(Entity) = 0;
  virtual bool has(Entity) = 0;
};

template <typename Component>
class ComponentSet : public IStorage
{
 public:
  bool has(Entity e)
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

struct Core_ecs {
  std::unordered_map<std::type_index, void*> sets;
  Entity next_entity = 0;
  std::vector<System*> systems;
};

extern Core_ecs core_ecs;



template<typename Component>
ComponentSet<Component>& getSet()
{
  std::type_index t = typeid(Component);
  if (!core_ecs.sets.contains(t))
  {
    core_ecs.sets[t] = new ComponentSet<Component>();
  }

  return *static_cast<ComponentSet<Component>*>(core_ecs.sets[t]);
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

Entity create();
void remove(Entity e);
void addSystem(System *sys);
void update();


}

#endif