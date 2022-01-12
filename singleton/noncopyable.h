#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_
namespace std {
    class NonCopyable {
        NonCopyable() = default;
        NonCopyable(const NonCopyable &) = delete;
        NonCopyable &operator=(const NonCopyable &) = delete;
    };
}
#endif