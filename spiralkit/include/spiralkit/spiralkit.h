#ifndef SK_SPIRALKIT_H
#define SK_SPIRALKIT_H

#include <godot/core/math/vector2i.h>
#include <dmsdk/gameobject/component.h>
#include "input_manager.h"
#include "scene_manager.h"
#include "sound_manager.h"
#include <string.h>

namespace spiralkit {
	class Spiralkit {
		private:
			Spiralkit();

		public:
			static dmGameObject::HInstance instance;
			static dmGameObject::HCollection collection;
			static dmMessage::HSocket urlSocket;

			static void Init() {
				InputManager::Init();
				SceneManager::Init();
				SoundManager::Init();
				const char *system_name = Defold::GetSystemName();
				Platform::isAndroid = strcmp(system_name, "Android") == 0;
				Platform::isiOS = strcmp(system_name, "iPhone OS") == 0;
				Platform::isHTML5 = strcmp(system_name, "HTML5") == 0;
				Platform::isMobile = Platform::isAndroid || Platform::isiOS;
				Platform::isDesktop = !Platform::isMobile;

				dmSys::GetApplicationPath(Platform::applicationPath, sizeof(Platform::applicationPath));
				char *last_separator = strrchr(Platform::applicationPath, Platform::pathSeparator);
				if (last_separator != nullptr) {
					*last_separator = 0;
				}
			}

			static void SetInstance(dmGameObject::HInstance instance) {
				Spiralkit::instance = instance;
				collection = dmGameObject::GetCollection(instance);
				urlSocket = dmGameObject::GetMessageSocket(collection);
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
