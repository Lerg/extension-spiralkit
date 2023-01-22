#ifndef APP_HASHES_H
#define APP_HASHES_H

#include <spiralkit/asset.h>

using namespace spiralkit;

namespace app {
	namespace hashes {
		static const dmhash_t play = dmHashString64("play");
		static const dmhash_t menu = dmHashString64("menu");
		static const dmhash_t about = dmHashString64("about");
	}
}

#endif
