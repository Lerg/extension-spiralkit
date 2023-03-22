#include <spiralkit/input_manager.h>

namespace spiralkit {
	GamepadInteractive::GamepadInteractive(SkObject *object)
	: object(object), onGamepad(nullptr) {
		InputManager::AddGamepadListener(this);
	}

	GamepadInteractive::~GamepadInteractive() {
		InputManager::RemoveGamepadListener(this);
	}
}
