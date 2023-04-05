#ifndef SK_SKPARTICLEFX_H
#define SK_SKPARTICLEFX_H

#include "asset.h"
#include "skobject.h"

namespace spiralkit {
	class SkParticleFX : public SkObject {
		public:
			dmMessage::URL componentUrl;

		private:
			void SetConstant(dmhash_t emitter_id, dmhash_t name, dmVMath::Matrix4 value, bool is_matrix4_type);

		public:
			SkParticleFX(asset::ParticleFX particlefx, SkObject *parent = nullptr);
			void Play();
			void Stop();
			void SetConstant(dmhash_t emitter_id, dmhash_t name, dmVMath::Matrix4 value);
			void SetConstant(dmhash_t emitter_id, dmhash_t name, dmVMath::Vector4 value);
			void SetAlpha(dmhash_t emitter_id, float alpha);
	};
}

#endif
