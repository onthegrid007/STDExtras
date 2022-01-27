#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

namespace std {
    class Semaphore {
        public:
        Semaphore(int64_t i);
        void wait();
        void signal();
        void waitForC(int64_t i);
        void reset();
        void set(int64_t i);
        void decrement();
        int64_t getC();
        ~Semaphore();

        private:
        mutex* m_M;
        condition_variable* m_CV;
        int64_t m_CInit;
        int64_t m_C;
        bool m_isShutdown;
        void Shutdown();
        void ShutdownErrorHandle();
    };
#endif