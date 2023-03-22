#ifndef SK_GAMESYS_LABEL_DDF_H
#define SK_GAMESYS_LABEL_DDF_H

#include <dmsdk/ddf/ddf.h>

namespace dmGameSystemDDF {
	struct SetText {
		static dmDDF::Descriptor* m_DDFDescriptor;
		const char * m_Text;
	};
}

#endif
