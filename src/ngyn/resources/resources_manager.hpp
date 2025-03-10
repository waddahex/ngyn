#pragma once

#include "../../ngynpch.hpp"

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
    std::unordered_map<std::string, std::shared_ptr<T>> resources;
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
    static std::weak_ptr<T> addResource(const std::string &name, Args&&... args)
    {
      ResourceStorage<T> &storage = getStorage<T>();

      auto [it, inserted] = storage.resources.emplace(name, std::make_shared<T>(std::forward<Args>(args)...));

      return it->second;
    }

    template<typename T>
    static std::weak_ptr<T> getResource(const std::string &name)
    {
      ResourceStorage<T> &storage = getStorage<T>();
      return storage.resources[name];
    }

    template<typename T>
    static void removeResource(const std::string &name)
    {
      ResourceStorage<T> &storage = getStorage<T>();

      if(storage.resources.find(name) == storage.resources.end())
      {
        return;
      }

      storage.resources[name].reset();
      storage.resources.erase(name);
    }
  };

  inline std::unordered_map<std::type_index, std::shared_ptr<IResourceStorage>> ResourcesManager::storages;
};