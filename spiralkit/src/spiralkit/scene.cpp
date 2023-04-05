#include <spiralkit/defold.h>

#include <spiralkit/scene.h>

namespace spiralkit {
	Scene::Scene(dmhash_t name)
	: name(name) {
		const InstanceIdentifierPair game_object = Defold::NewGameObject();
		instance = game_object.instance;
		identifier = game_object.identifier;
		Init();
	}
}
