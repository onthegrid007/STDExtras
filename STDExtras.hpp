#ifndef STDEXTRAS_H_
#define STDEXTRAS_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <deque>
#include <shared_mutex>
#include <functional>
#include <typeinfo>
#include <sstream>
#include <cmath>
#include <cassert>
#include <iomanip>
#include <condition_variable>
#include <array>
#include <memory>
#include <bit>
#include <type_traits>
#include <algorithm>
#include <endian>
#define UPTR(__T, __Name, ...) std::unique_ptr<__T> __Name{std::make_unique<__T>(__VA_ARGS__)}

#include "vendor/ThreadPool/vendor/Semaphore/vendor/Singleton/inline_abi_macros.h"

namespace std {
	using namespace this_thread;
	typedef unsigned long int ULI;
	typedef signed long int SLI;
	typedef unsigned long long int ULLI;
	typedef signed long long int SLLI;
	typedef long double LD;
	typedef int64_t I64;
	typedef int32_t I32;
	typedef struct null_mutex {
		void lock() const {}
		void unlock() const {}
		bool try_lock() const { return true; }
	} null_mutex;
	typedef lock_guard<null_mutex> NullThreadLock;
	typedef lock_guard<mutex> ThreadLock;
	typedef lock_guard<shared_timed_mutex> ThreadLockS;
	typedef unique_lock<null_mutex> NullCVThreadLock;
	typedef unique_lock<mutex> CVThreadLock;
	typedef unique_lock<shared_timed_mutex> CVThreadLockS;
	typedef thread::id ThreadID;
	/* a=target variable, b=bit number to act upon 0..n */
	#define BIT(x) (decltype(x)(1) << (x))
	#define BIT_SET(a,b) ((a) |= BIT(b))
	#define BIT_CLEAR(a,b) ((a) &= ~BIT(b))
	#define BIT_FLIP(a,b) ((a) ^= BIT(b))
	#define BIT_CHECK(a,b) (!!((a) & BIT(b)))
	#define BYTE 8
	#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))
	static constexpr LD PI = 3.141592653589793;
	template<typename T>
	inline const LD rad2deg(T rad) { return rad * (PI / LD(180)); }
	template<typename T>
	inline const LD deg2rad(T deg) { return deg * (LD(180) / PI); }
	template<typename T0, typename T1, typename T2, typename T3, typename T4>
	inline const LD mapval(T0 value, T1 minIn, T2 maxIn, T3 minOut, T4 maxOut ) { return (((LD)value - (LD)minIn) / ((LD)maxIn - (LD)minIn)) * ((LD)maxOut - (LD)minOut) + (LD)minOut; }
	inline void _yield(I64 ns = 1) { sleep_for(chrono::nanoseconds(ns)); yield(); }
	inline LD randd() { return (LD(rand()) / LD(RAND_MAX)); }
	static inline constexpr bool isLittleEndianS{std::endian::native == std::endian::little};
	// []() constexpr->bool{
		// struct ET {
		// 	union {
		// 		const I32 a;
		// 		const char b[sizeof(I32)];
		// 	};
		// 	constexpr ET(const I32& _a) : a(_a) {}
		// };
		// // typedef  ET;
		// static constexpr I32 V{0x00000001};
		// static constexpr ET E{V};
		// return !(E.b[0]);
	// }()};
	
	// template<typename T>
	// inline constexpr T PowerUpVal(const T v1, const T v2) {
	// 	return v1 * v2;
	// }

	// template<typename T, short N, T LT, T... Vals>
	// ALWAYS_INLINE constexpr typename std::enable_if<(N == sizeof...(Vals)), std::array<T, N>>::type MakePowerUpArr() {
	// 	return std::array<T, N>{{Vals...}};
	// }

	// template<typename T, short N, T... Vals>
	// ALWAYS_INLINE constexpr typename std::enable_if<(sizeof...(Vals) > 0), std::array<T, N>>::type MakePowerUpArr() {
	// 	constexpr std::initializer_list<T> L{Vals...};
	// 	return MakePowerUpArr<T, N, PowerUpVal(*(L.begin() + (L.size() - 1)), *(L.begin() + (L.size() - 1)), Vals..., PowerUpVal(*(L.begin() + (L.size() - 1)), *(L.begin() + (L.size() - 1)))>();
	// }
	
	// template<typename T, short N, T LT, T... Vals>
	// ALWAYS_INLINE constexpr typename std::enable_if<(N != sizeof...(Vals)), std::array<T, N>>::type MakePowerUpArr() {
	// 	constexpr std::initializer_list<T> L{Vals...};
	// 	return MakePowerUpArr<T, N, PowerUpVal(*(L.begin() + (L.size() - 1)), *(L.begin() + (L.size() - 1)), Vals..., PowerUpVal(*(L.begin() + (L.size() - 1)), *(L.begin() + (L.size() - 1)))>();
	// }
	
