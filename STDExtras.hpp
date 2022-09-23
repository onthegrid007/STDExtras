#pragma once
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

namespace std {
	using namespace chrono;
	using namespace this_thread;
	typedef unsigned long int ULI;
	typedef signed long int SLI;
	typedef unsigned long long int ULLI;
	typedef signed long long int SLLI;
	typedef long double LD;
	typedef int64_t I64;
	typedef int32_t I32;
	typedef lock_guard<mutex> ThreadLock;
	typedef lock_guard<shared_mutex> ThreadLockS;
	typedef unique_lock<mutex> CVThreadLock;
	typedef unique_lock<shared_mutex> CVThreadLockS;
	typedef thread::id ThreadID;
	/* a=target variable, b=bit number to act upon 0..n */
	#define BIT(x) (std::I64(1) << (x))
	#define BIT_SET(a,b) ((a) |= BIT(b))
	#define BIT_CLEAR(a,b) ((a) &= ~BIT(b))
	#define BIT_FLIP(a,b) ((a) ^= BIT(b))
	#define BIT_CHECK(a,b) (!!((a) & BIT(b)))
	#define BYTE 8
	#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))
	inline constexpr static LD PI = 3.141592653589793;
	template<typename T>
	inline T rad2deg(T rad) { return rad * (PI / (T)180.0); }
	template<typename T>
	inline T deg2rad(T deg) { return deg * ((T)180.0 / PI); }
	template<typename T>
	inline T mapval( T value, T minIn, T maxIn, T minOut, T maxOut ) { return ((value - minIn) / (maxIn - minIn)) * (maxOut - minOut) + minOut; }
	inline void _yield(I64 ns = 1) { sleep_for(nanoseconds(ns)); yield(); }
	template<typename T>
	inline T randd() { return (T)rand() / (T)RAND_MAX; }
	
	template<size_t N>
    struct rValStr {
        char data[N];
    };
	
    template <size_t N, size_t K>
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

	inline void toLowercase(string& str) {
		for_each(str.begin(), str.end(), [&](char& c) {
			c = tolower(c);
		});
	}

	inline string toLowercaseRtn(string str) {
		toLowercase(str);
		return str;
	}

	inline void toUppercase(string& str) {
		for_each(str.begin(), str.end(), [&](char& c) {
			c = toupper(c);
		});
	}

	inline string toUpperrcaseRrn(string str) {
		toUppercase(str);
		return str;
	}

	template<template<typename> typename VT, typename T>
	T inline vecAvg(VT<T>& vec) {
		T sum = 0;
		for(const auto& val : vec)
			sum += val;
		sum /= (T)vec.size();
		return sum;
	}

	template<typename T>
	inline string toString(T a) {
		{ if(typeid(T) == typeid(bool)) { return ((a) ? "true" : "false"); } }
		stringstream ss;
		ss << a;
		return ss.str();
    }
	
	template<typename T>
	inline string toStringPrecision(T a, char precision) {
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
	inline void writeVectorFile(string filepath, VT<T>& vec, bool append = false) {
		ULLI vSize = vec.size();
		ofstream* f = new ofstream(filepath, ios::binary);
		!append ? f->clear() : void(0);
		f->write(reinterpret_cast<char*>(&vSize), sizeof(ULLI));
		for(ULLI i = 0; i < vSize; i++) f->write(reinterpret_cast<char*>(&vec[i]), sizeof(T));
		f->close();
		delete f;
	}
	
	template<template<typename> typename VT, typename T>
	inline void readVectorFile(string filepath, VT<T>& vec, bool append = false, ULLI beginIdx = 0, ULLI endIdx = 0) {
		ULLI size = 0;
		ifstream* f = new ifstream(filepath, ios::binary);
		if(!f) { return; }
		f->read(reinterpret_cast<char*>(&size), sizeof(ULLI));
		ULI readlength = ((endIdx != 0) ? endIdx : size) - beginIdx;
		if(readlength < 1) { return; }
		if(beginIdx != 0) {
			f->seekg(sizeof(ULLI) + (beginIdx * sizeof(T)));
			// T tmp;
			// Find function that just skips rather than a full read execution
			// for(ULLI i = 0; i < beginIdx; i++) f->read(reinterpret_cast<char*>(&tmp), sizeof(T));
		}
		ULLI currentSize = vec.size();
		if(append) {
			vec.resize(currentSize + readlength);
		} else {
			vec.clear();
			vec.resize(readlength);
		}
		for(ULLI i = 0; i < readlength; i++) f->read(reinterpret_cast<char*>(&vec[append ? currentSize + i : i]), sizeof(T));
		f->close();
		delete f;
	}
}
	/*
	inline void intervalThread(function<void()> begin, function<void()> loop, function<void()> stop, function<bool()> exitCondition, int64_t Interval, vector<int64_t>* LoopTimes, bool debug = false) {
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
	
	inline void intervalFunction(string Name, function<void()> Start, function<void()> Loop, function<void()> Exit, function<bool()> ExitCondition, int64_t Interval, vector<int64_t>* LoopTimes, bool debug = false) {
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
	inline vec3 RGBtoHSV(vec3 rgb) {
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