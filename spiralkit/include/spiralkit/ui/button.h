#ifndef SK_UI_BUTTON_H
#define SK_UI_BUTTON_H

#include <functional>
#include "../sksprite.h"
#include "../sklabel.h"
#include "../touch_interactive.h"
#include "../mouse_interactive.h"

namespace spiralkit::ui {
	enum ButtonState {
		ButtonState_Default,
		ButtonState_Pressed
	};

	class Button; // Forward reference.
	typedef std::function<void(Button&)> ButtonCallback;
	typedef std::function<void(Button&, const TouchEvent&)> ButtonMoveCallback;

	class Button : public SkObject {
		private:
			asset::Sprite _defaultSprite, _pressedSprite;
			SkSprite *_sprite = nullptr;
			SkLabel *_label = nullptr;
			TouchInteractive *_touchInteractive = nullptr;
			MouseInteractive *_mouseInteractive = nullptr;
			ButtonState _state = ButtonState_Default;
			bool _isWorkAroundExecuted = false;
			static bool _OnTouchDefault(TouchInteractive &touch_interactive, const TouchEvent &touch_event);
			static bool _OnMouseDefault(MouseInteractive &mouse_interactive, const MouseEvent &mouse_event);

		public:
			ButtonCallback onPress = nullptr;
			ButtonMoveCallback onMove = nullptr;
			ButtonCallback onRelease = nullptr;

			Button(asset::Sprite default_sprite, asset::Sprite pressed_sprite, asset::Label label, SkObject *parent = nullptr) {
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

			~Button() {
				Delete();
			}

			void Delete() {
				delete _touchInteractive;
				delete _mouseInteractive;
				SkObject::Delete();
			}

			inline void SetState(ButtonState state) {
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

			inline SkLabel *GetLabel() {
				return _label;
			}

			inline void SetScale(float scale) {
				SkObject::SetScale(scale);
			}

			inline void SetSprites(asset::Sprite default_sprite, asset::Sprite pressed_sprite) {
				_defaultSprite = default_sprite;
				_pressedSprite = pressed_sprite;
				SetState(_state);
			}

			void SetIsHighlighted(bool is_highlight) {
				_sprite->AnimateComponent(_sprite->componentUrl.m_Fragment, spiralkit::hashes::effects_exposure, dmGameObject::PropertyVar(is_highlight ? 1.5 : 1), is_highlight ? 0.5 : 0.2, 0, dmGameObject::PLAYBACK_ONCE_FORWARD, dmEasing::TYPE_OUTQUAD);
			}
	};
}

#endif