	template<typename T, short N>
	constexpr std::array<T, N> GenCExprArr(const std::function<const T(const std::array<T, N>&, const short)>&& _callback) {
		std::array<T, N> arr{};
		for(short i = 0; i < N; i++)
			arr[i] = _callback(arr, i);
		return arr;
	};
	
	// #define _TSIZE 128
	// static constexpr std::array<ULLI, _TSIZE> PTPTable = []() constexpr->std::array<ULLI, _TSIZE>{
	// 	std::array<ULLI, _TSIZE> rtn{};
	// 	rtn[0] = 10;
	// 	for(ULLI i = 1; i < short(_TSIZE); i++) { rtn[i] = (ULLI(10) * rtn[i - 1]); }
	// 	return rtn;
	// };
	
	// template<typename T>
	// constexpr char log10_cxpr(const T& num) {
	// 	for(char i = 0; i < short(_TSIZE); i++) { if(PTPTable[i] > num) return i; }
	// 	return 0;
	// }
	
	template<size_t N>
    struct rValStr {
        char data[N];
    };
	
    template<size_t N, size_t K>
    inline constexpr auto RemoveStringContents(const char(&expr)[N], const char(&remove)[K]) {
        rValStr<N> result = {};
        size_t srcIdx = 0;
        size_t dstIdx = 0;
        while(srcIdx < N) {
            size_t matchIdx = 0;
            while(matchIdx < K - 1 && srcIdx + matchIdx < N - 1 && expr[srcIdx + matchIdx] == remove[matchIdx])
                matchIdx++;
            if(matchIdx == (K - 1))
                srcIdx += matchIdx;
            result.data[(dstIdx += 1)] = expr[srcIdx] == '"' ? '\'' : expr[srcIdx];
            srcIdx++;
        }
        return result;
    }
	
	template<class Tp>
	inline volatile typename enable_if<is_trivially_copyable<Tp>::value && (sizeof(Tp) <= sizeof(Tp*)), void>::type DoNotOptimize(Tp& value) {
		asm volatile("" : "+r"(&value) : : "memory");
	}
	
	template<class Tp>
	inline volatile typename enable_if<!std::is_trivially_copyable<Tp>::value || (sizeof(Tp) > sizeof(Tp*)), void>::type DoNotOptimize(Tp& value) {
		asm volatile("" : "+m"(&value) : : "memory");
	}
	
	#ifdef USING_BAD_LOG
		#define BADLOG(x) std::cout << (x) << std::endl;
		#define BADLOGV(x) BADLOG(#x << ":") BADLOG((x) << std::endl)
	#else
		#define BADLOG(x)
		#define BADLOGV(x)
	#endif
	
	inline void toLowercase(string& str) {
		for_each(str.begin(), str.end(), [](char& c) {
			c = tolower(c);
		});
	}
	
	inline string toLowercaseRtn(string str) {
		toLowercase(str);
		return str;
	}
	
	inline void toUppercase(string& str) {
		for_each(str.begin(), str.end(), [](char& c) {
			c = toupper(c);
		});
	}
	
	inline string toUpperrcaseRrn(string str) {
		toUppercase(str);
		return str;
	}
	
	template<template<typename> typename VT, typename T>
	inline const LD vecAvg(const VT<T>& vec) {
		LD sum = 0;
		for(const T& val : vec) sum += val;
		return sum / (LD)vec.size();
	}
	
	// template<typename T> requires is_type<T, bool>::value
	// inline const string toString(T a) {
	// 	return (a ? "true" : "false");
	// }
	
	// template<typename T> requires !is_type<T, bool>::value
	// inline const string toString(T a) {
	// 	stringstream ss;
	// 	ss << a;
	// 	return ss.str();
    // }
	
	template<typename T>
	inline const string toStringPrecision(T a, const char precision) {
		// { if(typeid(T) == typeid(bool)) { return (a ? "true" : "false"); } }
		stringstream ss1;
		stringstream ss2;
		ss1 << setprecision(16) << fixed << a;
		ss2 << ULLI(a);
		string s1 = ss1.str();
		string s2 = ss2.str();
		if((s2.size() >= (int)precision) || (s1.substr(0, precision).size() == s2.size() + 1))
			return s2;
		return s1.substr(0, precision);
	}
	
	template<template<typename> typename VT, typename T>
	inline void writeVectorFile(const string filepath, VT<T>& vec, const bool append = false) {
		const ULLI vSize = vec.size();
		UPTR(ofstream, f, filepath, ios::binary);
		if(!append) f.get()->clear();
		f.get()->write(reinterpret_cast<const char*>(&vSize), sizeof(ULLI));
		for(ULLI i = 0; i < vSize; i++) f.get()->write(reinterpret_cast<char*>(&vec[i]), sizeof(T));
	}
	
