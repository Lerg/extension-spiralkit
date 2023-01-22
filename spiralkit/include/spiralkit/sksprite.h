#ifndef SK_SKSPRITE_H
#define SK_SKSPRITE_H

#include <gamesys/gamesys_ddf.h>
#include <gamesys/sprite_ddf.h>
#include "asset.h"
#include "skobject.h"

namespace spiralkit {
	class SkSprite : public SkObject {
		public:
			dmMessage::URL componentUrl;
			SkSprite(asset::Sprite sprite, SkObject *parent = nullptr) {
				identifier = Defold::FactoryCreate(sprite.path);
				this->parent = parent;
				size = Vector2i(sprite.width, sprite.height);
				const dmGameObject::HInstance go_instance = dmScript::CheckGOInstance(spiralkit::Defold::L);
				const dmGameObject::HCollection collection = dmGameObject::GetCollection(go_instance);
				instance = dmGameObject::GetInstanceFromIdentifier(collection, identifier);

				Init();
				dmMessage::ResetURL(&componentUrl);
				componentUrl.m_Socket = dmGameObject::GetMessageSocket(collection);
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
