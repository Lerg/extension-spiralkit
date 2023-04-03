#ifndef SK_SPIRALKIT_H
#define SK_SPIRALKIT_H

#include <dmsdk/gameobject/component.h>

namespace spiralkit {
	class Spiralkit {
		private:
			Spiralkit();

		public:
			static dmGameObject::HInstance instance;
			static dmGameObject::HCollection collection;
			static dmMessage::HSocket urlSocket;

			static void Init();

			static void SetInstance(dmGameObject::HInstance instance) {
				Spiralkit::instance = instance;
				collection = dmGameObject::GetCollection(instance);
				urlSocket = dmGameObject::GetMessageSocket(collection);
			}

			static void OnUpdate(float dt);

			static void OnInput(const dmGameObject::ComponentOnInputParams &params);

			static void OnResize(uint32_t width, uint32_t height);
	};
}

#endif
