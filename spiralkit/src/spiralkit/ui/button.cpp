#include <spiralkit/ui/button.h>
#include <spiralkit/input_manager.h>

namespace spiralkit::ui {
	bool Button::_OnTouchDefault(TouchInteractive &touch_interactive, const TouchEvent &touch_event) {
		Button *button = (Button *)touch_interactive.object;
		if (touch_event.phase == InputEventPhase_Pressed) {
			InputManager::SetTouchFocus(touch_event.id, &touch_interactive);
			touch_interactive.isFocused = true;
			button->SetState(ButtonState_Pressed);
			if (button->onPress != nullptr) {
				button->onPress(*button);
			}
			return true;
		} else if (touch_interactive.isFocused) {
			const bool is_inside = touch_interactive.IsInside(touch_event.position);
			if (touch_event.phase == InputEventPhase_Released) {
				InputManager::ReleaseTouchFocus(touch_event.id);
				touch_interactive.isFocused = false;
				button->SetState(ButtonState_Default);
				if (button->onRelease != nullptr && is_inside) {
					button->onRelease(*button);
				}
			} else {
				button->SetState(is_inside ? ButtonState_Pressed : ButtonState_Default);
				if (button->onMove != nullptr) {
					button->onMove(*button, touch_event);
				}
			}
			return true;
		}
		return false;
	}
}
