#include <gamesys/gamesys_ddf.h>
#include <gamesys/sprite_ddf.h>
#include <dmsdk/dlib/log.h>
#include <spiralkit/hashes.h>
#include <spiralkit/defold.h>

#include <spiralkit/sksprite.h>

namespace spiralkit {
	SkSprite::SkSprite(asset::Sprite sprite, SkObject *parent) {
		identifier = Defold::FactoryCreate(sprite.path);
		this->parent = parent;
		size = Vector2i(sprite.width, sprite.height);
		instance = dmGameObject::GetInstanceFromIdentifier(Spiralkit::collection, identifier);

		Init();
		dmMessage::ResetURL(&componentUrl);
		componentUrl.m_Socket = Spiralkit::urlSocket;
		componentUrl.m_Path = identifier;
		componentUrl.m_Fragment = hashes::component;

		//_SetupLuaCallback();

		PlayAnimation(sprite);
	}

	/*void _SetupLuaCallback() {
		lua_State *L = dmScript::GetMainThread(Defold::L);
		lua_pushlightuserdata(L, (void*)this);
		lua_pushcclosure(L, _luaOnAnimationEnd, 1);
		_onAnimationEndFunctionLuaRef = dmScript::RefInInstance(L) - LUA_NOREF;
		dmLogInfo("setup %d", _onAnimationEndFunctionLuaRef);
	}*/

	void SkSprite::PlayAnimation(asset::Sprite sprite, float offset, float playback_rate) {
		size = Vector2i(sprite.width, sprite.height);
		/*if (onAnimationEnd != nullptr && _onAnimationEndFunctionLuaRef == 0) {
			_SetupLuaCallback();
		}*/
		dmGameSystemDDF::PlayAnimation message;
		message.m_Id = sprite.animation_hash;
		message.m_Offset = offset;
		message.m_PlaybackRate = playback_rate;
		//dmMessage::PostDDF(&message, &url, &componentUrl, (uintptr_t)instance, onAnimationEnd != nullptr ? (uintptr_t)_onAnimationEndFunctionLuaRef : 0, nullptr);
		dmMessage::PostDDF(&message, &url, &componentUrl, (uintptr_t)instance, 0, nullptr);
	}

	void SkSprite::SetHFlip(bool flip) {
		dmGameSystemDDF::SetFlipHorizontal message;
		message.m_Flip = (uint32_t)flip;
		dmMessage::PostDDF(&message, &url, &componentUrl, (uintptr_t)instance, 0, nullptr);
	}

	void SkSprite::SetVFlip(bool flip) {
		dmGameSystemDDF::SetFlipVertical message;
		message.m_Flip = (uint32_t)flip;
		dmMessage::PostDDF(&message, &url, &componentUrl, (uintptr_t)instance, 0, nullptr);
	}
}
