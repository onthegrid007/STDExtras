#ifndef MEMTRACK_HPP_
#define MEMTRACK_HPP_
#include "../Semaphore/semaphore.h"

namespace MemTrack {
    namespace Global {
        namespace Heap {
            inline static std::unique_ptr<Semaphore> AllocatedBytes = std::make_unique<Semaphore>(0);
            inline static std::unique_ptr<Semaphore> DeallocatedBytes = std::make_unique<Semaphore>(0);
            inline static const int64_t TotalActiveMemory() {
                return AllocatedBytes.get() - DeallocatedBytes.get();
            }
        }
        namespace Stack {
            inline static std::unique_ptr<Semaphore> AllocatedBytes = std::make_unique<Semaphore>(sizeof(Semaphore) * 4);
            inline static std::unique_ptr<Semaphore> DeallocatedBytes = std::make_unique<Semaphore>(0);
            inline static const int64_t TotalActiveMemory() {
                return AllocatedBytes.get() - DeallocatedBytes.get();
            }
        }
        inline static const int64_t TotalAllocated() {
            return *Heap::AllocatedBytes.get() + *Stack::AllocatedBytes.get();
        }
        inline static const int64_t TotalDeallocated() {
            return *Heap::DeallocatedBytes.get() + *Stack::DeallocatedBytes.get();
        }
        inline static const int64_t TotalActiveMemory() {
            return TotalAllocated() - TotalDeallocated();
        }
    }
    
    template<typename T>
    class TrackMemory {
        public:
        class Heap {
            public:
            inline static std::unique_ptr<Semaphore> AllocatedBytes = std::make_unique<Semaphore>(0);
            inline static std::unique_ptr<Semaphore> DeallocatedBytes = std::make_unique<Semaphore>(0);
            inline static const int64_t TotalActiveMemory() {
                return AllocatedBytes.get() - DeallocatedBytes.get();
            }
            inline static void* Allocate(const size_t _s) {
                void* rtn = malloc(_s);
                if(!rtn) return rtn;
                *TrackMemory<T>::Heap::AllocatedBytes.get() += _s;
                *Global::Heap::AllocatedBytes.get() += _s;
                return rtn;
            }
            inline static void* Allocate() {
                return Allocate(sizeof(T));
            }
            inline static void Deallocate(void* p, const size_t _s) {
                if(!p || !_s) return;
                *TrackMemory<T>::Heap::DeallocatedBytes.get() += _s;
                *Global::Heap::DeallocatedBytes.get() += _s;
                if(_s != sizeof(T)) {
                    free(p);
                    return;
                }
                T* del = (T*)p;
                free(del);
            }
            inline static void Deallocate(T* t) {
                Deallocate(t, sizeof(T));
            }
        };
        class Stack {
            public:
            inline static std::unique_ptr<Semaphore> AllocatedBytes = std::make_unique<Semaphore>(sizeof(Semaphore) * 4);
            inline static std::unique_ptr<Semaphore> DeallocatedBytes = std::make_unique<Semaphore>(0);
            inline static const int64_t TotalActiveMemory() {
                return AllocatedBytes.get() - DeallocatedBytes.get();
            }
            inline static void Allocate(const size_t _s) {
                if(!_s) return;
                *TrackMemory<T>::Stack::AllocatedBytes.get() += _s;
                *Global::Stack::AllocatedBytes.get() += _s;
            }
            inline static void Allocate() {
                Allocate(sizeof(T));
            }
            inline static void Deallocate(const size_t _s) {
                if(!_s) return;
                *TrackMemory<T>::Stack::DeallocatedBytes.get() += _s;
                *Global::Stack::DeallocatedBytes.get() += _s;
            }
            inline static void Deallocate() {
                Deallocate(sizeof(T));
            }
        };
    };
}
#endif