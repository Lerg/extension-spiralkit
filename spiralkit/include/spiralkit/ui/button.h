#ifndef SK_UI_BUTTON_H
#define SK_UI_BUTTON_H

#include <functional>
#include "../input_event.h"
#include "../sksprite.h"
#include "../sklabel.h"

namespace spiralkit {
	class TouchInteractive;
	class MouseInteractive;
}

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
			ButtonCallback onPress;
			ButtonMoveCallback onMove;
			ButtonCallback onRelease;

			Button(asset::Sprite default_sprite, asset::Sprite pressed_sprite, asset::Label label, SkObject *parent = nullptr);

			~Button();

			void Delete();

			void SetState(ButtonState state);

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

			void SetIsHighlighted(bool is_highlight);
	};
}

#endif
