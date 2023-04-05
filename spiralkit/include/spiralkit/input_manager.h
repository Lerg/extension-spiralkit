#ifndef SK_INPUT_MANAGER_H
#define SK_INPUT_MANAGER_H

#include <dmsdk/gameobject/gameobject.h>
#include <dmsdk/dlib/hashtable.h>
#include <spiralkit/input_event.h>

namespace spiralkit {
	class TouchInteractive;
	class KeyInteractive;
	class MouseInteractive;
	class GamepadInteractive;

	typedef void (*OnInputTypeChangedCallback)(InputEventType input_event_type);

	class InputManager {
		private:
			static dmArray<TouchInteractive*> _touchListeners;
			static dmHashTable64<TouchInteractive*> _focusedTouchListeners;
			static dmArray<KeyInteractive*> _keyListeners;
			static dmArray<MouseInteractive*> _mouseListeners;
			static dmArray<GamepadInteractive*> _gamepadListeners;
			static dmHashTable64<bool> _keyActions;
			static dmHashTable64<bool> _mouseActions;
			static dmHashTable64<bool> _gamepadActions;
			static dmHashTable64<bool> _actionDown;
			static uint32_t _gamepadIndex;

			InputManager();

		public:
			static dmHID::HContext hidContext;
			static InputEventType lastInputType;
			static OnInputTypeChangedCallback onInputTypeChanged;

			static void Init();
			static void SetTouchFocus(uint64_t id, TouchInteractive *target);
			static void ReleaseTouchFocus(uint64_t id);
			static void AddTouchListener(TouchInteractive *listener);
			static void RemoveTouchListener(TouchInteractive *listener);
			static void OnTouch(const dmGameObject::InputAction &input_action);
			static void AddMouseListener(MouseInteractive *listener);
			static void RemoveMouseListener(MouseInteractive *listener);
			static void OnMouseClick(const dmGameObject::InputAction &action);
			static void OnMouseMove(const dmGameObject::InputAction &action);
			static void OnMouseWheel(const dmGameObject::InputAction &action);
			static void OnText(const dmGameObject::InputAction &action);
			static void AddKeyListener(KeyInteractive *listener);
			static void RemoveKeyListener(KeyInteractive *listener);
			static void OnKey(const dmGameObject::InputAction &action);
			static void AddGamepadListener(GamepadInteractive *listener);
			static void RemoveGamepadListener(GamepadInteractive *listener);
			static void OnGamepad(const dmGameObject::InputAction &action);
			static void OnInput(dmGameObject::InputAction &input_action);
			static bool IsActionDown(dmhash_t action_id);
			static bool IsController();

		private:
			static void _SetLastInputType(InputEventType input_event_type);
	};
}

#endif
