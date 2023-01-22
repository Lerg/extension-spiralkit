#include <spiralkit/input_manager.h>

namespace spiralkit {
	dmArray<TouchInteractive*> InputManager::_touchListeners;
	dmHashTable64<TouchInteractive*> InputManager::_focusedTouchListeners;
	dmArray<KeyInteractive*> InputManager::_keyListeners;
	dmHashTable64<bool> InputManager::_keyActions;
	dmHashTable64<bool> InputManager::_mouseActions;
	dmHashTable64<bool> InputManager::_gamepadActions;
}
