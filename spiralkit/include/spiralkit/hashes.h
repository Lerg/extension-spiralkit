#ifndef SK_HASHES_H
#define SK_HASHES_H

#include <dmsdk/dlib/hash.h>

namespace spiralkit::hashes {
	static const dmhash_t component = dmHashString64("component");
	static const dmhash_t position = dmHashString64("position");
	static const dmhash_t position_x = dmHashString64("position.x");
	static const dmhash_t position_y = dmHashString64("position.y");
	static const dmhash_t position_z = dmHashString64("position.z");
	static const dmhash_t scale = dmHashString64("scale");
	static const dmhash_t scale_x = dmHashString64("scale.x");
	static const dmhash_t scale_y = dmHashString64("scale.y");
	static const dmhash_t scale_z = dmHashString64("scale.z");
	static const dmhash_t rotation = dmHashString64("rotation");
	static const dmhash_t rotation_x = dmHashString64("rotation.x");
	static const dmhash_t rotation_y = dmHashString64("rotation.y");
	static const dmhash_t rotation_z = dmHashString64("rotation.z");
	static const dmhash_t euler = dmHashString64("euler");
	static const dmhash_t euler_x = dmHashString64("euler.x");
	static const dmhash_t euler_y = dmHashString64("euler.y");
	static const dmhash_t euler_z = dmHashString64("euler.z");
	static const dmhash_t tint = dmHashString64("tint");
	static const dmhash_t tint_x = dmHashString64("tint.x");
	static const dmhash_t tint_y = dmHashString64("tint.y");
	static const dmhash_t tint_z = dmHashString64("tint.z");
	static const dmhash_t tint_w = dmHashString64("tint.w");
}

#endif
