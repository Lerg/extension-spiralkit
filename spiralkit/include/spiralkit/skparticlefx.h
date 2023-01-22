#ifndef SK_SKPARTICLEFX_H
#define SK_SKPARTICLEFX_H

#include <gamesys/gamesys_ddf.h>
#include "asset.h"
#include "skobject.h"

namespace spiralkit {
	class SkParticleFX : public SkObject {
		public:
			dmMessage::URL componentUrl;
			SkParticleFX(asset::ParticleFX particlefx, SkObject *parent = nullptr) {
				identifier = Defold::FactoryCreate(particlefx.path);
				this->parent = parent;
				size = Vector2i(0, 0);
				const dmGameObject::HInstance go_instance = dmScript::CheckGOInstance(spiralkit::Defold::L);
				const dmGameObject::HCollection collection = dmGameObject::GetCollection(go_instance);
				instance = dmGameObject::GetInstanceFromIdentifier(collection, identifier);

				Init();
				dmMessage::ResetURL(&componentUrl);
				componentUrl.m_Socket = dmGameObject::GetMessageSocket(collection);
				componentUrl.m_Path = identifier;
				componentUrl.m_Fragment = hashes::component;
			};

			void Play() {
				dmGameSystemDDF::PlayParticleFX message;
				dmMessage::PostDDF(&message, &url, &componentUrl, (uintptr_t)instance, 0, nullptr);
			}

			void Stop() {
				dmGameSystemDDF::StopParticleFX message;
				dmMessage::PostDDF(&message, &url, &componentUrl, (uintptr_t)instance, 0, nullptr);
			}
	};
}

#endif
