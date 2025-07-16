#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>

using Entity = unsigned int;

template <typename Component>
class ComponentStorage
{
 public:
  bool has(Entity e) const
  {
    return sparse.find(e) != sparse.end();
  }

  void insert(Entity e, const Component& component)
  {
    if (has(e)) return;
    sparse[e] = dense.size();
    entities.push_back(e);
    dense.push_back(component);
  }

  void remove(Entity e)
  {
    if (!has(e)) return;
    int index = sparse[e];
    int last = entities.size()-1;

    std::swap(entities[index], entities[last]);
    std::swap(dense[index], dense[last]);

    sparse[entities[index]] = index;

    entities.pop_back();
    dense.pop_back();
    sparse.erase(e);
  }

  Component& get(Entity e)
  {
    return dense[sparse[e]];
  }

  const std::vector<Entity>& getEntities() const
  {
    return entities;
  }

 private:
  std::vector<Entity> entities;
  std::vector<Component> dense;
  std::unordered_map<Entity, int> sparse;
};

template<typename ...T>
class View
{
 public:
  View(ComponentStorage<T>&... args) : storages(args...) {}

  std::vector<Entity> getMatching() const
  {
    std::vector<Entity> out;
    const auto& base =  getSmallest();

    for (Entity e : base)
    {
      if ((std::get<ComponentStorage<T>&>(storages).has(e) && ...))
      {
        out.push_back(e);
      }
    }

    return out;
  }

 private:
  std::tuple<ComponentStorage<T>&...> storages;

  const std::vector<Entity>& getSmallest() const
  {
    const std::vector<Entity>* smallest = &std::get<0>(storages).getEntities();
    ((smallest = (std::get<ComponentStorage<T>&>(storages).getEntities().size() < smallest->size() 
                  ? &std::get<ComponentStorage<T>&>(storages).getEntities() : smallest)), ...);

    return *smallest;
  }
};

struct {
  std::unordered_map<std::type_index, void*> storages;
  Entity next_entity = 0;
} registry;

Entity create()
{
  return registry.next_entity++;
}

template<typename T>
ComponentStorage<T>& getStorage()
{
  std::type_index type_in = typeid(T);
  if (!registry.storages.contains(type_in))
  {
    registry.storages[type_in] = new ComponentStorage<T>();
  }
  return *static_cast<ComponentStorage<T>*>(registry.storages[type_in]);
}

template<typename T>
void emplace(Entity e, const T& t)
{
  getStorage<T>().insert(e, t);
}

template<typename T>
T& get(Entity e)
{
  return getStorage<T>().get(e);
}

template<typename T>
bool has(Entity e)
{
  return getStorage<T>().has(e);
}

template<typename ...T>
View<T...> view()
{
  return View<T...>(getStorage<T>()...);
}

struct Position {
  float x;
  float y;
  float z;
};

int main()
{
  Entity e1 = create();
  emplace<Position>(e1, {1.0f, 0.0f, 0.0f});
  View v = view<Position>();
  for (Entity e : v.getMatching())
  {
    auto& pos = get<Position>(e);
    std::cout << pos.x;
  }
}