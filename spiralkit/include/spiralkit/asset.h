#ifndef SK_ASSET_H
#define SK_ASSET_H

#include <dmsdk/dlib/hash.h>

namespace spiralkit {
	namespace asset {
		struct Asset {
			const char *path;
			Asset(const char *path): path(path) {}
		};
		struct Sprite : Asset {
			const char *animation;
			dmhash_t animation_hash;
			uint32_t width;
			uint32_t height;
			double aspect;
			Sprite(const char *path, const char *animation, uint32_t width, uint32_t height)
			: Asset(path), animation(animation), animation_hash(dmHashString64(animation))
			, width(width), height(height), aspect(width / height) {}
		};
		struct Label : Asset {
			uint32_t height;
			Label(const char *path, uint32_t height) : Asset(path), height(height) {}
		};
		struct ParticleFX : Asset {
			ParticleFX(const char *path) : Asset(path) {}
		};
	}
}

#endif
