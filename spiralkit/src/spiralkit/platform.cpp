#include <spiralkit/platform.h>

namespace spiralkit {
	#ifdef DM_PLATFORM_WINDOWS
		const char Platform::pathSeparator = '\\';
	#else
		const char Platform::pathSeparator = '/';
	#endif
	char Platform::applicationPath[4096];
	bool Platform::isAndroid = false;
	bool Platform::isiOS = false;
	bool Platform::isHTML5 = false;
	bool Platform::isMobile = false;
	bool Platform::isDesktop = false;
}
