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
			dmArray<ui::Button*> _buttons;

		public:
			#pragma region Scene

			MenuScene()
			: Scene(hashes::menu)
			, _background(SkSprite(assets::sprites::background, this)) {
				_buttons.SetCapacity(3);
			}

			void OnWillShow(void *userdata) {
			}

			void OnDidHide() {
			}

			void OnUpdate(float dt) {
			}

			void OnResize(Vector2i screen_size) {
				ui::ZoomFit(&_background, screen_size);
			}

			#pragma endregion

		private:
	};
}

#endif
