#include <spiralkit/input_manager.h>
#include <spiralkit/scene_manager.h>
#include <spiralkit/skobject.h>

namespace spiralkit {
	bool MouseInteractive::OnMouseDefault(MouseInteractive &mouse_interactive, const MouseEvent &mouse_event) {
		if (mouse_event.phase == InputEventPhase_Pressed) {
			if (mouse_interactive.onPress != nullptr) {
				mouse_interactive.onPress(mouse_interactive, mouse_event);
				return true;
			}
		} else if (mouse_event.phase == InputEventPhase_Released) {
			if (mouse_interactive.onRelease != nullptr) {
				mouse_interactive.onRelease(mouse_interactive, mouse_event);
				return true;
			}
		} else if (mouse_event.phase == InputEventPhase_Moved) {
			if (mouse_interactive.onMove != nullptr) {
				if (mouse_interactive._isWorkAroundExecuted) {
					mouse_interactive.onMove(mouse_interactive, mouse_event);
				} else {
					mouse_interactive._isWorkAroundExecuted = true;
					mouse_interactive.isOver = false;
				}
				return true;
			}
		}
		return false;
	}

	MouseInteractive::MouseInteractive(SkObject *object)
	: object(object), onMouse(OnMouseDefault), onPress(nullptr), onMove(nullptr), onRelease(nullptr) {
		InputManager::AddMouseListener(this);
	}

	MouseInteractive::~MouseInteractive() {
		InputManager::RemoveMouseListener(this);
	}

	bool MouseInteractive::IsInside(Vector2i position) {
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
