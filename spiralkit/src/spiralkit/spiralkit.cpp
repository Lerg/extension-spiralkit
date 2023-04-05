#include <string.h>
#include <dlib/sys.h>
#include <spiralkit/input_manager.h>
#include <spiralkit/platform.h>
#include <spiralkit/scene_manager.h>
#include <spiralkit/sound_manager.h>

#include <spiralkit/spiralkit.h>

namespace spiralkit {
	dmGameObject::HInstance Spiralkit::instance;
	dmGameObject::HCollection Spiralkit::collection;
	dmMessage::HSocket Spiralkit::urlSocket;

	void Spiralkit::Init() {
		InputManager::Init();
		SceneManager::Init();
		SoundManager::Init();

		dmSys::SystemInfo sys_info;
		dmSys::GetSystemInfo(&sys_info);

		const char *system_name = sys_info.m_SystemName;
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

		dmSys::EngineInfo engine_info;
		dmSys::GetEngineInfo(&engine_info);
		Platform::isDebug = engine_info.m_IsDebug;
	}

	void Spiralkit::SetInstance(dmGameObject::HInstance instance) {
		Spiralkit::instance = instance;
		collection = dmGameObject::GetCollection(instance);
		urlSocket = dmGameObject::GetMessageSocket(collection);
	}

	void Spiralkit::OnUpdate(float dt) {
		SceneManager::OnUpdate(dt);
	}

	void Spiralkit::OnInput(const dmGameObject::ComponentOnInputParams &params) {
		dmGameObject::InputAction input_action = *params.m_InputAction;
		InputManager::OnInput(input_action);
	}

	void Spiralkit::OnResize(uint32_t width, uint32_t height) {
		SceneManager::OnResize(Vector2i(width, height));
	}
}
