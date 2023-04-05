#include <dmsdk/dlib/log.h>
#include <dmsdk/dlib/dstrings.h>
#include <dlib/path.h>
#include <dlib/sys.h>

#include <spiralkit/defold.h>

namespace spiralkit {
	lua_State *Defold::L = nullptr;
	uint32_t Defold::scriptInstanceRef = 0;

	InstanceIdentifierPair::InstanceIdentifierPair(dmGameObject::HInstance instance, dmhash_t identifier)
	: instance(instance), identifier(identifier) {}

	dmhash_t Defold::FactoryCreate(const char *url) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, scriptInstanceRef);
		dmScript::SetInstance(L);
		lua_getglobal(L, "factory");
		lua_getfield(L, -1, "create");
		lua_remove(L, -2); //factory
		lua_pushstring(L, url);
		lua_call(L, 1, 1);
		return dmScript::CheckHash(L, -1);
	}

	InstanceIdentifierPair Defold::NewGameObject() {
		const dmGameObject::HCollection collection = Spiralkit::collection;
		const uint32_t index = dmGameObject::AcquireInstanceIndex(collection);
		if (index == dmGameObject::INVALID_INSTANCE_POOL_INDEX) {
			dmLogError("Failed to acquire instance index for scene root object.");
		}
		const dmhash_t identifier = dmGameObject::ConstructInstanceId(index);

		const dmGameObject::HInstance instance = dmGameObject::New(collection, nullptr);
		dmGameObject::AssignInstanceIndex(index, instance);

		const dmGameObject::Result result = dmGameObject::SetIdentifier(collection, instance, identifier);
		if (dmGameObject::RESULT_OK != result) {
			dmLogError("Failed to set identifier for scene root object.");
		}
		return InstanceIdentifierPair(instance, identifier);
	}

	const char *Defold::GetSystemName() {
		lua_getglobal(L, "sys");
		lua_getfield(L, -1, "get_sys_info");
		lua_remove(L, -2); //sys
		lua_call(L, 0, 1);

		lua_getfield(L, -1, "system_name");
		lua_remove(L, -2); //sys_info
		const char *system_name = lua_tostring(L, -1);
		lua_pop(L, 1);
		return system_name;
	}

	void Defold::GetSaveFile(const char *application_id, const char *filename, char *path, size_t path_length) {
		dmSys::Result r = dmSys::GetApplicationSavePath(application_id, path, path_length);
		if (r != dmSys::RESULT_OK) {
			dmLogError("Unable to locate application support path for \"%s\": (%d)", application_id, r);
			return;
		}

		char *dm_home = dmSys::GetEnv("DM_SAVE_HOME");
		// Higher priority
		if (dm_home) {
			dmStrlCpy(path, dm_home, path_length);
		}

		dmStrlCat(path, dmPath::PATH_CHARACTER, path_length);
		dmStrlCat(path, filename, path_length);
	}
}
