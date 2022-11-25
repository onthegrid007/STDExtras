// #pragma once
#ifndef STDEXTRAS_H_
#define STDEXTRAS_H_
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <typeinfo>
#include <sstream>
#include <cmath>
#include <cassert>
#include <iomanip>
#include <condition_variable>
#include "vendor/ThreadPool/vendor/Semaphore/vendor/Singleton/inline_abi_macros.h"

namespace std {
	// using namespace chrono;
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
	typedef lock_guard<shared_mutex> ThreadLockS;
	typedef unique_lock<null_mutex> NullCVThreadLock;
	typedef unique_lock<mutex> CVThreadLock;
	typedef unique_lock<shared_mutex> CVThreadLockS;
	typedef thread::id ThreadID;
	/* a=target variable, b=bit number to act upon 0..n */
	#define BIT(x) (decltype(x)(1) << (x))
	#define BIT_SET(a,b) ((a) |= BIT(b))
	#define BIT_CLEAR(a,b) ((a) &= ~BIT(b))
	#define BIT_FLIP(a,b) ((a) ^= BIT(b))
	#define BIT_CHECK(a,b) (!!((a) & BIT(b)))
	#define BYTE 8
	#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))
	constexpr static LD PI = 3.141592653589793;
	template<typename T>
	ALWAYS_INLINE LD rad2deg(T rad) { return rad * (PI / (LD)180.0); }
	template<typename T>
	ALWAYS_INLINE LD deg2rad(T deg) { return deg * ((LD)180.0 / PI); }
	template<typename T0, typename T1, typename T2, typename T3, typename T4>
	ALWAYS_INLINE LD mapval(T0 value, T1 minIn, T2 maxIn, T3 minOut, T4 maxOut ) { return (((LD)value - (LD)minIn) / ((LD)maxIn - (LD)minIn)) * ((LD)maxOut - (LD)minOut) + (LD)minOut; }
	ALWAYS_INLINE void _yield(I64 ns = 1) { sleep_for(chrono::nanoseconds(ns)); yield(); }
	ALWAYS_INLINE LD randd() { return (LD)rand() / (LD)RAND_MAX; }
	
	template<size_t N>
    struct rValStr {
        char data[N];
    };
	
    template<size_t N, size_t K>
    constexpr auto RemoveStringContents(const char(&expr)[N], const char(&remove)[K]) {
        rValStr<N> result = {};
        size_t srcIdx = 0;
        size_t dstIdx = 0;
        while(srcIdx < N) {
            size_t matchIdx = 0;
            while(matchIdx < K - 1 && srcIdx + matchIdx < N - 1 && expr[srcIdx + matchIdx] == remove[matchIdx])
                matchIdx++;
            if(matchIdx == (K - 1))
                srcIdx += matchIdx;
            result.data[dstIdx++] = expr[srcIdx] == '"' ? '\'' : expr[srcIdx];
            srcIdx++;
        }
        return result;
    }
	
	template<class Tp>
	ALWAYS_INLINE
	typename enable_if<is_trivially_copyable<Tp>::value && (sizeof(Tp) <= sizeof(Tp*)), void>::type
	DoNotOptimize(Tp& value) {
		asm volatile("" : "+r"(&value) : : "memory");
	}

	template<class Tp>
	ALWAYS_INLINE
	typename enable_if<!std::is_trivially_copyable<Tp>::value || (sizeof(Tp) > sizeof(Tp*)), void>::type
	DoNotOptimize(Tp& value) {
		asm volatile("" : "+m"(&value) : : "memory");
	}
	
	#ifdef ENABLE_BAD_LOG
		#define BADLOG(x) std::cout << x << std::endl;
		#define BADLOGV(x) BADLOG(#x << ":") BADLOG(x << std::endl)
	#else
		#define BADLOG(x)
		#define BADLOGV(x)
	#endif

	ALWAYS_INLINE void toLowercase(string& str) {
		for_each(str.begin(), str.end(), [&](char& c) {
			c = tolower(c);
		});
	}

