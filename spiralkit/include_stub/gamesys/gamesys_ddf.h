#ifndef SK_GAMESYS_GAMESYS_DDF_H
#define SK_GAMESYS_GAMESYS_DDF_H

#include <dmsdk/ddf/ddf.h>

namespace dmGameSystemDDF {
	struct PlayParticleFX {
		static dmDDF::Descriptor* m_DDFDescriptor;
	};

	struct StopParticleFX {
		static dmDDF::Descriptor* m_DDFDescriptor;
	};
}

#endif
