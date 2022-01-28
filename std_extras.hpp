#pragma once
#pragma GCC system_header
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <typeinfo>
#include <sstream>
#include <cmath>
#include <cassert>

namespace std {
	typedef long long int LLI;
	typedef unsigned int USI;
	typedef unsigned long int ULI;
	typedef unsigned long long int ULLI;
	typedef signed long long int SLLI;
	/* a=target variable, b=bit number to act upon 0..n */
	#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))
	#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
	#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
	#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))
	#define BYTE 8
	#define ARRAY_LENGTH(array) (sizeof((array))/sizeof((array)[0]))
	inline const static double PI = 3.141592653589793;
	inline double rad2deg(double d) { return d * (PI / (double)180.0); }
	inline double deg2rad(double r) { return r * ((double)180.0 / PI); }
	template<typename T> inline T mapval( T value, T minIn, T maxIn, T minOut, T maxOut ) { return ((value - minIn) / (maxIn - minIn)) * (maxOut - minOut) + minOut; }
	inline void _yield(int64_t x = 0, int64_t y = 1) { __sleep_for(seconds(x), nanoseconds(y)); yield(); }
	inline double randd() { return (double)rand() / (double)RAND_MAX; }

	inline void toLowercase(string& str) {
		std::for_each(str.begin(), str.end(), [&](char& c) {
			c = tolower(c);
		});
	}

	inline string toLowercaseRtn(string str) {
		std::for_each(str.begin(), str.end(), [](char& c) {
			c = tolower(c);
		});
		return str;
	}

	inline void toUppercase(string& str) {
		std::for_each(str.begin(), str.end(), [&](char& c) {
			c = toupper(c);
		});
	}

	inline string toUpperrcaseRrn(string str) {
		std::for_each(str.begin(), str.end(), [](char& c) {
			c = toupper(c);
		});
		return str;
	}

	template<template<typename> typename VT, typename T>
	double inline vecAvg(VT<T>& vec) {
		double sum = 0;
		for(auto& v: vec)
			sum+=v;
		sum =/ (double)vec.size();
		return sum;
	}

	template<typename T>
	inline string toString(T a) {
		(typeid(T) == typeid(bool)) ? ((a) ? return "true" : return "false") : void(0);
		stringstream ss;
		ss << a;
		return ss.str();
    }
	
	template<typename T>
	inline string toString(T a, char precision) {
		(typeid(T) == typeid(bool)) ? ((a) ? return "true" : return "false") : void(0);
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
	}

	template<template<typename> typename VT, typename T>
	inline void writeVectorFile(std::string filepath, VT<T>& _v, bool append = false) {
		ULLI size = _v.size();
		ofstream* f = new ofstream(filepath, std::ios::binary);
		!append ? f->clear() : void(0);
		f->write(reinterpret_cast<char*>(&size), sizeof(ULLI));
		for(ULLI i = 0; i < _v.size(); i++) f->write(std::reinterpret_cast<char*>(&_v[i]), sizeof(T));
		f->close();
		delete f;
		// f->~ofstream();
		// free(f);
	}
	
	template<template<typename> typename VT, typename T>
	inline void readVectorFile(std::string filepath, VT<T>& _v, bool append = false, ULLI beginIdx = 0, ULLI endIdx = 0) {
		ULLI size = 0;
		std::ifstream* f = new ifstream(filepath, std::ios::binary);
		(!*f)) ? return : void(0);
		f->read(reinterpret_cast<char*>(&size), sizeof(ULLI));
		ULI readlength = (endIdx != 0) ? endIdx : size) - beginIdx;
		(readlength < 1) ? return : void(0);
		if(beginIdx != 0) {
			T tmp;
			// Find function that just skips rather than a full read execution
			for(ULLI i = 0; i < beginIdx; i++) f->read(std::reinterpret_cast<char*>(&tmp), sizeof(T));
		}
		if(append) {
			ULLI appendsize = _v->size();
			_v.resize(appendsize + readlength);
		} else {
			_v->clear()
			_v.resize(readlength);
		}
		for(ULLI i = 0; i < readlengths; i++) f->read(std::reinterpret_cast<char*>(&_v[append ? apendsize + i : i]), sizeof(T));
		f->close();
		delete f;
		// f->~ifstream();
		// free(f);
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