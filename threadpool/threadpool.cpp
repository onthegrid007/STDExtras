w.i.p.




#include "threadpool.h"
std::unordered_map<std::string, T> std::ThreadPool::ContainerMap;
std::ThreadPool::ThreadPool() {}

/*
ThreadPool::ThreadPool() :
	m_mtx(new mutex) {}

void ThreadPool::Initalize(string&& poolName, int64_t threadCount, bool debugThreads, bool debug) {
	m_poolName = poolName;
	m_debugThreads = debugThreads;
	m_threadCount = threadCount;
	if(m_workers.size() > 0)
		Destroy(debug);
	for(int64_t i = 0; i < m_threadCount; i++) {
		m_workers.emplace_back(new thread([=, i]{
			unique_lock<mutex> lock(*m_mtx);
			m_cv.wait(lock, [this]{ return (m_state == 1); });
			m_workers[i]->detach();
			while(true) {
				m_cv.wait(lock, [this]{ return ((m_deq.size() > 0) | (m_state == 2)); });
				if(m_state == 1) {
					auto Task = m_deq.front();
					if(Task != nullptr) {
						m_deq.pop_front();
						if(m_debugThreads) mpcout << "[ThreadPool][" << m_poolName << "][" << i << "] Executing Job!" << mpendl;
						Task();
					}
				}
				else if(m_state == 2) {
					if(m_debugThreads) mpcout << "[ThreadPool][" << m_poolName << "][" << i << "] Exiting..." << mpendl;
					return;
				}
			}
		}));
	}
	m_cv.notify_all();
}

void ThreadPool::Destroy(bool debug) {
	m_state = 2;
	m_cv.notify_all();
	for(auto& worker : m_workers)
		worker->~thread();
	m_workers.clear();
	m_deq.clear();
	m_state = 0;
}

ThreadPool::~ThreadPool() {}
*/
