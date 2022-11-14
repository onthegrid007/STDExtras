/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2022, Peter Ferranti
*   All rights reserved.
*/

#include "threadpool.h"
_SCM_CHILD_DEFINITIONS(ThreadPool)

void ThreadPool::WorkerRun(ThreadPool& pool, Worker& self) {
	self.ID = std::this_thread::get_id();
	pool.m_onTBegin();
	while(pool.m_poolState != PoolState::RUNNING);
	while(pool.m_poolState == PoolState::RUNNING) {
		pool.m_sem->waitFor([&](const int64_t cVal, const int64_t CInitVal){ return (cVal != CInitVal) || (pool.m_poolState != PoolState::RUNNING); });
		std::lock_guard<std::mutex> lock(*pool.m_mtx);
		if(pool.m_poolState == PoolState::SHUTTING_DOWN) break;
			if(pool.m_tasks.size() > 0) {
			Task T = pool.m_tasks.front();
			if(T != nullptr) {
				pool.m_tasks.pop_front();
				T();
			}
		}
	}
	pool.m_onTEnd();
	std::lock_guard<std::mutex> lock(*pool.m_mtx);
	self.Thread->detach();
	for(auto itt = begin(pool.m_workers); itt != end(pool.m_workers); itt++)
		if(itt->Thread == self.Thread) { pool.m_workers.erase(itt); break; }
}

void ThreadPool::AllocateWorker() {
	std::lock_guard<std::mutex> lock(*m_mtx);
	if(m_workers.size() <= m_maxWorkerCount) {
		m_workers.emplace_back(std::move(Worker{{}, 0}));
		std::thread* t = std::move(new std::thread(ThreadPool::WorkerRun, this, m_workers.back()));
		m_workers.back().Thread = std::move(t);
	}
}

ThreadPool::ThreadPool(int64_t maxWorkerCount, bool isOverflowBlocking, int64_t maxQueueSize, Task onTBegin, Task onTEnd) :
	m_maxWorkerCount(maxWorkerCount < 2 ? 2 : maxWorkerCount),
	m_isOverflowBlocking(isOverflowBlocking),
	m_maxQueueSize(maxQueueSize),
	m_onTBegin(std::move(onTBegin)),
	m_onTEnd(std::move(onTEnd)),
	m_sem(new Semaphore),
	m_mtx(new std::mutex) {
	for(char i = 0; i < 2; i++)
		AllocateWorker();
}