	template<template<typename> typename VT, typename T>
	inline void readVectorFile(string filepath, VT<T>& vec, const bool append = false, const ULLI beginIdx = 0, const ULLI endIdx = 0) {
		ULLI vSize = 0;
		UPTR(ifstream, f, filepath, ios::binary);
		if(!(f.get())) return;
		f.get()->read(reinterpret_cast<char*>(&vSize), sizeof(ULLI));
		const ULLI readlength = ((endIdx != 0) ? (endIdx < vSize ? endIdx : vSize) : vSize) - beginIdx;
		if(readlength < 1) return;
		if(beginIdx != 0) f.get()->seekg(sizeof(ULLI) + (beginIdx * sizeof(T)));
		const ULLI currentSize = append ? vec.size() : 0;
		append ? vec.resize(currentSize + readlength) : (vec.clear() && vec.resize(readlength));
		for(ULLI i = 0; i < readlength; i++) f.get()->read(reinterpret_cast<char*>(&vec[currentSize + i]), sizeof(T));
	}
}
#endif
	/*
	ALWAYS_INLINE void intervalThread(function<void()> begin, function<void()> loop, function<void()> stop, function<bool()> exitCondition, int64_t Interval, vector<int64_t>* LoopTimes, bool debug = false) {
		thread([Name, Start, Loop, Exit, ExitCondition, Interval, LoopTimes, debug] {
			SAClock threadTimer;
			int64_t sleep;
			mpcout << "Starting " << Name << " Thread!\n";
			Start();
			while(!(ExitCondition())) {
				{
					threadTimer.tare();
					Loop();
					sleep = Interval - threadTimer.elapsed();
				}
				if((sleep < 0) && (debug == true)) {
					mpcout << Name << " Thread Overrun: " << -sleep << " nanoseconds!\n";
				}
				__sleep_for(seconds(0), nanoseconds(sleep));
				if(LoopTimes != nullptr) {
					if(LoopTimes->size() > 99)
						LoopTimes->erase(LoopTimes->begin());
					if(Interval > 1) {
						LoopTimes->emplace_back(Interval);
					}
					else {
						LoopTimes->emplace_back(threadTimer.elapsed());
					}
				}
			}
			mpcout << "Stopping " << Name << " Thread!\n";
			Exit();
		}).detach();
	}
	
	ALWAYS_INLINE void intervalFunction(string Name, function<void()> Start, function<void()> Loop, function<void()> Exit, function<bool()> ExitCondition, int64_t Interval, vector<int64_t>* LoopTimes, bool debug = false) {
		SAClock threadTimer;
		int64_t sleep;
		mpcout << "Starting " << Name << " Function!\n";
		Start();
		while(!(ExitCondition())) {
			{
				threadTimer.tare();
				Loop();
				sleep = Interval - threadTimer.elapsed();
			}
			if((sleep < 0) && (debug == true)) {
				mpcout << Name << " Function Overrun: " << -sleep << " nanoseconds!\n";
			}
			__sleep_for(seconds(0), nanoseconds(sleep));
			if(LoopTimes != nullptr) {
				if(LoopTimes->size() > 99)
					LoopTimes->erase(LoopTimes->begin());
				if(Interval > 1) {
					LoopTimes->emplace_back(Interval);
				}
				else {
					LoopTimes->emplace_back(threadTimer.elapsed());
				}
			}
		}
		mpcout << "Stopping " << Name << " Function!\n";
		Exit();
	}
	ALWAYS_INLINE vec3 RGBtoHSV(vec3 rgb) {
		vec3 hsv;
		float fCMax = max(max(rgb.x, rgb.y), rgb.z);
		float fCMin = min(min(rgb.x, rgb.y), rgb.z);
		float fDelta = fCMax - fCMin;

		if(fDelta > 0) {
			if(fCMax == rgb.x) {
			  hsv.x = 60 * (fmod(((rgb.y - rgb.z) / fDelta), 6));
			} else if(fCMax == rgb.y) {
			  hsv.x = 60 * (((rgb.z - rgb.x) / fDelta) + 2);
			} else if(fCMax == rgb.z) {
			  hsv.x = 60 * (((rgb.x - rgb.y) / fDelta) + 4);
			}

			if(fCMax > 0) {
			  hsv.y = fDelta / fCMax;
			} else {
			  hsv.y = 0;
			}

			hsv.z = fCMax;
		} else {
			hsv.x = 0;
			hsv.y = 0;
			hsv.z = fCMax;
		}

		if(hsv.x < 0) {
		hsv.x = 360 + hsv.x;
		}
		return hsv;
	}
*/