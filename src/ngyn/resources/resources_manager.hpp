#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>

namespace ngyn
{
  class IResourceStorage
  {
    public:
    IResourceStorage(){};
    ~IResourceStorage(){};
  };

  template<typename T>
  class ResourceStorage : public IResourceStorage
  {
    public:
    std::unordered_map<std::string, T> resources;
  };

  class ResourcesManager
  {
    public:
    static std::unordered_map<std::type_index, std::shared_ptr<IResourceStorage>> storages;

    template<typename T>
    static ResourceStorage<T>& getStorage()
    {
      std::type_index typeId = std::type_index(typeid(T));

      if(storages.find(typeId) == storages.end())
      {
        storages[typeId] = std::make_shared<ResourceStorage<T>>();
      }

      return *static_cast<ResourceStorage<T>*>(storages[typeId].get());
    }

    template<typename T, typename... Args>
    static T* addResource(const std::string &name, Args&&... args)
    {
      ResourceStorage<T> &storage = getStorage<T>();

      // Do something about adding the same resource, just return it?
      auto [it, inserted] = storage.resources.emplace(name, T(std::forward<Args>(args)...));

      return &it->second;
    }

    template<typename T>
    static T* getResource(const std::string &name)
    {
      ResourceStorage<T> &storage = getStorage<T>();

      if(storage.resources.find(name) == storage.resources.end())
      {
        return nullptr;
      }

      return &storage.resources[name];
    }
  };

  inline std::unordered_map<std::type_index, std::shared_ptr<IResourceStorage>> ResourcesManager::storages;
};