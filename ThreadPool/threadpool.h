/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2022, Peter Ferranti
*   All rights reserved.
*/

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <thread>
#include <deque>
#include <vector>

#include "../vendor/Singleton/singleton_container_map.hpp"
#include "../vendor/Timestamp/timestamp.hpp"
#include "../Semaphore/semaphore.h"

using namespace std::this_thread;
using namespace std::chrono;

class ThreadPool : public SingltonContainerMap<ThreadPool> {
	public:
	typedef std::function<void(void)> Task;
	typedef std::thread::id TIDType;
	enum PoolState : char {
		STARTING = 0,
		RUNNING = 1,
		SHUTTING_DOWN = 2,
		INACTIVE = -1
	};
	
	private:
	_SCM_CHILD_DECLORATIONS(ThreadPool)
	typedef struct {
		TIDType ID = {};
		std::thread* Thread = nullptr;
		Timestamp Begin;
	} Worker;
	Task m_onTBegin = Task{};
	Task m_onTEnd = Task{};
	static void WorkerRun(ThreadPool& pool, ThreadPool::Worker& self);
	void AllocateWorker();
	int64_t m_maxWorkerCount = 2;
	int64_t m_maxQueueSize = 32;
	std::deque<Task> m_tasks = {};
	std::vector<Worker> m_workers = {};
	Timestamp m_begin;
	Semaphore* m_sem = 0;
	std::mutex* m_mtx = 0;
	PoolState m_poolState = PoolState::INACTIVE;
	bool m_isOverflowBlocking = true;
	ThreadPool(int64_t maxWorkerCount = 2, bool isOverflowBlocking = true, int64_t maxQueueSize = 32, Task onTBegin = Task{}, Task onTEnd = Task{});
	~ThreadPool();
	
	public:
	template<typename Func, typename... Args>
	void enqueue_work(Func&& F, Args&& ... args) {
		m_sem->waitFor([&](const int64_t cVal, const int64_t CInitVal){ return (m_poolState != PoolState::RUNNING) && (m_poolState != PoolState::INACTIVE); });
		if(m_poolState != PoolState::INACTIVE) {
			if(m_workers.size() <= m_maxWorkerCount) AllocateWorker();
			m_sem->waitFor([&](const int64_t cVal, const int64_t CInitVal){ return (m_tasks.size() >= m_maxQueueSize) && m_isOverflowBlocking; });
			if(m_tasks.size() < m_maxQueueSize) {
				std::lock_guard<std::mutex> lock(*m_mtx);
				m_tasks.emplace_back(std::move([&](){ F(args...); }));
				m_sem->inc();
			}
		}
	}
};
#endif
