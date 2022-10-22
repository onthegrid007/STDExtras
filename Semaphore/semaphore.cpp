/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2022, Peter Ferranti
*   All rights reserved.
*   Other Contributers:
*/

#include "semaphore.h"

Semaphore::Semaphore(int64_t i) :
	m_CInit(i),
	m_C(i) { }

void Semaphore::inc(int64_t i) {
	*this += i;
}

void Semaphore::dec(int64_t i) {
	*this -= i;
}

void Semaphore::notify() {
	m_CV.notify_all();
}

void Semaphore::waitFor(std::function<bool(const int64_t cVal, const int64_t CInitVal)>&& rtnBool) {
	std::unique_lock<std::mutex> lock(m_M);
	notify();
	while(!rtnBool(m_C, m_CInit)) m_CV.wait(lock, [cVal = m_C, cInitVal = m_CInit, &rtnBool]{ return rtnBool(cVal, cInitVal); });
}

void Semaphore::waitForI(int64_t i) {
	waitFor([&, i](const int64_t cVal, const int64_t cInitVal){ return (cVal == i); });
}

void Semaphore::wait() {
	waitFor([&](const int64_t cVal, const int64_t cInitVal){ return (cVal == cInitVal); });
}

void Semaphore::set(int64_t i) {
	wait();
	m_C = i;
	m_CInit = i;
}

Semaphore::operator int64_t() {
	return m_C;
}

int64_t Semaphore::operator+(const Semaphore& other) {
	return m_C + other.m_C;
}

int64_t Semaphore::operator-(const Semaphore& other) {
	return m_C - other.m_C;
}

int64_t Semaphore::operator+=(const int64_t i) {
	std::lock_guard<std::mutex> lock(m_M);
	m_C += i;
	notify();
	return m_C;
}

int64_t Semaphore::operator-=(const int64_t i) {
	std::lock_guard<std::mutex> lock(m_M);
	m_C -= i;
	notify();
	return m_C;

}

Semaphore::~Semaphore() {
	// wait();
}