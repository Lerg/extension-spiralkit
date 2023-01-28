#include <dmsdk/dlib/hash.h>
#include <dmsdk/gameobject/script.h>
#include <dmsdk/gameobject/component.h>

#include <spiralkit/spiralkit.h>
#include "assets.h"
#include "scenes/menu_scene.h"
#include "scenes/about_scene.h"
#include "scenes/play_scene.h"

using namespace spiralkit;

namespace app {
	App::App() {
		const dmGameObject::HInstance go_instance = dmScript::CheckGOInstance(Defold::L);
		const dmGameObject::HCollection collection = dmGameObject::GetCollection(go_instance);
		dmGameObject::AcquireInputFocus(collection, go_instance);

		SceneManager::popupShadeAsset = assets::sprites::popup_shade;
		_menuScene = new MenuScene();
		_aboutScene = new AboutScene();
		_playScene = new PlayScene();
		SceneManager::Add(_menuScene);
		SceneManager::Add(_aboutScene);
		SceneManager::Add(_playScene);
		SceneManager::Show(_menuScene->name);
	}

	App::~App() {}

	void App::Update(float dt) {}
}
