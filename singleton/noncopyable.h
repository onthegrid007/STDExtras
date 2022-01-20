#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_
namespace std {
    class NonCopyable {
        public:
        NonCopyable() = default;
        NonCopyable(const NonCopyable &) = delete;
        NonCopyable &operator=(const NonCopyable &) = delete;
    };
};
#endif