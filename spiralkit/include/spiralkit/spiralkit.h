#ifndef SK_SPIRALKIT_H
#define SK_SPIRALKIT_H

#include <godot/core/math/vector2i.h>
#include <dmsdk/gameobject/component.h>
#include "input_manager.h"
#include "scene_manager.h"

namespace spiralkit {
	class Spiralkit {
		private:
			Spiralkit();

		public:
			static void Init() {
				InputManager::Init();
				SceneManager::Init();
				const char *system_name = Defold::GetSystemName();
				Platform::isAndroid = strcmp(system_name, "Android") == 0;
				Platform::isiOS = strcmp(system_name, "iPhone OS") == 0;
				Platform::isHTML5 = strcmp(system_name, "HTML5") == 0;
				Platform::isMobile = Platform::isAndroid || Platform::isiOS;
				Platform::isDesktop = !Platform::isMobile;
			}

			inline static void OnUpdate(float dt) {
				SceneManager::OnUpdate(dt);
			}

			inline static void OnInput(const dmGameObject::ComponentOnInputParams &params) {
				dmGameObject::InputAction input_action = *params.m_InputAction;
				InputManager::OnInput(input_action);
			}

			inline static void OnResize(uint32_t width, uint32_t height) {
				SceneManager::OnResize(Vector2i(width, height));
			}
	};
}

#endif
