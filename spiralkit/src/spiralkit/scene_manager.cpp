#include <spiralkit/scene_manager.h>
#include <spiralkit/touch_interactive.h>
#include <spiralkit/sksprite.h>

namespace spiralkit {
	dmHashTable64<Scene*> SceneManager::_loadedScenes;
	Scene *SceneManager::_currentScene = nullptr;
	dmArray<Scene*> SceneManager::_popups;
	dmArray<SkSprite*> SceneManager::_popupShades;
	Vector2i SceneManager::screenSize = Vector2i();
	SkObject *SceneManager::_root = nullptr;
	asset::Sprite SceneManager::popupShadeAsset = asset::Sprite("", "", 0, 0);
	TouchInteractive *SceneManager::_touchInteractive = nullptr;

	bool SceneManager::_OnTouchBlock(TouchInteractive &touch_interactive, const TouchEvent &touch_event) {
		return true;
	}

	void SceneManager::Init() {
		_currentScene = nullptr;
		screenSize = {1024, 1024};
		_loadedScenes.SetCapacity(21, 32);
		_popups.SetCapacity(32);
		_popupShades.SetCapacity(32);
		const InstanceIdentifierPair root_game_object = Defold::NewGameObject();
		_root = new SkObject(root_game_object.instance, root_game_object.identifier);
		_touchInteractive = new TouchInteractive(_root);
		_touchInteractive->onTouch = _OnTouchBlock;
	}

	void SceneManager::ShowPopup(dmhash_t name_hash, SceneTransition scene_transition = SceneTransition_None, void *userdata = nullptr) {
		Scene **entry = _loadedScenes.Get(name_hash);
		if (entry != nullptr) {
			SkSprite *popup_shade = new SkSprite(popupShadeAsset, _root);
			if (_popupShades.Full()) {
				_popupShades.SetCapacity(2 * _popupShades.Capacity());
			}
			_popupShades.Push(popup_shade);
			popup_shade->SetScale(Vector2((float)screenSize.width / popupShadeAsset.width, (float)screenSize.height / popupShadeAsset.height));
			_touchInteractive->object = popup_shade;

			Scene *scene = *entry;
			if (_popups.Full()) {
				_popups.SetCapacity(2 * _popups.Capacity());
			}
			_popups.Push(scene);
			scene->OnWillShow(userdata);
			scene->SetIsVisible(true);
			scene->OnResize(screenSize);
			_AnimateTransition(scene, scene_transition, true);
		} else {
			dmLogError("SceneManager::ShowPopup() no such scene: %s", dmHashReverseSafe64(name_hash));
		}
	}

	void SceneManager::HidePopup(SceneTransition scene_transition = SceneTransition_None) {
		if (_popups.Size() > 0) {
			SkSprite *popup_shade = _popupShades[_popupShades.Size() - 1];
			_popupShades.Pop();
			delete popup_shade;
			if (_popupShades.Size() > 0) {
				_touchInteractive->object = _popupShades[_popupShades.Size() - 1];
			} else {
				_touchInteractive->object = _root;
			}

			Scene *scene = _popups[_popups.Size() - 1];
			_popups.Pop();
			scene->OnWillHide();
			_AnimateTransition(scene, scene_transition, false);
		}
	}

	void SceneManager::OnUpdate(float dt) {
		if (_currentScene != nullptr) {
			_currentScene->OnUpdate(dt);
		}
		for (uint32_t i = 0; i < _popups.Size(); ++i) {
			Scene *popup = _popups[i];
			if (popup->IsVisible()) {
				popup->OnUpdate(dt);
			}
		}
	}

	void SceneManager::OnResize(Vector2i screen_size) {
		screenSize = screen_size;
		if (_currentScene != nullptr) {
			_currentScene->OnResize(screen_size);
		}
		for (uint32_t i = 0; i < _popupShades.Size(); ++i) {
			_popupShades[i]->SetScale(Vector2((float)screenSize.width / popupShadeAsset.width, (float)screenSize.height / popupShadeAsset.height));
		}
		for (uint32_t i = 0; i < _popups.Size(); ++i) {
			Scene *popup = _popups[i];
			if (popup->IsVisible()) {
				popup->OnResize(screen_size);
			}
		}
	}
}
