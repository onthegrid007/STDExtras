#ifndef TSCOUT_HPP_
#define TSCOUT_HPP_

#include "../singleton/singleton.hpp"
#include <iostream>

namsepace std {
	class TSCout: public Singleton<TSCout> {
	public:
		template<typename T>
		ostream& operator<<(T& op) {
			lock_guard<mutex>(*m_mutex);
			cout << op;
			cout.flush();
		}

		template<typename T>
		ostream& operator<<(T&& op) {
			lock_guard<mutex>(*m_mutex);
			cout << op;
			cout.flush();
		}
	private:
		TSCout() {}
		mutex m_mutex;
		~TSCout() {}
	};

	inline TSCout TSCout::Instance;

	#define tscout TSCout:GetInstance();
	#define tsendl "\n"
}

#endif
