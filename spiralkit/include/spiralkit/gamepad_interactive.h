#ifndef SK_GAMEPAD_INTERACTIVE_H
#define SK_GAMEPAD_INTERACTIVE_H

#include "input_event.h"

namespace spiralkit {
	class GamepadInteractive; // Forward reference.
	class SkObject;
	typedef bool (*GamepadInteractiveOnGamepadCallback)(GamepadInteractive &gamepad_interactive, const GamepadEvent &gamepad_event);

	class GamepadInteractive {
		public:
			SkObject *object = nullptr;
			GamepadInteractiveOnGamepadCallback onGamepad = nullptr;

			GamepadInteractive(SkObject *object);
			~GamepadInteractive();
	};
}

#endif