	ALWAYS_INLINE string toLowercaseRtn(string str) {
		toLowercase(str);
		return str;
	}

	ALWAYS_INLINE void toUppercase(string& str) {
		for_each(str.begin(), str.end(), [&](char& c) {
			c = toupper(c);
		});
	}

	ALWAYS_INLINE string toUpperrcaseRrn(string str) {
		toUppercase(str);
		return str;
	}
	
	ALWAYS_INLINE LLI getEditDistance(const string& x, const string& y) {
		const LLI m = x.length();
		const LLI n = y.length();
		LLI T[m + 1][n + 1];
		for(LLI i = 1; i <= m; i++)
			T[i][0] = i;
		for(LLI j = 1; j <= n; j++)
			T[0][j] = j;
		for(LLI i = 1; i <= m; i++)
			for (LLI j = 1; j <= n; j++)
				T[i][j] = min(min(T[i-1][j] + 1, T[i][j-1] + 1), T[i-1][j-1] + (x[i - 1] == y[j - 1] ? 0 : 1));
		return T[m][n];
	}
 
	ALWAYS_INLINE LD findStringSimilarity(const string& first, const string& second) {
		const LD maxL = max(first.length(), second.length());
		return ((maxL > 0) ? ((maxL - getEditDistance(first, second)) / maxL) : 1);
	}

	template<template<typename> typename VT, typename T>
	ALWAYS_INLINE LD vecAvg(VT<T>& vec) {
		LD sum = 0;
		for(const T& val : vec) sum += val;
		return sum / (LD)vec.size();
	}

	template<typename T>
	ALWAYS_INLINE string toString(T a) {
		{ if(typeid(T) == typeid(bool)) { return ((a) ? "true" : "false"); } }
		stringstream ss;
		ss << a;
		return ss.str();
    }
	
	template<typename T>
	ALWAYS_INLINE string toStringPrecision(T a, char precision) {
		{ if(typeid(T) == typeid(bool)) { return ((a) ? "true" : "false"); } }
		stringstream ss1;
		stringstream ss2;
		ss1 << setprecision(16) << fixed << a;
		ss2 << LLI(a);
		string s1 = ss1.str();
		string s2 = ss2.str();
		if((s2.size() >= (int)precision) || (s1.substr(0, precision).size() == s2.size() + 1))
			return s2;
		return s1.substr(0, precision);
	}

	template<template<typename> typename VT, typename T>
	ALWAYS_INLINE void writeVectorFile(string filepath, VT<T>& vec, bool append = false) {
		const ULLI vSize = vec.size();
		ofstream* f = new ofstream(filepath, ios::binary);
		!append ? f->clear() : void(0);
		f->write(reinterpret_cast<char*>(&vSize), sizeof(ULLI));
		for(ULLI i = 0; i < vSize; i++) f->write(reinterpret_cast<char*>(&vec[i]), sizeof(T));
		f->close();
		delete f;
	}
	
	template<template<typename> typename VT, typename T>
	ALWAYS_INLINE void readVectorFile(string filepath, VT<T>& vec, bool append = false, ULLI beginIdx = 0, ULLI endIdx = 0) {
		ULLI size = 0;
		ifstream* f = new ifstream(filepath, ios::binary);
		if(!f) { return; }
		f->read(reinterpret_cast<char*>(&size), sizeof(ULLI));
		ULI readlength = ((endIdx != 0) ? (endIdx < size ? endIdx : size) : size) - beginIdx;
		if(readlength < 1) { return; }
		if(beginIdx != 0) f->seekg(sizeof(ULLI) + (beginIdx * sizeof(T)));
		ULLI currentSize = append ? vec.size() : 0;
		append ? vec.resize(currentSize + readlength) : (vec.clear() && vec.resize(readlength));
		for(ULLI i = 0; i < readlength; i++) f->read(reinterpret_cast<char*>(&vec[currentSize + i]), sizeof(T));
		f->close();
		delete f;
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