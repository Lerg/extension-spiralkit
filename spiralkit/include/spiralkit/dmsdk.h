#ifndef SK_DMSDK_H
#define SK_DMSDK_H

#include <dmsdk/gameobject/gameobject.h>
#include <dmsdk/lua/lua.h>
#include <dlib/easing.h>

namespace dmGameObject {
	void AcquireInputFocus(HCollection collection, HInstance instance);

	typedef void (*AnimationStopped)(dmGameObject::HInstance instance, dmhash_t component_id, dmhash_t property_id,
		bool finished, void* userdata1, void* userdata2);

	PropertyResult Animate(HCollection collection, HInstance instance, dmhash_t component_id,
		dmhash_t property_id,
		Playback playback,
		PropertyVar& to,
		dmEasing::Curve easing,
		float duration,
		float delay,
		AnimationStopped animation_stopped,
		void* userdata1, void* userdata2);

	PropertyResult GetProperty(HInstance instance, dmhash_t component_id, dmhash_t property_id, PropertyOptions options, PropertyDesc& out_value);
	PropertyResult SetProperty(HInstance instance, dmhash_t component_id, dmhash_t property_id, PropertyOptions options, const PropertyVar& value);
	PropertyResult CancelAnimations(HCollection collection, HInstance instance, dmhash_t component_id, dmhash_t property_id);
}

namespace dmMessage {
	//extern const uint32_t DM_MESSAGE_MAX_DATA_SIZE;
}

namespace dmScript {
	uint32_t CheckDDF(lua_State* L, const dmDDF::Descriptor* descriptor, char* buffer, uint32_t buffer_size, int index);
}

#endif
