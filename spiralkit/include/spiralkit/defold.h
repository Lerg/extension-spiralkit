#ifndef SK_DEFOLD_H
#define SK_DEFOLD_H

#include "asset.h"
#include "spiralkit.h"

namespace spiralkit {
	struct InstanceIdentifierPair {
		dmGameObject::HInstance instance;
		dmhash_t identifier;
		InstanceIdentifierPair(dmGameObject::HInstance instance, dmhash_t identifier);
	};

	class Defold {
		public:
			static lua_State *L;
			static uint32_t scriptInstanceRef;
			static const uint32_t DM_MESSAGE_MAX_DATA_SIZE = 3072U;
			static dmhash_t FactoryCreate(const char *url);
			static InstanceIdentifierPair NewGameObject();
			static const char *GetSystemName();
			static void GetSaveFile(const char *application_id, const char *filename, char *path, size_t path_length) ;
	};
}

#endif
