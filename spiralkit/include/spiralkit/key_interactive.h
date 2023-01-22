#ifndef SK_KEY_INTERACTIVE_H
#define SK_KEY_INTERACTIVE_H

#include "input_event.h"

namespace spiralkit {
	class KeyInteractive; // Forward reference.
	typedef bool (*KeyInteractiveOnKeyCallback)(KeyInteractive &key_interactive, const KeyEvent &key_event);

	class KeyInteractive {
		public:
			SkObject *object = nullptr;
			KeyInteractiveOnKeyCallback onKey = nullptr;

			KeyInteractive(SkObject *object);
			~KeyInteractive();
	};
}

#endif
