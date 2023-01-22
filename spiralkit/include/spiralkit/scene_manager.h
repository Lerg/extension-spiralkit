#ifndef SK_SCENE_MANAGER_H
#define SK_SCENE_MANAGER_H

#include <godot/core/math/vector2i.h>
#include <dmsdk/dlib/array.h>
#include <dmsdk/dlib/hashtable.h>
#include <dmsdk/vectormath/cpp/vectormath_aos.h>
#include <dmsdk/dlib/hash.h>
#include "scene.h"

namespace spiralkit {
	class TouchInteractive;
	struct TouchEvent;
	class SkSprite;

	static void OnResizeIterateCallback(Vector2i *screen_size, const dmhash_t *key, Scene **scene) {
		(*scene)->OnResize(*screen_size);
	}

	enum SceneTransition {
		SceneTransition_None,
		SceneTransition_SlideLeft,
		SceneTransition_SlideRight,
		SceneTransition_SlideDown,
		SceneTransition_SlideUp
	};

	class SceneManager {
		private:
			static dmHashTable64<Scene*> _loadedScenes;
			static Scene *_currentScene;
			static dmArray<Scene*> _popups;
			static dmArray<SkSprite*> _popupShades;
			static SkObject *_root;
			static TouchInteractive *_touchInteractive;
		public:
			static asset::Sprite popupShadeAsset;
			static Vector2i screenSize;

		private:
			SceneManager();

			static void _OnAnimateTransitionStopped(
				dmGameObject::HInstance instance,
				dmhash_t component_id,
				dmhash_t property_id,
				bool finished,
				void *userdata1, // Scene *scene
				void *userdata2 // bool is_forward
			) {
				Scene *scene = (Scene *)userdata1;
				const bool is_forward = (bool)userdata2;
				if (is_forward) {
					scene->OnDidShow();
				} else {
					scene->SetIsVisible(false);
					scene->OnDidHide();
				}
			}

			static void _AnimateTransition(Scene *scene, SceneTransition scene_transition, bool is_forward) {
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
					dmGameObject::GetCollection(scene->instance), scene->instance, 0,
					property_id, playback, to,
					dmEasing::Curve(easing_curve),
					0.2, 0,
					_OnAnimateTransitionStopped, (void*)scene, (void*)is_forward
				);
			}

			static bool _OnTouchBlock(TouchInteractive &touch_interactive, const TouchEvent &touch_event);

		public:
			static void Init();

			inline static void Add(Scene *scene) {
				if (_loadedScenes.Full()) {
					uint32_t capacity = _loadedScenes.Capacity();
					_loadedScenes.SetCapacity(2 / 3 * capacity, 2 * capacity);
				}
				_loadedScenes.Put(scene->name, scene);
				scene->SetParent(_root);
				scene->SetIsVisible(false);
			}

			static void Show(dmhash_t name_hash, SceneTransition scene_transition = SceneTransition_None, void *userdata = nullptr) {
				if (_currentScene != nullptr) {
					if (_currentScene->name == name_hash) {
						_currentScene->OnWillHide();
						_currentScene->OnDidHide();
					} else {
						_currentScene->OnWillHide();
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

			static void ShowPopup(dmhash_t name_hash, SceneTransition scene_transition, void *userdata);

			static void HidePopup(SceneTransition scene_transition);

			static void OnUpdate(float dt);

			static void OnResize(Vector2i screen_size);

			inline static Vector2i ScreenToWorld(Vector2i position) {
				return position - screenSize / 2;
			}

			inline static Vector2i WorldToScreen(Vector2i position) {
				return position + screenSize / 2;
			}
	};
}

#endif
