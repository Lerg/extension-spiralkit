#ifndef SK_ASSET_H
#define SK_ASSET_H

#include <dmsdk/dlib/hash.h>

namespace spiralkit {
	namespace asset {
		struct Asset {
			const char *path = nullptr;
			Asset() {}
			Asset(const char *path): path(path) {}
		};

		struct Sprite : Asset {
			const char *animation = nullptr;
			dmhash_t animation_hash = 0;
			uint32_t width = 0;
			uint32_t height = 0;
			double aspect = 0;
			Sprite() {}
			Sprite(const char *path, const char *animation, uint32_t width, uint32_t height)
			: Asset(path), animation(animation), animation_hash(dmHashString64(animation))
			, width(width), height(height), aspect(width / height) {}
		};

		struct Label : Asset {
			uint32_t height = 0;
			Label(const char *path, uint32_t height) : Asset(path), height(height) {}
		};

		struct Sound : Asset {
			dmhash_t name_hash = 0;
			Sound(const char *path, const char *name) : Asset(path), name_hash(dmHashString64(name)) {}
		};

		struct ParticleFX : Asset {
			ParticleFX(const char *path) : Asset(path) {}
		};
	}
}

#endif
