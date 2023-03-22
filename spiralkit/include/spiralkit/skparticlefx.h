#ifndef SK_SKPARTICLEFX_H
#define SK_SKPARTICLEFX_H

#include <gamesys/gamesys_ddf.h>
#include "asset.h"
#include "skobject.h"
#include "spiralkit.h"

namespace spiralkit {
	class SkParticleFX : public SkObject {
		public:
			dmMessage::URL componentUrl;

		private:
			void SetConstant(dmhash_t emitter_id, dmhash_t name, dmVMath::Matrix4 value, bool is_matrix4_type) {
				dmGameSystemDDF::SetConstantParticleFX message;
				message.m_EmitterId = emitter_id;
				message.m_NameHash = name;
				message.m_Value = value;
				message.m_IsMatrix4 = is_matrix4_type;

				dmMessage::Post(
					&url,
					&componentUrl,
					dmGameSystemDDF::SetConstantParticleFX::m_DDFDescriptor->m_NameHash,
					(uintptr_t)instance,
					0,
					(uintptr_t)dmGameSystemDDF::SetConstantParticleFX::m_DDFDescriptor,
					&message,
					sizeof(message),
					0
				);
			}

		public:
			SkParticleFX(asset::ParticleFX particlefx, SkObject *parent = nullptr) {
				identifier = Defold::FactoryCreate(particlefx.path);
				this->parent = parent;
				size = Vector2i(0, 0);
				instance = dmGameObject::GetInstanceFromIdentifier(Spiralkit::collection, identifier);

				Init();
				dmMessage::ResetURL(&componentUrl);
				componentUrl.m_Socket = Spiralkit::urlSocket;
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

			void SetConstant(dmhash_t emitter_id, dmhash_t name, dmVMath::Matrix4 value) {
				SetConstant(emitter_id, name, value, true);
			}

			void SetConstant(dmhash_t emitter_id, dmhash_t name, dmVMath::Vector4 value) {
				dmVMath::Matrix4 matrix_value;
				matrix_value.setCol0(value);
				SetConstant(emitter_id, name, matrix_value, false);
			}

			void SetAlpha(dmhash_t emitter_id, float alpha) {
				SetConstant(emitter_id, hashes::tint, dmVMath::Vector4(1, 1, 1, alpha));
			}
	};
}

#endif
