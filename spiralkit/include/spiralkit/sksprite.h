#ifndef SK_SKSPRITE_H
#define SK_SKSPRITE_H

#include <gamesys/gamesys_ddf.h>
#include <gamesys/sprite_ddf.h>
#include "asset.h"
#include "skobject.h"

namespace spiralkit {
	class SkSprite;
	typedef void (*OnAnimationEndCallback)(SkSprite *self);

	class SkSprite : public SkObject {
		private:
			//int32_t _onAnimationEndFunctionLuaRef = 0;

		public:
			dmMessage::URL componentUrl;

		private:
			/*static int32_t _luaOnAnimationEnd(lua_State *L) {
				dmLogInfo("end l");
				SkSprite *self = (SkSprite*)lua_topointer(L, lua_upvalueindex(1));
				if (self->onAnimationEnd != nullptr) {
					self->onAnimationEnd(self);
				}
				return 0;
			}*/

		public:
			//OnAnimationEndCallback onAnimationEnd = nullptr;

			SkSprite(asset::Sprite sprite, SkObject *parent = nullptr) {
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
			};

			void PlayAnimation(asset::Sprite sprite, float offset = 0.0, float playback_rate = 1.0) {
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

			void SetHFlip(bool flip) {
				dmGameSystemDDF::SetFlipHorizontal message;
				message.m_Flip = (uint32_t)flip;
				dmMessage::PostDDF(&message, &url, &componentUrl, (uintptr_t)instance, 0, nullptr);
			}

			void SetVFlip(bool flip) {
				dmGameSystemDDF::SetFlipVertical message;
				message.m_Flip = (uint32_t)flip;
				dmMessage::PostDDF(&message, &url, &componentUrl, (uintptr_t)instance, 0, nullptr);
			}

		private:
			/*void _SetupLuaCallback() {
				lua_State *L = dmScript::GetMainThread(Defold::L);
				lua_pushlightuserdata(L, (void*)this);
				lua_pushcclosure(L, _luaOnAnimationEnd, 1);
				_onAnimationEndFunctionLuaRef = dmScript::RefInInstance(L) - LUA_NOREF;
				dmLogInfo("setup %d", _onAnimationEndFunctionLuaRef);
			}*/
	};
}

#endif
