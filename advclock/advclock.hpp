#ifndef ADVCLOCK_HPP_
#define ADVCLOCK_HPP_

#include <chrono>

namespace std {
  using namespace chrono;
  class ADVClock {
    public:
    ADVClock() :
      m_begin(std::steady_clock::now()) {}
    template<typename rtnType, typename _T1, typename _T2>
    rtnType begin(duration<_T1, _T2> timestepType) {
      return duration_cast<timestepType>(m_begin - GLOBAL_CLOCK.m_begin).count();
    }
    template<typename rtnType, typename _T1, typename _T2>
    rtnType now(duration<_T1, _T2> timestepType) {
      return duration_cast<timestepType>(steady_clock::now() - GLOBAL_CLOCK.m_begin).count();
    }
    template<typename rtnType>
    rtnType elapsedRaw(bool tareClock = false) {
      rtnType rtn = now<nanoseconds, int64_t>() - begin<nanoseconds, int64_t>();
      tareClock ? tare() : void(0);
      return rtn;
    }
    template<typename rtnType, typename _T1, typename _T2>
    rtnType elapsed(duration<_T1, _T2> timestepType, bool tareClock = false) {
      return duration_cast<timestepType>(nanoseconds(int64_t(elapsedRaw(tareClock)))).count();
    }
    void tare() { m_begin = steady_clock::now(); }
    ~ADVClock();

    private:
    time_point<steady_clock> m_begin;
    static const ADVClock GLOBAL_CLOCK;
  };
  const ADVClock ADVClock::GLOBAL_CLOCK;
}
#endif
