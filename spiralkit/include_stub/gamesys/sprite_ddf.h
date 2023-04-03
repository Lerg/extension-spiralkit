#ifndef SK_GAMESYS_SPRITE_DDF_H
#define SK_GAMESYS_SPRITE_DDF_H

#include <dmsdk/ddf/ddf.h>

namespace dmGameSystemDDF {
	struct PlayAnimation {
		static dmDDF::Descriptor* m_DDFDescriptor;
		dmhash_t m_Id;
		float m_Offset;
		float m_PlaybackRate;
	};

	struct SetFlipHorizontal {
		static dmDDF::Descriptor* m_DDFDescriptor;
		uint32_t m_Flip;
	};

	struct SetFlipVertical {
		static dmDDF::Descriptor* m_DDFDescriptor;
		uint32_t m_Flip;
	};
}

#endif
