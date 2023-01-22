#include <stdint.h>
#include <stddef.h>

#include <dmsdk/extension/extension.h>
#include "extension.h"

#include "app/app.h"

static app::App *app_instance = nullptr;

static int extension_init(lua_State *L) {
	app_instance = new app::App();
	return 0;
}

static const luaL_reg extension_functions[] = {
	{"init", extension_init},
	{0, 0}
};

dmExtension::Result APP_INITIALIZE(dmExtension::AppParams *params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result APP_FINALIZE(dmExtension::AppParams *params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result INITIALIZE(dmExtension::Params *params) {
	luaL_register(params->m_L, EXTENSION_NAME_STRING, extension_functions);
	lua_pop(params->m_L, 1);
	return dmExtension::RESULT_OK;
}

dmExtension::Result UPDATE(dmExtension::Params *params) {
	return dmExtension::RESULT_OK;
}

dmExtension::Result FINALIZE(dmExtension::Params *params) {
	delete app_instance;
	return dmExtension::RESULT_OK;
}

DECLARE_DEFOLD_EXTENSION