#ifndef SK_SCENE_H
#define SK_SCENE_H

#include <dmsdk/script/script.h>
#include <dmsdk/gameobject/gameobject.h>
#include <dmsdk/gamesys/script.h>
#include "skobject.h"

namespace spiralkit {
	class Scene : public SkObject {
		private:
		public:
			dmhash_t name;
			Scene(dmhash_t name)
			: name(name) {
				const InstanceIdentifierPair game_object = Defold::NewGameObject();
				instance = game_object.instance;
				identifier = game_object.identifier;
				Init();
			};
			~Scene() {};
			virtual void OnWillShow(void *userdata) {};
			virtual void OnDidShow() {};
			virtual void OnWillHide() {};
			virtual void OnDidHide() {};
			virtual void OnUpdate(float dt) {};
			virtual void OnResize(Vector2i screen_size) {};
			virtual void OnDestroy() {};
	};
}

#endif
