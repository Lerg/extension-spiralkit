#include <stdint.h>
#include <stddef.h>

#include <dmsdk/extension/extension.h>
#include <dmsdk/engine/extension.h>
#include <spiralkit/spiralkit.h>
#include <spiralkit/signal_manager.h>
#include <spiralkit/defold.h>
#include <spiralkit/input_manager.h>

#include "extension.h"

static const char *sys_load_resource(lua_State *L, const char *filename, size_t *out_length) {
	lua_getglobal(L, "sys");
	lua_getfield(L, -1, "load_resource");
	lua_remove(L, -2); //sys
	lua_pushstring(L, filename);
	lua_call(L, 1, 1);
	return lua_tolstring(L, -1, out_length);
}

static int extension_init(lua_State *L) {
	spiralkit::Defold::L = L;
	dmScript::GetInstance(L);
	spiralkit::Defold::scriptInstanceRef = dmScript::Ref(L, LUA_REGISTRYINDEX);
	spiralkit::Spiralkit::Init();
	return 0;
}

static int on_window_resize(lua_State *L) {
	uint32_t width = (uint32_t)lua_tonumber(L, 1);
	uint32_t height = (uint32_t)lua_tonumber(L, 2);
	spiralkit::Spiralkit::OnResize(width, height);
	return 0;
}

static const luaL_reg extension_functions[] = {
	{"init", extension_init},
	{"on_window_resize", on_window_resize},
	{0, 0}
};

dmExtension::Result APP_INITIALIZE(dmExtension::AppParams *params) {
	spiralkit::InputManager::hidContext = dmEngine::GetHIDContext(params);
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
	spiralkit::Defold::L = params->m_L;
	return dmExtension::RESULT_OK;
}

dmExtension::Result FINALIZE(dmExtension::Params *params) {
	spiralkit::SignalManager::finalize();
	return dmExtension::RESULT_OK;
}

DECLARE_DEFOLD_EXTENSION
