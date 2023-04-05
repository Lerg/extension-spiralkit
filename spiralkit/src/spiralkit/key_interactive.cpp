#include <spiralkit/input_manager.h>

#include <spiralkit/key_interactive.h>

namespace spiralkit {
	KeyInteractive::KeyInteractive(SkObject *object)
	: object(object), onKey(nullptr) {
		InputManager::AddKeyListener(this);
	}

	KeyInteractive::~KeyInteractive() {
		InputManager::RemoveKeyListener(this);
	}
}
