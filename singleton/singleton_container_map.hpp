#ifndef SINGLETON_CONTAINER_MAP_HPP_
#define SINGLETON_CONTAINER_MAP_HPP_
#include "noncopyable.h"
#include "nonmoveable.h"

#include <string>
#include <unordered_map>

namespace std {
	template<typename T>
    class SingletonContainerMap : public NonMovable, public NonCopyable {
        public:
        template<typename... Args>
        static T& CreateNewInstance(string key, Args&& ... args) {
            ContainerMap[key] = move(T(args));
            return ContainerMap[key];
        }
        static T& GetInstanceByKey(string key) {
            if(exists(key)( {
                return ContainerMap[key];
            }
            else {
                //Handle error
                assert(false);
            }
        }
        static const void DeleteInstanceByKey(string key) {
            if(exists(key)) {
                ContainerMap[key].Cleanup();
                ContainerMap.erase(key);
                return;
            }
            else {
                // Handle Error
            }
        }
        private:
        static unordered_map<string, T> ContainerMap;
        friend class pair<basic_string, T>;
        static bool exists(const string& key) { return (ContainerMap.find(key) != ContainerMap.end()); }
        virtual void Cleanup() = 0;
    };
}
#endif