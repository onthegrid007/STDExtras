#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <mutex>
#include <thread>
#include <deque>
#include <vector>
#include <condition_variable>

#include "vendor/singleton/singleton_container_map.hpp"

namespace std {
	class ThreadPool : public SingltonContainerMap<ThreadPool> {
		public:
		typedef function<void(void)> Task;
		private:
		_SCM_CHILD_DECLORATIONS(ThreadPool)
		ThreadPool();
	};
}

/*
class ThreadPool {
public:
	ThreadPool();
    void Initalize(string&& poolName, int64_t threadCount, bool debugThreads = false, bool debug = false);
    template<typename Func, typename... Args>
	void enqueue_work(Func&& F, Args&& ... args, bool debug = false) {
		m_cv.wait(lock, [this]{ return (m_state == 1); });
    	m_deq.emplace_back([=](){ F(args...); });
    	if(debug) mpcout << "[ThreadPool] Enqueued work!" << mpendl;
    	m_cv.notify_one();
    }
    void Destroy(bool debug = false);
	~ThreadPool();
private:
    using Task = function<void(void)>;
    string m_poolName;
	mutex* m_mtx;
	condition_variable m_cv;
	bool m_debugThreads;
	int64_t m_threadCount;
	deque<Task> m_deq;
	vector<thread*> m_workers;
	char m_state = 0;
};
*/
#endif
