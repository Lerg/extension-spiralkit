#ifndef SK_UI_BUTTON_H
#define SK_UI_BUTTON_H

#include <functional>
#include "../sksprite.h"
#include "../sklabel.h"
#include "../touch_interactive.h"

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
			SkSprite *_defaultImage = nullptr, *_pressedImage = nullptr;
			SkLabel *_label = nullptr;
			TouchInteractive *_touchInteractive = nullptr;
			ButtonState _state = ButtonState_Default;
			static bool _OnTouchDefault(TouchInteractive &touch_interactive, const TouchEvent &touch_event);

		public:
			ButtonCallback onPress = nullptr;
			ButtonMoveCallback onMove = nullptr;
			ButtonCallback onRelease = nullptr;

			Button(asset::Sprite default_image, asset::Sprite pressed_image, asset::Label label, SkObject *parent = nullptr) {
				InstanceIdentifierPair game_object = Defold::NewGameObject();
				instance = game_object.instance;
				identifier = game_object.identifier;
				this->parent = parent;
				Init();
				_defaultImage = new SkSprite(default_image, this);
				_pressedImage = new SkSprite(pressed_image, this);
				_pressedImage->SetIsVisible(false);
				size = _defaultImage->size;
				_label = new SkLabel(label, this);
				_label->SetScale(0.25 * size.height / label.height);
				_touchInteractive = new TouchInteractive(this);
				_touchInteractive->onTouch = _OnTouchDefault;
			}

			~Button() {
				Delete();
			}

			void Delete() {
				delete _defaultImage;
				delete _pressedImage;
				delete _label;
				delete _touchInteractive;
			}

			inline void SetState(ButtonState state) {
				switch (state) {
					case ButtonState_Default:
						_defaultImage->SetIsVisible(true);
						_pressedImage->SetIsVisible(false);
						break;
					case ButtonState_Pressed:
						_defaultImage->SetIsVisible(false);
						_pressedImage->SetIsVisible(true);
						break;
				}
			}

			inline SkLabel *GetLabel() {
				return _label;
			}

			inline void SetScale(float scale) {
				SkObject::SetScale(scale);
			}

			inline void SetSprites(asset::Sprite default_image, asset::Sprite pressed_image) {
				_defaultImage->PlayAnimation(default_image);
				_pressedImage->PlayAnimation(pressed_image);
			}
	};
}

#endif
