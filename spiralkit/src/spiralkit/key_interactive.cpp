#include <spiralkit/input_manager.h>

namespace spiralkit {
	KeyInteractive::KeyInteractive(SkObject *object)
	: object(object), onKey(nullptr) {
		InputManager::AddKeyListener(this);
	}

	KeyInteractive::~KeyInteractive() {
		InputManager::RemoveKeyListener(this);
	}
}
