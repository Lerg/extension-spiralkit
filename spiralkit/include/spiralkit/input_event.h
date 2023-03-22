#ifndef SK_INPUT_EVENT_H
#define SK_INPUT_EVENT_H

#include <godot/core/math/vector2i.h>
#include <dmsdk/gameobject/gameobject.h>

namespace spiralkit {
	enum InputEventType {
		InputEventType_Touch,
		InputEventType_Mouse,
		InputEventType_Key,
		InputEventType_Gamepad
	};

	enum InputEventPhase {
		InputEventPhase_Pressed,
		InputEventPhase_Moved,
		InputEventPhase_Released
	};

	struct InputEvent {
		InputEventType type;
		InputEvent(InputEventType type): type(type) {}
	};

	struct TouchEvent : InputEvent {
		uint64_t id = 0;
		Vector2i position;
		InputEventPhase phase;
		TouchEvent(uint64_t id, Vector2i position, InputEventPhase phase)
		: InputEvent(InputEventType_Touch), id(id), position(position), phase(phase) {}
	};

	struct KeyEvent : InputEvent {
		uint64_t id = 0;
		InputEventPhase phase;
		KeyEvent(uint64_t id, InputEventPhase phase)
		: InputEvent(InputEventType_Key), id(id), phase(phase) {}
	};

	struct MouseEvent : InputEvent {
		uint64_t id = 0;
		Vector2i position;
		InputEventPhase phase;
		MouseEvent(uint64_t id, Vector2i position, InputEventPhase phase)
		: InputEvent(InputEventType_Mouse), id(id), position(position), phase(phase) {}
	};

	struct GamepadEvent : InputEvent {
		uint64_t id = 0;
		InputEventPhase phase;
		float value = 0;
		GamepadEvent(uint64_t id, InputEventPhase phase, float value)
		: InputEvent(InputEventType_Gamepad), id(id), phase(phase), value(value) {}
	};
}

#endif
