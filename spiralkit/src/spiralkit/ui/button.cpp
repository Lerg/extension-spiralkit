#include <spiralkit/defold.h>
#include <spiralkit/hashes.h>
#include <spiralkit/input.h>
#include <spiralkit/input_manager.h>
#include <spiralkit/mouse_interactive.h>
#include <spiralkit/touch_interactive.h>

#include <spiralkit/ui/button.h>

namespace spiralkit::ui {
	Button::Button(asset::Sprite default_sprite, asset::Sprite pressed_sprite, asset::Label label, SkObject *parent) {
		InstanceIdentifierPair game_object = Defold::NewGameObject();
		instance = game_object.instance;
		identifier = game_object.identifier;
		this->parent = parent;
		Init();
		_defaultSprite = default_sprite;
		_pressedSprite = pressed_sprite;
		_sprite = new SkSprite(_defaultSprite, this);
		size = _sprite->size;
		_label = new SkLabel(label, this);
		_label->SetScale(0.25 * size.height / label.height);

		_touchInteractive = new TouchInteractive(this);
		_touchInteractive->onTouch = _OnTouchDefault;
		_mouseInteractive = new MouseInteractive(this);
		_mouseInteractive->onMouse = _OnMouseDefault;
	}

	Button::~Button() {
		Delete();
	}

	void Button::Delete() {
		delete _touchInteractive;
		delete _mouseInteractive;
		SkObject::Delete();
	}

	void Button::SetState(ButtonState state) {
		_state = state;
		switch (state) {
			case ButtonState_Default:
				_sprite->PlayAnimation(_defaultSprite);
				break;
			case ButtonState_Pressed:
				_sprite->PlayAnimation(_pressedSprite);
				break;
		}
	}

	void Button::SetIsHighlighted(bool is_highlight) {
		_sprite->AnimateComponent(_sprite->componentUrl.m_Fragment, hashes::effects_exposure, dmGameObject::PropertyVar(is_highlight ? 1.5 : 1), is_highlight ? 0.5 : 0.2, 0, dmGameObject::PLAYBACK_ONCE_FORWARD, dmEasing::TYPE_OUTQUAD);
	}

	bool Button::_OnTouchDefault(TouchInteractive &touch_interactive, const TouchEvent &touch_event) {
		Button *button = (Button*)touch_interactive.object;
		if (touch_event.phase == InputEventPhase_Pressed) {
			InputManager::SetTouchFocus(touch_event.id, &touch_interactive);
			touch_interactive.isFocused = true;
			button->SetState(ButtonState_Pressed);
			if (button->onPress) {
				button->onPress(*button);
			}
			return true;
		} else if (touch_interactive.isFocused) {
			const bool is_inside = touch_interactive.IsInside(touch_event.position);
			if (touch_event.phase == InputEventPhase_Released) {
				InputManager::ReleaseTouchFocus(touch_event.id);
				touch_interactive.isFocused = false;
				button->SetState(ButtonState_Default);
				if (button->onRelease && is_inside) {
					button->onRelease(*button);
				}
			} else {
				button->SetState(is_inside ? ButtonState_Pressed : ButtonState_Default);
				if (button->onMove) {
					button->onMove(*button, touch_event);
				}
			}
			return true;
		}
		return false;
	}

	bool Button::_OnMouseDefault(MouseInteractive &mouse_interactive, const MouseEvent &mouse_event) {
		Button *button = (Button*)mouse_interactive.object;
		if (mouse_event.phase == InputEventPhase_Moved) {
			if (button->_isWorkAroundExecuted) {
				if (mouse_event.id == input::MOUSE_ENTER) {
					button->SetIsHighlighted(true);
				} else if (mouse_event.id == input::MOUSE_EXIT) {
					button->SetIsHighlighted(false);
				}
			} else {
				button->_isWorkAroundExecuted = true;
				mouse_interactive.isOver = false;
			}
		}
		return true;
	}
}
