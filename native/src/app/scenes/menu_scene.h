#ifndef APP_MENU_SCENE_H
#define APP_MENU_SCENE_H

#include <spiralkit/scene_manager.h>
#include <spiralkit/ui/button.h>
#include <spiralkit/ui/ui.h>
#include "../app.h"
#include "../assets.h"
#include "../hashes.h"

using namespace spiralkit;

namespace app {
	class MenuScene : public Scene {
		private:
			SkSprite _background;
			ui::Button _playButton;
			ui::Button _aboutButton;
			dmArray<ui::Button*> _buttons;

		public:
			#pragma region Scene

			MenuScene()
			: Scene(hashes::menu)
			, _background(SkSprite(assets::sprites::background, this))
			, _playButton(ui::Button(assets::sprites::buttons$button, assets::sprites::buttons$button_pressed, assets::labels::main, this))
			, _aboutButton(ui::Button(assets::sprites::buttons$button, assets::sprites::buttons$button_pressed, assets::labels::main, this)) {
				_buttons.SetCapacity(3);
				_buttons.Push(&_playButton);
				_buttons.Push(&_aboutButton);
				_playButton.GetLabel()->SetText("PLAY");
				_aboutButton.GetLabel()->SetTextF("%s", "ABOUT");

				_playButton.onRelease = [this](ui::Button &button) {
					SceneManager::Show(hashes::play, SceneTransition_SlideDown);
				};

				_aboutButton.onRelease = [this](ui::Button &button) {
					SceneManager::Show(hashes::about, SceneTransition_SlideLeft);
				};
			}

			void OnDidShow() {
				dmLogInfo("Menu did show");
			}

			void OnDidHide() {
			}

			void OnUpdate(float dt) {
			}

			void OnResize(Vector2i screen_size) {
				ui::LetterboxFit(&_background, screen_size);

				float scale = 50 * (float)assets::sprites::buttons$button.width / screen_size.width;
				for (uint32_t i = 0; i < _buttons.Size(); ++i) {
					_buttons[i]->SetPosition(Vector2(-0.25 * screen_size.width, (0.25 - 0.5 * i) * screen_size.height));
					_buttons[i]->SetScale(scale);
				}
			}

			#pragma endregion

		private:
	};
}

#endif
