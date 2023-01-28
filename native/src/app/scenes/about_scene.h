#ifndef APP_ABOUT_SCENE_H
#define APP_ABOUT_SCENE_H

#include <spiralkit/scene_manager.h>
#include <spiralkit/ui/button.h>
#include <spiralkit/ui/ui.h>
#include "../app.h"
#include "../assets.h"
#include "../hashes.h"

using namespace spiralkit;

namespace app {
	class AboutScene : public Scene {
		private:
			SkSprite _background;
			ui::Button _backButton;
			SkLabel _label;

		public:
			#pragma region Scene

			AboutScene()
			: Scene(hashes::about)
			, _background(SkSprite(assets::sprites::background, this))
			, _backButton(ui::Button(assets::sprites::buttons$button, assets::sprites::buttons$button_pressed, assets::labels::main, this))
			, _label(SkLabel(assets::labels::main, this)) {
				_backButton.GetLabel()->SetText("BACK");
				_label.SetText("This is a sample game for the SpiralKit\nNice.");

				_backButton.onRelease = [this](ui::Button &button) {
					SceneManager::Show(hashes::menu, SceneTransition_SlideRight);
				};
			}

			void OnWillShow(void *userdata) {
				dmLogInfo("About will be shown");
			}

			void OnDidHide() {
			}

			void OnUpdate(float dt) {
			}

			void OnResize(Vector2i screen_size) {
				ui::LetterboxFit(&_background, screen_size);

				float scale = 25 * (float)assets::sprites::buttons$button.width / screen_size.width;
				_backButton.SetPosition(Vector2(-0.35 * screen_size.width, 0.35 * screen_size.height));
				_backButton.SetScale(scale);

				_label.SetScale(0.2 * scale);
			}

			#pragma endregion

		private:
	};
}

#endif
