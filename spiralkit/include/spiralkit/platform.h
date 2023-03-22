#ifndef SK_PLATFORM_H
#define SK_PLATFORM_H

namespace spiralkit {
	class Platform {
		public:
			static const char pathSeparator;
			static char applicationPath[4096];
			static bool isAndroid;
			static bool isiOS;
			static bool isHTML5;
			static bool isMobile;
			static bool isDesktop;
	};
}

#endif
