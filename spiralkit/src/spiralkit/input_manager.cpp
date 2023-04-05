#include <dmsdk/dlib/math.h>
#include <algorithm>
#include <godot/core/math/vector2i.h>
#include <spiralkit/array.h>
#include <spiralkit/hashtable.h>
#include <spiralkit/input.h>
#include <spiralkit/touch_interactive.h>
#include <spiralkit/key_interactive.h>
#include <spiralkit/mouse_interactive.h>
#include <spiralkit/gamepad_interactive.h>
#include <spiralkit/platform.h>
#include <spiralkit/skobject.h>

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
	OnInputTypeChangedCallback InputManager::onInputTypeChanged = nullptr;

	struct TouchInteractiveSortPred {
		inline bool operator() (const TouchInteractive *a, const TouchInteractive *b) {
			return a->object->GetWorldPosition3D().getZ() > b->object->GetWorldPosition3D().getZ();
		}
	};

	struct KeyInteractiveSortPred {
		inline bool operator() (const KeyInteractive *a, const KeyInteractive *b) {
			return a->object->GetWorldPosition3D().getZ() > b->object->GetWorldPosition3D().getZ();
		}
	};

	struct GamepadInteractiveSortPred {
		inline bool operator() (const GamepadInteractive *a, const GamepadInteractive *b) {
			return a->object->GetWorldPosition3D().getZ() > b->object->GetWorldPosition3D().getZ();
		}
	};

	void InputManager::Init() {
		lastInputType = InputEventType_Touch;
		_touchListeners.SetCapacity(32);
		_focusedTouchListeners.SetCapacity(24, 32);
		_keyListeners.SetCapacity(32);
		_mouseListeners.SetCapacity(32);
		_gamepadListeners.SetCapacity(32);

		_keyActions.SetCapacity(72, 124);
		_mouseActions.SetCapacity(16, 13);
		_gamepadActions.SetCapacity(8, 27);
		_actionDown.SetCapacity(100, _keyActions.Capacity() + _mouseActions.Capacity() + _gamepadActions.Capacity());

		_keyActions.Put(input::KEY_SPACE, true);
		_keyActions.Put(input::KEY_EXCLAMATIONMARK, true);
		_keyActions.Put(input::KEY_DOUBLEQUOTE, true);
		_keyActions.Put(input::KEY_HASH, true);
		_keyActions.Put(input::KEY_DOLLARSIGN, true);
		_keyActions.Put(input::KEY_AMPERSAND, true);
		_keyActions.Put(input::KEY_SINGLEQUOTE, true);
		_keyActions.Put(input::KEY_LPAREN, true);
		_keyActions.Put(input::KEY_RPAREN, true);
		_keyActions.Put(input::KEY_ASTERISK, true);
		_keyActions.Put(input::KEY_PLUS, true);
		_keyActions.Put(input::KEY_COMMA, true);
		_keyActions.Put(input::KEY_MINUS, true);
		_keyActions.Put(input::KEY_PERIOD, true);
		_keyActions.Put(input::KEY_SLASH, true);
		_keyActions.Put(input::KEY_0, true);
		_keyActions.Put(input::KEY_1, true);
		_keyActions.Put(input::KEY_2, true);
		_keyActions.Put(input::KEY_3, true);
		_keyActions.Put(input::KEY_4, true);
		_keyActions.Put(input::KEY_5, true);
		_keyActions.Put(input::KEY_6, true);
		_keyActions.Put(input::KEY_7, true);
		_keyActions.Put(input::KEY_8, true);
		_keyActions.Put(input::KEY_9, true);
		_keyActions.Put(input::KEY_COLON, true);
		_keyActions.Put(input::KEY_SEMICOLON, true);
		_keyActions.Put(input::KEY_LESSTHAN, true);
		_keyActions.Put(input::KEY_EQUALS, true);
		_keyActions.Put(input::KEY_GREATERTHAN, true);
		_keyActions.Put(input::KEY_QUESTIONMARK, true);
		_keyActions.Put(input::KEY_AT, true);
		_keyActions.Put(input::KEY_A, true);
		_keyActions.Put(input::KEY_B, true);
		_keyActions.Put(input::KEY_C, true);
		_keyActions.Put(input::KEY_D, true);
		_keyActions.Put(input::KEY_E, true);
		_keyActions.Put(input::KEY_F, true);
		_keyActions.Put(input::KEY_G, true);
		_keyActions.Put(input::KEY_H, true);
		_keyActions.Put(input::KEY_I, true);
		_keyActions.Put(input::KEY_J, true);
		_keyActions.Put(input::KEY_K, true);
		_keyActions.Put(input::KEY_L, true);
		_keyActions.Put(input::KEY_M, true);
		_keyActions.Put(input::KEY_N, true);
		_keyActions.Put(input::KEY_O, true);
		_keyActions.Put(input::KEY_P, true);
		_keyActions.Put(input::KEY_Q, true);
		_keyActions.Put(input::KEY_R, true);
		_keyActions.Put(input::KEY_S, true);
		_keyActions.Put(input::KEY_T, true);
		_keyActions.Put(input::KEY_U, true);
		_keyActions.Put(input::KEY_V, true);
		_keyActions.Put(input::KEY_W, true);
		_keyActions.Put(input::KEY_X, true);
		_keyActions.Put(input::KEY_Y, true);
		_keyActions.Put(input::KEY_Z, true);
		_keyActions.Put(input::KEY_LBRACKET, true);
		_keyActions.Put(input::KEY_RBRACKET, true);
		_keyActions.Put(input::KEY_BACKSLASH, true);
		_keyActions.Put(input::KEY_CARET, true);
		_keyActions.Put(input::KEY_UNDERSCORE, true);
		_keyActions.Put(input::KEY_GRAVE, true);
		_keyActions.Put(input::KEY_LBRACE, true);
		_keyActions.Put(input::KEY_RBRACE, true);
		_keyActions.Put(input::KEY_PIPE, true);
		_keyActions.Put(input::KEY_ESC, true);
		_keyActions.Put(input::KEY_F1, true);
		_keyActions.Put(input::KEY_F2, true);
		_keyActions.Put(input::KEY_F3, true);
		_keyActions.Put(input::KEY_F4, true);
		_keyActions.Put(input::KEY_F5, true);
		_keyActions.Put(input::KEY_F6, true);
		_keyActions.Put(input::KEY_F7, true);
		_keyActions.Put(input::KEY_F8, true);
		_keyActions.Put(input::KEY_F9, true);
		_keyActions.Put(input::KEY_F10, true);
		_keyActions.Put(input::KEY_F11, true);
		_keyActions.Put(input::KEY_F12, true);
		_keyActions.Put(input::KEY_UP, true);
		_keyActions.Put(input::KEY_DOWN, true);
		_keyActions.Put(input::KEY_LEFT, true);
		_keyActions.Put(input::KEY_RIGHT, true);
		_keyActions.Put(input::KEY_LSHIFT, true);
		_keyActions.Put(input::KEY_RSHIFT, true);
		_keyActions.Put(input::KEY_LCTRL, true);
		_keyActions.Put(input::KEY_RCTRL, true);
		_keyActions.Put(input::KEY_LALT, true);
		_keyActions.Put(input::KEY_RALT, true);
		_keyActions.Put(input::KEY_TAB, true);
		_keyActions.Put(input::KEY_ENTER, true);
		_keyActions.Put(input::KEY_BACKSPACE, true);
		_keyActions.Put(input::KEY_INSERT, true);
		_keyActions.Put(input::KEY_DEL, true);
		_keyActions.Put(input::KEY_PAGEUP, true);
		_keyActions.Put(input::KEY_PAGEDOWN, true);
		_keyActions.Put(input::KEY_HOME, true);
		_keyActions.Put(input::KEY_END, true);
		_keyActions.Put(input::KEY_NUMPAD_0, true);
		_keyActions.Put(input::KEY_NUMPAD_1, true);
		_keyActions.Put(input::KEY_NUMPAD_2, true);
		_keyActions.Put(input::KEY_NUMPAD_3, true);
		_keyActions.Put(input::KEY_NUMPAD_4, true);
		_keyActions.Put(input::KEY_NUMPAD_5, true);
		_keyActions.Put(input::KEY_NUMPAD_6, true);
		_keyActions.Put(input::KEY_NUMPAD_7, true);
		_keyActions.Put(input::KEY_NUMPAD_8, true);
		_keyActions.Put(input::KEY_NUMPAD_9, true);
		_keyActions.Put(input::KEY_NUMPAD_DIVIDE, true);
		_keyActions.Put(input::KEY_NUMPAD_MULTIPLY, true);
		_keyActions.Put(input::KEY_NUMPAD_SUBTRACT, true);
		_keyActions.Put(input::KEY_NUMPAD_ADD, true);
		_keyActions.Put(input::KEY_NUMPAD_DECIMAL, true);
		_keyActions.Put(input::KEY_NUMPAD_EQUAL, true);
		_keyActions.Put(input::KEY_NUMPAD_ENTER, true);
		_keyActions.Put(input::KEY_NUMPAD_NUMLOCK, true);
		_keyActions.Put(input::KEY_CAPSLOCK, true);
		_keyActions.Put(input::KEY_SCROLLLOCK, true);
		_keyActions.Put(input::KEY_PAUSE, true);
		_keyActions.Put(input::KEY_LSUPER, true);
		_keyActions.Put(input::KEY_RSUPER, true);
		_keyActions.Put(input::KEY_MENU, true);
		_keyActions.Put(input::KEY_BACK, true);

		_mouseActions.Put(input::MOUSE_WHEEL_UP, true);
		_mouseActions.Put(input::MOUSE_WHEEL_DOWN, true);
		_mouseActions.Put(input::MOUSE_BUTTON_LEFT, true);
		_mouseActions.Put(input::MOUSE_BUTTON_MIDDLE, true);
		_mouseActions.Put(input::MOUSE_BUTTON_RIGHT, true);
		_mouseActions.Put(input::MOUSE_BUTTON_1, true);
		_mouseActions.Put(input::MOUSE_BUTTON_2, true);
		_mouseActions.Put(input::MOUSE_BUTTON_3, true);
		_mouseActions.Put(input::MOUSE_BUTTON_4, true);
		_mouseActions.Put(input::MOUSE_BUTTON_5, true);
		_mouseActions.Put(input::MOUSE_BUTTON_6, true);
		_mouseActions.Put(input::MOUSE_BUTTON_7, true);
		_mouseActions.Put(input::MOUSE_BUTTON_8, true);

		_gamepadActions.Put(input::GAMEPAD_LSTICK_LEFT, true);
		_gamepadActions.Put(input::GAMEPAD_LSTICK_RIGHT, true);
		_gamepadActions.Put(input::GAMEPAD_LSTICK_DOWN, true);
		_gamepadActions.Put(input::GAMEPAD_LSTICK_UP, true);
		_gamepadActions.Put(input::GAMEPAD_LSTICK_CLICK, true);
		_gamepadActions.Put(input::GAMEPAD_LTRIGGER, true);
		_gamepadActions.Put(input::GAMEPAD_LSHOULDER, true);
		_gamepadActions.Put(input::GAMEPAD_LPAD_LEFT, true);
		_gamepadActions.Put(input::GAMEPAD_LPAD_RIGHT, true);
		_gamepadActions.Put(input::GAMEPAD_LPAD_DOWN, true);
		_gamepadActions.Put(input::GAMEPAD_LPAD_UP, true);
		_gamepadActions.Put(input::GAMEPAD_RSTICK_LEFT, true);
		_gamepadActions.Put(input::GAMEPAD_RSTICK_RIGHT, true);
		_gamepadActions.Put(input::GAMEPAD_RSTICK_DOWN, true);
		_gamepadActions.Put(input::GAMEPAD_RSTICK_UP, true);
		_gamepadActions.Put(input::GAMEPAD_RSTICK_CLICK, true);
		_gamepadActions.Put(input::GAMEPAD_RTRIGGER, true);
		_gamepadActions.Put(input::GAMEPAD_RSHOULDER, true);
		_gamepadActions.Put(input::GAMEPAD_RPAD_LEFT, true);
		_gamepadActions.Put(input::GAMEPAD_RPAD_RIGHT, true);
		_gamepadActions.Put(input::GAMEPAD_RPAD_DOWN, true);
		_gamepadActions.Put(input::GAMEPAD_RPAD_UP, true);
		_gamepadActions.Put(input::GAMEPAD_START, true);
		_gamepadActions.Put(input::GAMEPAD_BACK, true);
		_gamepadActions.Put(input::GAMEPAD_GUIDE, true);
		_gamepadActions.Put(input::GAMEPAD_CONNECT, true);
		_gamepadActions.Put(input::GAMEPAD_DISCONNECT, true);
	}

	#pragma region Touch

	void InputManager::SetTouchFocus(uint64_t id, TouchInteractive *target) {
		if (_focusedTouchListeners.Full()) {
			uint32_t new_capacity = _focusedTouchListeners.Capacity() + 32;
			_focusedTouchListeners.SetCapacity(dmMath::Max(1U, 2 * new_capacity / 3), new_capacity);
		}
		_focusedTouchListeners.Put(id, target);
	}

	void InputManager::ReleaseTouchFocus(uint64_t id) {
		_focusedTouchListeners.Erase(id);
	}

	void InputManager::AddTouchListener(TouchInteractive *listener) {
		if (_touchListeners.Full()) {
			_touchListeners.OffsetCapacity(32);
		}
		_touchListeners.Push(listener);
	}

	void InputManager::RemoveTouchListener(TouchInteractive *listener) {
		_touchListeners.EraseSwap(dmArrayUtil::GetIndexOf(&_touchListeners, listener));
		const uint64_t *key = dmHashtableUtil::FindKey(&_focusedTouchListeners, listener);
		if (key != nullptr) {
			_focusedTouchListeners.Erase(*key);
		}
	}

	void InputManager::OnTouch(const dmGameObject::InputAction &input_action) {
		for (int32_t i = 0; i < input_action.m_TouchCount; ++i) {
			const dmHID::Touch touch = input_action.m_Touch[i];
			bool is_taken = false;
			InputEventPhase phase;
			switch (touch.m_Phase) {
				case dmHID::PHASE_BEGAN:
					phase = InputEventPhase_Pressed;
					break;
				case dmHID::PHASE_ENDED:
					phase = InputEventPhase_Released;
					break;
				case dmHID::PHASE_MOVED:
					phase = InputEventPhase_Moved;
					break;
				default:
					continue;
			}
			if (phase == InputEventPhase_Moved && touch.m_ScreenDX == 0 && touch.m_ScreenDY == 0) {
				continue;
			}
			const TouchEvent touch_event(touch.m_Id, Vector2i{touch.m_ScreenX, touch.m_ScreenY}, phase);
			TouchInteractive **touch_interactive = _focusedTouchListeners.Get(touch_event.id);
			if (touch_interactive != nullptr) {
				is_taken = (*touch_interactive)->onTouch(**touch_interactive, touch_event);
			}
			if (!is_taken) {
				std::sort(_touchListeners.Begin(), _touchListeners.End(), TouchInteractiveSortPred());
				// Tempopary array to safely iterate while the main array may change size.
				dmArray<TouchInteractive*> tmp_listeners;
				tmp_listeners.SetCapacity(_touchListeners.Size());
				for (uint32_t i = 0; i < _touchListeners.Size(); ++i) {
					tmp_listeners.Push(_touchListeners[i]);
				}
				for (uint32_t i = 0; i < tmp_listeners.Size(); ++i) {
					TouchInteractive *touch_interactive = tmp_listeners[i];
					if (
						touch_interactive->object->IsEnabled()
						&& touch_interactive->object->IsVisible()
						&& touch_interactive->IsInside(touch_event.position)
						&& touch_interactive->onTouch(*touch_interactive, touch_event)
					) {
						break;
					}
				}
			}
		}
	}

	#pragma endregion

	#pragma region Mouse

	void InputManager::AddMouseListener(MouseInteractive *listener) {
		if (_mouseListeners.Full()) {
			_mouseListeners.OffsetCapacity(32);
		}
		_mouseListeners.Push(listener);
	}

	void InputManager::RemoveMouseListener(MouseInteractive *listener) {
		_mouseListeners.EraseSwap(dmArrayUtil::GetIndexOf(&_mouseListeners, listener));
	}

	void InputManager::OnMouseClick(const dmGameObject::InputAction &action) {

	}

	void InputManager::OnMouseMove(const dmGameObject::InputAction &action) {
		InputEventPhase phase = InputEventPhase_Moved;
		const Vector2i mouse_position = Vector2i(action.m_ScreenX, action.m_ScreenY);
		const MouseEvent mouse_move_event(0, mouse_position, phase);
		const MouseEvent mouse_enter_event(input::MOUSE_ENTER, mouse_position, phase);
		const MouseEvent mouse_exit_event(input::MOUSE_EXIT, mouse_position, phase);
		// Tempopary array to safely iterate while the main array may change size.
		dmArray<MouseInteractive*> tmp_listeners;
		tmp_listeners.SetCapacity(_mouseListeners.Size());
		for (uint32_t i = 0; i < _mouseListeners.Size(); ++i) {
			tmp_listeners.Push(_mouseListeners[i]);
		}
		for (uint32_t i = 0; i < tmp_listeners.Size(); ++i) {
			MouseInteractive *mouse_interactive = tmp_listeners[i];
			bool is_taken = false;
			if (mouse_interactive->object->IsEnabled() && mouse_interactive->object->IsVisible()) {
				if (mouse_interactive->IsInside(mouse_position)) {
					if (!mouse_interactive->isOver) {
						mouse_interactive->isOver = true;
						mouse_interactive->onMouse(*mouse_interactive, mouse_enter_event);
					}
					if (!is_taken) {
						is_taken = mouse_interactive->onMouse(*mouse_interactive, mouse_move_event);
					}
				} else if (mouse_interactive->isOver) {
					mouse_interactive->isOver = false;
					mouse_interactive->onMouse(*mouse_interactive, mouse_exit_event);
				}
			}
		}
	}

	void InputManager::OnMouseWheel(const dmGameObject::InputAction &action) {

	}

	#pragma endregion

	#pragma region Key

	void InputManager::OnText(const dmGameObject::InputAction &action) {

	}

	void InputManager::AddKeyListener(KeyInteractive *listener) {
		if (_keyListeners.Full()) {
			_keyListeners.OffsetCapacity(32);
		}
		_keyListeners.Push(listener);
	}

	void InputManager::RemoveKeyListener(KeyInteractive *listener) {
		_keyListeners.EraseSwap(dmArrayUtil::GetIndexOf(&_keyListeners, listener));
	}

	void InputManager::OnKey(const dmGameObject::InputAction &action) {
		InputEventPhase phase;
		if (action.m_Pressed) {
			phase = InputEventPhase_Pressed;
		} else if (action.m_Released) {
			phase = InputEventPhase_Released;
		} else {
			return;
		}
		const KeyEvent key_event(action.m_ActionId, phase);
		std::sort(_keyListeners.Begin(), _keyListeners.End(), KeyInteractiveSortPred());
		for (uint32_t i = 0; i < _keyListeners.Size(); ++i) {
			KeyInteractive *key_interactive = _keyListeners[i];
			if (
				key_interactive->object->IsEnabled()
				&& key_interactive->object->IsVisible()
				&& key_interactive->onKey(*key_interactive, key_event)
			) {
				break;
			}
		}
	}

	#pragma endregion

	#pragma region Gamepad

	void InputManager::AddGamepadListener(GamepadInteractive *listener) {
		if (_gamepadListeners.Full()) {
			_gamepadListeners.OffsetCapacity(32);
		}
		_gamepadListeners.Push(listener);
	}

	void InputManager::RemoveGamepadListener(GamepadInteractive *listener) {
		_gamepadListeners.EraseSwap(dmArrayUtil::GetIndexOf(&_gamepadListeners, listener));
	}

	void InputManager::OnGamepad(const dmGameObject::InputAction &action) {
		if (action.m_ActionId == input::GAMEPAD_CONNECT) {
			_gamepadIndex = action.m_GamepadIndex;
			//dmLogInfo("gamepad connected, text count: %d, text: %s", action.m_TextCount, action.m_TextCount > 0 ? action.m_Text : "");
			//dmLogInfo("m_Gamepads driver, %d %d", hidContext->m_Gamepads[0].m_ButtonCount, hidContext->m_Gamepads[0].m_Connected);
		}
		if (action.m_GamepadIndex == _gamepadIndex) {
			InputEventPhase phase;
			if (action.m_Pressed) {
				phase = InputEventPhase_Pressed;
			} else if (action.m_Released) {
				phase = InputEventPhase_Released;
			} else {
				return;
			}
			const GamepadEvent gamepad_event(action.m_ActionId, phase, action.m_Value);
			std::sort(_gamepadListeners.Begin(), _gamepadListeners.End(), GamepadInteractiveSortPred());
			for (uint32_t i = 0; i < _gamepadListeners.Size(); ++i) {
				GamepadInteractive *gamepad_interactive = _gamepadListeners[i];
				if (
					gamepad_interactive->object->IsEnabled()
					&& gamepad_interactive->object->IsVisible()
					&& gamepad_interactive->onGamepad(*gamepad_interactive, gamepad_event)
				) {
					break;
				}
			}
		}
	}

	#pragma endregion

	void InputManager::OnInput(dmGameObject::InputAction &input_action) {
		/*if (input_action.m_ActionId == input::GAMEPAD_RAW) {
			dmLogInfo("raw ind %d, is gamepad %d, has packet %d%s%s", input_action.m_GamepadIndex, input_action.m_IsGamepad, input_action.m_HasGamepadPacket, input_action.m_Pressed ? ", pressed" : "", input_action.m_Released ? ", released" : "");
			if (input_action.m_GamepadPacket.m_GamepadConnected) {

			}
		}*/
		if (input_action.m_Pressed) {
			_actionDown.Put(input_action.m_ActionId, true);
		} else if (input_action.m_Released) {
			_actionDown.Put(input_action.m_ActionId, false);
		}
		if (Platform::isMobile) {
			if (input_action.m_ActionId == input::TOUCH) {
				OnTouch(input_action);
				_SetLastInputType(InputEventType_Touch);
			}
		} else {
			if (input_action.m_ActionId == 0) {
				OnMouseMove(input_action);
			} else if (Platform::isDesktop && input_action.m_ActionId == input::MOUSE_BUTTON_LEFT) {
				input_action.m_TouchCount = 1;
				input_action.m_Touch[0].m_X = input_action.m_X;
				input_action.m_Touch[0].m_Y = input_action.m_Y;
				input_action.m_Touch[0].m_DX = input_action.m_DX;
				input_action.m_Touch[0].m_DY = input_action.m_DY;
				input_action.m_Touch[0].m_ScreenX = input_action.m_ScreenX;
				input_action.m_Touch[0].m_ScreenY = input_action.m_ScreenY;
				input_action.m_Touch[0].m_ScreenDX = input_action.m_ScreenDX;
				input_action.m_Touch[0].m_ScreenDY = input_action.m_ScreenDY;
				input_action.m_Touch[0].m_Phase = input_action.m_Pressed != 0 ? dmHID::PHASE_BEGAN : (input_action.m_Released != 0 ? dmHID::PHASE_ENDED : dmHID::PHASE_MOVED);
				input_action.m_Touch[0].m_TapCount = 0;
				OnTouch(input_action);
				_SetLastInputType(InputEventType_Touch);
			} else if (input_action.m_ActionId == input::MOUSE_BUTTON_LEFT || input_action.m_ActionId == input::MOUSE_BUTTON_RIGHT) {
				OnMouseClick(input_action);
				_SetLastInputType(InputEventType_Mouse);
			} else if (input_action.m_ActionId == input::MOUSE_WHEEL_UP || input_action.m_ActionId == input::MOUSE_WHEEL_DOWN) {
				if (input_action.m_ActionId == input::MOUSE_WHEEL_DOWN) {
					input_action.m_Value = -input_action.m_Value;
				}
				OnMouseWheel(input_action);
			} else if (input_action.m_ActionId == input::TEXT) {
				OnText(input_action);
			} else if (dmHashtableUtil::Exists(&_keyActions, input_action.m_ActionId)) {
				OnKey(input_action);
				_SetLastInputType(InputEventType_Key);
				/*if (input_action.m_ActionId == input::KEY_ENTER) {
					memcpy(input_action.m_Text, "\n", 2);
					input_action.m_HasText = true;
					input_action.m_TextCount = 2;
					OnText(input_action);
				}*/
			} else if (dmHashtableUtil::Exists(&_gamepadActions, input_action.m_ActionId)) {
				OnGamepad(input_action);
				_SetLastInputType(InputEventType_Gamepad);
			}
		}
	}

	bool InputManager::IsActionDown(dmhash_t action_id) {
		bool *value = _actionDown.Get(action_id);
		if (value != nullptr) {
			return *value;
		} else {
			return false;
		}
	}

	bool InputManager::IsController() {
		return lastInputType == InputEventType_Key || lastInputType == InputEventType_Gamepad;
	}

	void InputManager::_SetLastInputType(InputEventType input_event_type) {
		if (lastInputType != input_event_type) {
			lastInputType = input_event_type;
			if (onInputTypeChanged != nullptr) {
				onInputTypeChanged(lastInputType);
			}
		}
	}
}
