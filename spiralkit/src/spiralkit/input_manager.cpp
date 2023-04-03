#include <spiralkit/input_manager.h>

namespace spiralkit {
	dmArray<TouchInteractive*> InputManager::_touchListeners;
	dmHashTable64<TouchInteractive*> InputManager::_focusedTouchListeners;
	dmArray<MouseInteractive*> InputManager::_mouseListeners;
	dmArray<KeyInteractive*> InputManager::_keyListeners;
	dmArray<GamepadInteractive*> InputManager::_gamepadListeners;
	dmHashTable64<bool> InputManager::_keyActions;
	dmHashTable64<bool> InputManager::_mouseActions;
	dmHashTable64<bool> InputManager::_gamepadActions;
	dmHashTable64<bool> InputManager::_actionDown;
	uint32_t InputManager::_gamepadIndex = 0;
	dmHID::HContext InputManager::hidContext;
	InputEventType InputManager::lastInputType;
}
