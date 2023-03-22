#ifndef SK_SKSPRITE_H
#define SK_SKSPRITE_H

#include <gamesys/gamesys_ddf.h>
#include <gamesys/sprite_ddf.h>
#include "asset.h"
#include "skobject.h"
#include "spiralkit.h"

namespace spiralkit {
	class SkSprite : public SkObject {
		public:
			dmMessage::URL componentUrl;
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

				PlayAnimation(sprite);
			};

			void PlayAnimation(asset::Sprite sprite, float offset = 0.0, float playback_rate = 1.0) {
				size = Vector2i(sprite.width, sprite.height);
				dmGameSystemDDF::PlayAnimation message;
				message.m_Id = sprite.animation_hash;
				message.m_Offset = offset;
				message.m_PlaybackRate = playback_rate;
				dmMessage::PostDDF(&message, &url, &componentUrl, (uintptr_t)instance, 0, nullptr);
			}
	};
}

#endif
