#ifndef SK_SKSPRITE_H
#define SK_SKSPRITE_H


#include "asset.h"
#include "skobject.h"

namespace spiralkit {
	class SkSprite;
	typedef void (*OnAnimationEndCallback)(SkSprite *self);

	class SkSprite : public SkObject {
		//private:
			//int32_t _onAnimationEndFunctionLuaRef = 0;

		public:
			dmMessage::URL componentUrl;

		//private:
			/*static int32_t _luaOnAnimationEnd(lua_State *L) {
				dmLogInfo("end l");
				SkSprite *self = (SkSprite*)lua_topointer(L, lua_upvalueindex(1));
				if (self->onAnimationEnd != nullptr) {
					self->onAnimationEnd(self);
				}
				return 0;
			}*/

		//public:
			SkSprite(asset::Sprite sprite, SkObject *parent = nullptr) ;;
			void PlayAnimation(asset::Sprite sprite, float offset = 0.0, float playback_rate = 1.0) ;
			void SetHFlip(bool flip) ;
			void SetVFlip(bool flip) ;

	};
}

#endif
