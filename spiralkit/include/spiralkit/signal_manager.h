#ifndef SK_SIGNAL_MANAGER_H
#define SK_SIGNAL_MANAGER_H

#include "signals.h"

namespace spiralkit {
	class SignalManager {
		private:
			SignalManager();
		public:
			static signal<void()> finalize;
	};
}

#endif
