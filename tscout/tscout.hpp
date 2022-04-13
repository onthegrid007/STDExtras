/*
*   BSD 3-Clause License, see file labled 'LICENSE' for the full License.
*   Copyright (c) 2022, Peter Ferranti
*   All rights reserved.
*   Other Contributers:
*/

#ifndef TSCOUT_HPP_
#define TSCOUT_HPP_

#include "vendor/singleton/singleton.hpp"
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
			return (*this) << op;
		}
		private:
		TSCout() {}
		mutex m_mutex;
		~TSCout() {}
	};

	#define tscout TSCout::GetInstance()
	#define tsendl "\n"
	_SINGLETON_CHILD_DEFINITIONS(TSCout)
}
#endif
