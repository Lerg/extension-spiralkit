#include <dmsdk/dlib/log.h>
#include <spiralkit/hashes.h>
#include <spiralkit/defold.h>
#include <spiralkit/scene.h>
#include <spiralkit/sksprite.h>
#include <spiralkit/touch_interactive.h>

#include <spiralkit/scene_manager.h>

namespace spiralkit {
	dmHashTable64<Scene*> SceneManager::_loadedScenes;
	Scene *SceneManager::_currentScene = nullptr;
	dmArray<Scene*> SceneManager::_popups;
	dmArray<SkSprite*> SceneManager::_popupShades;
	Vector2i SceneManager::screenSize = Vector2i();
	SkObject *SceneManager::_root = nullptr;
	asset::Sprite SceneManager::popupShadeAsset = asset::Sprite("", "", 0, 0);
	TouchInteractive *SceneManager::_touchInteractive = nullptr;

	void SceneManager::_OnAnimateTransitionStopped(
		dmGameObject::HInstance instance,
		dmhash_t component_id,
		dmhash_t property_id,
		bool finished,
		void *userdata1, // Scene *scene
		void *userdata2 // bool is_forward
	) {
		Scene *scene = (Scene*)userdata1;
		const bool is_forward = (bool)userdata2;
		if (is_forward) {
			scene->OnDidShow();
		} else {
			scene->SetIsVisible(false);
			scene->OnDidHide();
		}
	}

	void SceneManager::_AnimateTransition(Scene *scene, SceneTransition scene_transition, bool is_forward) {
		dmhash_t property_id = 0;
		const dmGameObject::Playback playback = is_forward ? dmGameObject::PLAYBACK_ONCE_FORWARD : dmGameObject::PLAYBACK_ONCE_BACKWARD;
		const dmEasing::Type easing_curve = is_forward ? dmEasing::TYPE_OUTQUAD : dmEasing::TYPE_INQUAD;
		dmGameObject::PropertyVar to = dmGameObject::PropertyVar(0.0);
		Vector2 position = {0, 0};
		switch (scene_transition) {
			case SceneTransition_None:
				scene->ToFront();
				scene->SetPosition(position);
				_OnAnimateTransitionStopped(scene->instance, 0, property_id, true, (void*)scene, (void*)is_forward);
				return;
			case SceneTransition_SlideLeft:
				property_id = hashes::position_x;
				position.x = (is_forward ? 1 : -1) * screenSize.width;
				break;
			case SceneTransition_SlideRight:
				property_id = hashes::position_x;
				position.x = (is_forward ? -1 : 1) * screenSize.width;
				break;
			case SceneTransition_SlideDown:
				property_id = hashes::position_y;
				position.y = (is_forward ? 1 : -1) * screenSize.height;
				break;
			case SceneTransition_SlideUp:
				property_id = hashes::position_y;
				position.y = (is_forward ? -1 : 1) * screenSize.height;
				break;
		}
		scene->ToFront();
		scene->SetPosition(position);
		dmGameObject::Animate(
			Spiralkit::collection, scene->instance, 0,
			property_id, playback, to,
			dmEasing::Curve(easing_curve),
			0.2, 0,
			_OnAnimateTransitionStopped, (void*)scene, (void*)is_forward
		);
	}

	bool SceneManager::_OnTouchBlock(TouchInteractive &touch_interactive, const TouchEvent &touch_event) {
		return true;
	}

	void SceneManager::Init() {
		_currentScene = nullptr;
		screenSize = {1024, 1024};
		_loadedScenes.SetCapacity(21, 32);
		_popups.SetCapacity(32);
		_popupShades.SetCapacity(32);
		if (_root == nullptr) {
			const InstanceIdentifierPair root_game_object = Defold::NewGameObject();
			_root = new SkObject(root_game_object.instance, root_game_object.identifier);
			_touchInteractive = new TouchInteractive(_root);
			_touchInteractive->onTouch = _OnTouchBlock;
		}
	}

	void SceneManager::Add(Scene *scene) {
		if (_loadedScenes.Full()) {
			uint32_t capacity = _loadedScenes.Capacity();
			_loadedScenes.SetCapacity(2 / 3 * capacity, 2 * capacity);
		}
		_loadedScenes.Put(scene->name, scene);
		scene->SetParent(_root);
		scene->SetIsVisible(false);
		scene->SetPosition(Vector2(10000, 0));
	}

	void SceneManager::Show(dmhash_t name_hash, SceneTransition scene_transition, void *userdata) {
		if (_currentScene != nullptr) {
			_currentScene->OnWillHide();
			if (_currentScene->name == name_hash) {
				_currentScene->OnDidHide();
			} else {
				_AnimateTransition(_currentScene, scene_transition, false);
			}
		}
		Scene **entry = _loadedScenes.Get(name_hash);
		if (entry != nullptr) {
			_currentScene = *entry;
			_currentScene->OnWillShow(userdata);
			_currentScene->SetIsVisible(true);
			_currentScene->OnResize(screenSize);
			_AnimateTransition(_currentScene, scene_transition, true);
		} else {
			dmLogError("SceneManager::Show() no such scene: %s", dmHashReverseSafe64(name_hash));
		}
	}

	void SceneManager::Clear() {
		_currentScene = nullptr;
		_loadedScenes.Clear();
		_popups.SetSize(0);
		_popupShades.SetSize(0);
		delete _touchInteractive;
		delete _root;
		_root = nullptr;
	}

	void SceneManager::ShowPopup(dmhash_t name_hash, SceneTransition scene_transition, void *userdata) {
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

	void SceneManager::HidePopup(SceneTransition scene_transition) {
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
