#ifndef SINGLETON_HPP_
#define SINGLETON_HPP_

#include "noncopyable.h"
#include "nonmoveable.h"

namespace std {
    template<typename T>
    class Singleton : public NonMovable, public NonCopyable {
        public:
        static T& GetInstance() { return Instance; }

        private:
        static T Instance;
    };
}
#endif