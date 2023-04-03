#ifndef SK_GAMESYS_GAMESYS_DDF_H
#define SK_GAMESYS_GAMESYS_DDF_H

#include <dmsdk/ddf/ddf.h>

namespace dmDDF {
	struct Descriptor {
		uint64_t m_NameHash;
	};
}

namespace dmGameSystemDDF {
	struct PlayParticleFX {
		static dmDDF::Descriptor *m_DDFDescriptor;
	};

	struct StopParticleFX {
		static dmDDF::Descriptor *m_DDFDescriptor;
	};

	struct SetConstantParticleFX {
		static dmDDF::Descriptor *m_DDFDescriptor;
		dmhash_t m_EmitterId;
		dmhash_t m_NameHash;
		dmVMath::Matrix4 m_Value;
		bool m_IsMatrix4;
	};

	struct PlaySound {
		static dmDDF::Descriptor *m_DDFDescriptor;
		float m_Delay;
		float m_Gain;
		float m_Pan;
		float m_Speed;
		uint32_t m_PlayId;
	};

	struct StopSound {
		static dmDDF::Descriptor *m_DDFDescriptor;
	};

}

#endif
