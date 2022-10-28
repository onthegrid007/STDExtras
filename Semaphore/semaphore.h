#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "../vendor/Singleton/noncopyable.h"
#include "../vendor/Singleton/nonmoveable.h"
#include <condition_variable>
#include <functional>

class Semaphore : public NonCopyable, public NonMovable {
    public:
    Semaphore(int64_t i = 0);
    ~Semaphore();
    void inc(int64_t i = 1);
    void dec(int64_t i = 1);
    void notify();
    void waitFor(std::function<bool(const int64_t cVal, const int64_t CInitVal)>&& retBool);
    void waitForI(int64_t i);
    void wait();
    void reset();
    void set(int64_t i);
    void decrement();
    operator int64_t();
    int64_t operator+(const Semaphore& other);
    int64_t operator-(const Semaphore& other);
    int64_t operator+=(const int64_t i);
    int64_t operator-=(const int64_t i);

    private:
    std::mutex m_M;
    std::condition_variable m_CV;
    int64_t m_CInit;
    int64_t m_C;
};
#endif