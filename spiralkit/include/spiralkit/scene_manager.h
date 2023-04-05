#ifndef SK_SCENE_MANAGER_H
#define SK_SCENE_MANAGER_H

#include <godot/core/math/vector2i.h>
#include <dmsdk/dlib/array.h>
#include <dmsdk/dlib/hashtable.h>
#include <dmsdk/dlib/hash.h>
#include <spiralkit/asset.h>

namespace dmGameObject {
	typedef struct Instance* HInstance;
}

namespace spiralkit {
	class TouchInteractive;
	struct TouchEvent;
	class SkObject;
	class SkSprite;
	class Scene;

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
			);
			static void _AnimateTransition(Scene *scene, SceneTransition scene_transition, bool is_forward);
			static bool _OnTouchBlock(TouchInteractive &touch_interactive, const TouchEvent &touch_event);

		public:
			static void Init();
			static void Add(Scene *scene);
			static void Clear();
			static void Show(dmhash_t name_hash, SceneTransition scene_transition = SceneTransition_None, void *userdata = nullptr);
			static void ShowPopup(dmhash_t name_hash, SceneTransition scene_transition = SceneTransition_None, void *userdata = nullptr);
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
