#ifndef ADVCLOCK_HPP_
#define ADVCLOCK_HPP_

#include <chrono>

namespace std {
  using namespace chrono;
  class ADVClock {
    public:
    #ifdef ADVCLOCKTYPE_OVERRIDE
    typedef ADVCLOCKTYPE_OVERRIDE ADVClockType;
    #else
    typedef high_resolution_clock ADVClockType;
    #endif
  
    enum Precision : char {
      Nanoseconds = 0,
      Microseconds = 1,
      Milliseconds = 2,
      Seconds = 3,
      Minutes = 4,
      Hours = 5,
      Days = 6,
      #if _HAS_CXX20
        Weeks = 7,
        Months = 8,
        Years = 9
      #endif
    };

    ADVClock() :
      m_begin(ADVClockType::now()) {}

    template<typename rtnType, typename castType>
    static rtnType RuntimeCast(castType cast, Precision precision) {
      switch((int)precision) {
        case Precision::Nanoseconds:
        return rtnType(duration_cast<nanoseconds>(cast).count());
        case Precision::Microseconds:
        return rtnType(duration_cast<microseconds>(cast).count());
        case Precision::Milliseconds:
        return rtnType(duration_cast<milliseconds>(cast).count());
        case Precision::Seconds:
        return rtnType(duration_cast<seconds>(cast).count());
        case Precision::Minutes:
        return rtnType(duration_cast<minutes>(cast).count());
        case Precision::Hours:
        return rtnType(duration_cast<hours>(cast).count());
        #if _HAS_CXX20
          case Precision::Days:
          return rtnType(duration_cast<days>(cast).count());
          case Precision::Weeks:
          return rtnType(duration_cast<weeks>(cast).count());
          case Precision::Months:
          return rtnType(duration_cast<months>(cast).count());
          case Precision::Years:
          return rtnType(duration_cast<years>(cast).count());
        #endif
        default:
        return 0;
      };
    }

    template<typename rtnType>
    rtnType begin(Precision precision = Precision::Nanoseconds) {
        return RuntimeCast<rtnType>(m_begin - GLOBAL_CLOCK.m_begin, precision); 
    }

    template<typename rtnType>
    rtnType now(Precision precision = Precision::Nanoseconds) {
        return RuntimeCast<rtnType>(ADVClockType::now() - GLOBAL_CLOCK.m_begin, precision); 
    }

    int64_t elapsedRawNano(bool tareClock = false) {
      int64_t rtn = now<int64_t>() - begin<int64_t>();
      tareClock ? tare() : void(0);
      return rtn;
    }

    template<typename rtnType>
    rtnType elapsed(Precision precision = Precision::Nanoseconds, bool tareClock = false) {
        return RuntimeCast<rtnType>(nanoseconds(elapsedRawNano(tareClock)), precision); 
    }

    void tare() { m_begin = ADVClockType::now(); }

    ~ADVClock() {}

    private:
    time_point<ADVClockType> m_begin;
    static const ADVClock GLOBAL_CLOCK;
  };
  const ADVClock ADVClock::GLOBAL_CLOCK;
}
#endif