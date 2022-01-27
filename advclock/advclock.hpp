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
      Weeks = 7,
      Months = 8,
      Years = 9
    };

    #define SecsInMin 60
    #define MinsInHour 60
    #define HoursInDay 24
    #define DaysInWeek 7
    #define DaysInYear 365.24
    #define MonthsInYear 12

    ADVClock() :
      m_begin(ADVClockType::now()) { }
      
    template<typename rtnType = double, typename castType>
    static rtnType RuntimeCast(castType cast, Precision precision = Precision::Nanoseconds) {
        return RuntimeCastFromNano(duration_cast<nanoseconds>(cast), precision);
    }

    template<typename rtnType = double>
    static rtnType RuntimeCastFromNano(nanoseconds fromNanos, Precision precision = Precision::Nanoseconds) {
      #define divb1k(x) (x / rtnType(1000.0))
      #define nanos int64_t(fromNanos.count())
      #define micros divb1k(nanos)
      #define millis divb1k(micros)
      #define secs divb1k(millis)
      #define mins (secs / SecsInMin)
      #define hours (mins / MinsInHour)
      #define days (hours / HoursInDay)
      #define weeks (days / DaysInWeek)
      #define years (days / DaysInYear)
      #define months (years / MonthsInYear)

      switch((int)precision) {
        case Precision::Nanoseconds:
        return nanos;
        case Precision::Microseconds:
        return micros;
        case Precision::Milliseconds:
        return millis;
        case Precision::Seconds:
        return secs;
        case Precision::Minutes:
        return mins;
        case Precision::Hours:
        return hours;
        case Precision::Days:
        return days;
        case Precision::Weeks:
        return weeks;
        case Precision::Months:
        return months;
        case Precision::Years:
        return years;
        default:
        return nanos;
      };
    }

    nanoseconds beginDur() {
        return m_begin - GLOBAL_CLOCK.m_begin; 
    }
    
    template<typename rtnType>
    rtnType begin(Precision precision = Precision::Nanoseconds) {
        return RuntimeCast<rtnType>(beginDur(), Precision::Nanoseconds);
    }
    
    nanoseconds nowDur() {
        return ADVClockType::now() - GLOBAL_CLOCK.m_begin; 
    }
    
    template<typename rtnType>
    rtnType now(Precision precision = Precision::Nanoseconds) {
        return RuntimeCast<rtnType>(nowDur(), Precision::Nanoseconds);
    }

    nanoseconds elapsedRawNanoDur(bool tareClock = false) {
      auto rtn = nowDur() - beginDur();
      tareClock ? tare() : void(0);
      return rtn;
    }
    
    int64_t elapsedRawNano(bool tareClock = false) {
      int64_t rtn = elapsedRawNanoDur().count();
      tareClock ? tare() : void(0);
      return rtn;
    }

    template<typename rtnType>
    rtnType elapsed(Precision precision = Precision::Nanoseconds, bool tareClock = false) {
        return RuntimeCast<rtnType>(elapsedRawNanoDur(tareClock), precision);
    }

    void tare() { m_begin = ADVClockType::now(); }

    ~ADVClock() {}

    private:
    time_point<ADVClockType> m_begin;
    static const ADVClock GLOBAL_CLOCK;
  };
  inline const ADVClock ADVClock::GLOBAL_CLOCK;
}
#endif

// Example Usage

/*
void doStuff() {
    double sum = 0;
    for(int i = 0; i < 999; i++) {
        for(int j = 0; j < 999; j++) {
            sum += i * j + i;   
        }
    }
}

#include <iostream>

int main() {
    using namespace std;
    using namespace chrono;
    nanoseconds nanosecs;
{
    ADVClock c;
    doStuff();
    nanosecs = c.elapsedRawNanoDur();
}
    std::cout << "Clock took:\n" <<
        nanosecs.count() << " nanos\n" <<
        ADVClock::RuntimeCast<double>(nanosecs, ADVClock::Precision::Microseconds) << " micros\n" <<
        ADVClock::RuntimeCast<double>(nanosecs, ADVClock::Precision::Milliseconds) << " millis\n" <<
        ADVClock::RuntimeCast<double>(nanosecs, ADVClock::Precision::Seconds) << " secs\n" <<
        ADVClock::RuntimeCast<double>(nanosecs, ADVClock::Precision::Minutes) << " mins\n" <<
        ADVClock::RuntimeCast<double>(nanosecs, ADVClock::Precision::Hours) << " hours\n" <<
        ADVClock::RuntimeCast<double>(nanosecs, ADVClock::Precision::Days) << " days\n" <<
        ADVClock::RuntimeCast<double>(nanosecs, ADVClock::Precision::Weeks) << " weeks\n" <<
        ADVClock::RuntimeCast<double>(nanosecs, ADVClock::Precision::Years) << " years\n";
    return 0;
}
*/