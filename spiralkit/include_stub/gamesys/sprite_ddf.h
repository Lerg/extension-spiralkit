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
}

#endif
