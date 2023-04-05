#include <spiralkit/input_manager.h>

#include <spiralkit/gamepad_interactive.h>

namespace spiralkit {
	GamepadInteractive::GamepadInteractive(SkObject *object)
	: object(object), onGamepad(nullptr) {
		InputManager::AddGamepadListener(this);
	}

	GamepadInteractive::~GamepadInteractive() {
		InputManager::RemoveGamepadListener(this);
	}
}
