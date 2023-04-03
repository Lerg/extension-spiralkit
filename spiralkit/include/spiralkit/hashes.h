#ifndef SK_HASHES_H
#define SK_HASHES_H

#include <dmsdk/dlib/hash.h>

namespace spiralkit::hashes {
	inline const dmhash_t component = dmHashString64("component");
	inline const dmhash_t position = dmHashString64("position");
	inline const dmhash_t position_x = dmHashString64("position.x");
	inline const dmhash_t position_y = dmHashString64("position.y");
	inline const dmhash_t position_z = dmHashString64("position.z");
	inline const dmhash_t scale = dmHashString64("scale");
	inline const dmhash_t scale_x = dmHashString64("scale.x");
	inline const dmhash_t scale_y = dmHashString64("scale.y");
	inline const dmhash_t scale_z = dmHashString64("scale.z");
	inline const dmhash_t rotation = dmHashString64("rotation");
	inline const dmhash_t rotation_x = dmHashString64("rotation.x");
	inline const dmhash_t rotation_y = dmHashString64("rotation.y");
	inline const dmhash_t rotation_z = dmHashString64("rotation.z");
	inline const dmhash_t euler = dmHashString64("euler");
	inline const dmhash_t euler_x = dmHashString64("euler.x");
	inline const dmhash_t euler_y = dmHashString64("euler.y");
	inline const dmhash_t euler_z = dmHashString64("euler.z");
	inline const dmhash_t tint = dmHashString64("tint");
	inline const dmhash_t tint_x = dmHashString64("tint.x");
	inline const dmhash_t tint_y = dmHashString64("tint.y");
	inline const dmhash_t tint_z = dmHashString64("tint.z");
	inline const dmhash_t tint_w = dmHashString64("tint.w");
	inline const dmhash_t effects_exposure = dmHashString64("effects.x");
	inline const dmhash_t color = dmHashString64("color");
	inline const dmhash_t color_x = dmHashString64("color.x");
	inline const dmhash_t color_y = dmHashString64("color.y");
	inline const dmhash_t color_z = dmHashString64("color.z");
	inline const dmhash_t color_w = dmHashString64("color.w");
	inline const dmhash_t outline = dmHashString64("outline");
	inline const dmhash_t outline_x = dmHashString64("outline.x");
	inline const dmhash_t outline_y = dmHashString64("outline.y");
	inline const dmhash_t outline_z = dmHashString64("outline.z");
	inline const dmhash_t outline_w = dmHashString64("outline.w");
}

#endif
