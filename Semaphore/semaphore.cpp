/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2022, Peter Ferranti
*   All rights reserved.
*   Other Contributers:
*/

#include "semaphore.h"
#include "../std_extras.hpp"

namespace std {

	Semaphore::Semaphore(int64_t i) :
		m_M(new mutex),
		m_CV(new condition_variable),
		m_CInit(i),
		m_C(i),
		m_isShutdown(false) { }

	void Semaphore::signal() {
		if(m_isShutdown) return ShutdownErrorHandle();
		ThreadLock lock(*m_M);
		if(m_C < m_CInit) m_C++;
		m_CV->notify_all();
	}

	void Semaphore::wait() {
		if(m_isShutdown) return ShutdownErrorHandle();
		CVThreadLock lock(*m_M);
		m_CV->wait(lock, [&] { return (m_C == m_CInit); });
		m_CV->notify_all();
	}

	void Semaphore::waitForC(int64_t i) {
		if(m_isShutdown) return ShutdownErrorHandle();
		CVThreadLock lock(*m_M);
		m_CV->wait(lock, [&] { return (m_C == i); });
		m_CV->notify_all();
	}

	void Semaphore::set(int64_t i) {
		wait();
		m_C = i;
		m_CInit = i;
	}

	void Semaphore::decrement() {
		if(m_isShutdown) return ShutdownErrorHandle();
		CVThreadLock lock(*m_M);
		m_CV->wait(lock, [&] { return ((m_C > 0)); });
		m_C--;
		m_CV->notify_all();
	}

	int64_t Semaphore::getC() {
		return m_C;
	}

	void Semaphore::Shutdown() {
		wait();
		delete m_M;
		delete m_CV;
		m_CInit = 0;
		m_C = 0;
		m_isShutdown = true;	
	}

	void std::Semaphore::ShutdownErrorHandle() {
		// Handle Error
	}

	Semaphore::~Semaphore() {
		if(!m_isShutdown) Shutdown();
	}
}