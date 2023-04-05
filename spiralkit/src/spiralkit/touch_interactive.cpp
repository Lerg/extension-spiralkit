#include <spiralkit/input_manager.h>
#include <spiralkit/scene_manager.h>
#include <spiralkit/skobject.h>

#include <spiralkit/touch_interactive.h>

namespace spiralkit {
	bool TouchInteractive::OnTouchDefault(TouchInteractive &touch_interactive, const TouchEvent &touch_event) {
		if (touch_event.phase == InputEventPhase_Pressed) {
			InputManager::SetTouchFocus(touch_event.id, &touch_interactive);
			touch_interactive.isFocused = true;
			if (touch_interactive.onPress != nullptr) {
				touch_interactive.onPress(touch_interactive, touch_event);
			}
			return true;
		} else if (touch_interactive.isFocused) {
			if (touch_event.phase == InputEventPhase_Released) {
				InputManager::ReleaseTouchFocus(touch_event.id);
				touch_interactive.isFocused = false;
				if (touch_interactive.onRelease != nullptr) {
					touch_interactive.onRelease(touch_interactive, touch_event);
				}
			} else {
				if (touch_interactive.onMove != nullptr) {
					touch_interactive.onMove(touch_interactive, touch_event);
				}
			}
			return true;
		}
		return false;
	}

	TouchInteractive::TouchInteractive(SkObject *object)
	: object(object), onTouch(OnTouchDefault), onPress(nullptr), onMove(nullptr), onRelease(nullptr), isFocused(false) {
		InputManager::AddTouchListener(this);
	}

	TouchInteractive::~TouchInteractive() {
		InputManager::RemoveTouchListener(this);
	}

	bool TouchInteractive::IsInside(Vector2i position) {
		Vector2 object_position = object->GetWorldPosition();
		Vector2 object_scale = object->GetWorldScale();
		Vector2 object_world_size = object_scale * object->size;
		Vector2i world_position = SceneManager::ScreenToWorld(position);
		return world_position.x > object_position.x - object_world_size.width / 2
			&& world_position.y > object_position.y - object_world_size.height / 2
			&& world_position.x < object_position.x + object_world_size.width / 2
			&& world_position.y < object_position.y + object_world_size.height / 2;
	}
}
