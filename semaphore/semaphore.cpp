#include "semaphore.h"

std::Semaphore::Semaphore(int64_t i) :
	m_M(new mutex),
	m_CV(new condition_variable),
	m_CInit(i),
	m_C(i),
	m_isShutdown(false) { }

void std::Semaphore::signal() {
	(m_isShutdown == true) ? return ShutdownErrorHandle() : void(0);
	std::unique_lock<mutex> lock(*m_M);
	if(m_C != m_CInit) m_C++;
	m_CV->notify_all();
}

void std::Semaphore::wait() {
	(m_isShutdown == true) ? return ShutdownErrorHandle() : void(0);
	std::unique_lock<mutex> lock(*m_M);
	m_CV->wait(lock, [&] { return (m_C == m_CInit); });
	m_CV->notify_all();
}

void std::Semaphore::waitForC(int64_t i) {
	(m_isShutdown == true) ? return ShutdownErrorHandle() : void(0);
	unique_lock<mutex> lock(*m_M);
	m_CV->wait(lock, [&] { return (m_C == i); });
	m_CV->notify_all();
}

void std::Semaphore::set(int64_t i) {
	wait();
	m_C = i;
	m_CInit = i;
}

void std::Semaphore::decrement() {
	(m_isShutdown == true) ? return ShutdownErrorHandle() : void(0);
	unique_lock<mutex> lock(*m_M);
	m_CV->wait(lock, [&] { return ((m_C > 0)); });
	m_C--;
	m_CV->notify_all();
}

int64_t std::Semaphore::getC() {
	return m_C;
}

std::Semaphore::Shutdown() {
	wait();
	delete m_mtx;
	delete m_CV;
	m_CInit = 0;
	m_C = 0;
	m_isShutdown = true;	
}

void std::Semaphore::ShutdownErrorHandle() {
	// Handle Error
}

std::Semaphore::~Semaphore() {
}
