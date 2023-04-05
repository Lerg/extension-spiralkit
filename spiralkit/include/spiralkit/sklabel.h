#ifndef SK_SKLABEL_H
#define SK_SKLABEL_H

#include "asset.h"
#include "skobject.h"

namespace spiralkit {
	class SkLabel : public SkObject {
		public:
			dmMessage::URL componentUrl;
			SkLabel(asset::Label asset, SkObject *parent = nullptr);
			void SetText(const char *text);
			void SetTextF(const char *format, ...);
	};
}

#endif
