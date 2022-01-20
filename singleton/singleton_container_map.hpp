#ifndef SINGLETON_CONTAINER_MAP_HPP_
#define SINGLETON_CONTAINER_MAP_HPP_
#include "noncopyable.h"
#include "nonmoveable.h"

#include <string>
#include <unordered_map>
#include <tuple>
#include <assert.h>

#define DEFAULT_IF_NULL_ARGS(...) bool(#__VA_ARGS__) ? __VA_ARGS__ ? void(0)

template<typename T>
class SingletonContainerMap : public std::NonMovable, public std::NonCopyable {
    private:
    static std::unordered_map<std::string, T*> ContainerMap;
    friend class std::pair<std::basic_string<char>, T*>;
    static bool exists(const std::string& key) { return (ContainerMap.find(key) != ContainerMap.end()); }

    public:
    template<typename... Args>
    static T& CreateNewInstance(const std::string key, Args& ... args) {
        ContainerMap[key] = std::move(new T(args...));
        return *(ContainerMap[key]);
    }
    template<typename... Args>
    static T& CreateNewInstance(const std::string key, Args&& ... args = DEFAULT_IF_NULL_ARGS(args...)) {
        return CreateNewInstance(key, args...);
    }

    template<typename... Args>
    static T& GetInstanceByKey(const std::string key, Args& ... args) {
        if(exists(key)) {
            return *(ContainerMap[key]);
        }
        assert(false);
    }
    template<typename... Args = void>
    static T& GetInstanceByKey(const std::string key, Args&& ... args) {
        return GetInstanceByKey(key, args...)
    }

    static const void DeleteInstanceByKey(const std::string key) {
        if(exists(key)) {
            return ContainerMap.erase(key);
        }
    }
    static const std::string GetKeyByInstance(const T& instance) {
        for(auto& pair = ContainerMap.begin(); pair != ContainerMap.end(); pair++)
            if(pair[1] == &instance) { return pair[0]; }
    }

    static const void DeleteInstanceByInstance(const T& instance) {
        return DeleteInstanceByKey(GetKeyByInstance(instance));
    }

    template<typename getType, typename rtnType, typename... Args>
    static rtnType& Get(const getType _getObj, Args& ... args = void(0)) {
        return (typeid(std::basic_string<char>) == typeid(getType)) ? GetInstanceByKey(_getObj, args...) : GetKeyByInstance(_getObj);
        // return (typeid(T) == typeid(getType)) ? GetKeyByInstance(_getObj) : GetInstanceByKey(_getObj);
        // Also works, but std::basic_string<char> I trust more than a User Def'ed T...
    }

    template<typename delType>
    static const void Del(delType _delObj) {
        return (typeid(std::basic_string<char>) == typeid(delType)) ? DeleteInstanceByKey(_delObj) : DeleteInstanceByInstance(_delObj);
    }
};

#define _SCM_DEFINITIONS(T) template<> std::unordered_map<std::string, T*> SingletonContainerMap<T>::ContainerMap; using T::SingletonContainerMap<T> _SCM;

#endif