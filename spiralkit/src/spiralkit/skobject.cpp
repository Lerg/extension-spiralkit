#include <gameobject/gameobject_ddf.h>
#include <spiralkit/array.h>
#include <spiralkit/defold.h>
#include <spiralkit/hashes.h>

#include <spiralkit/skobject.h>

namespace spiralkit {
	inline static void SetPositionZ(SkObject *instance, float z) {
		dmVMath::Point3 position = instance->GetPosition3D();
		position.setZ(z);
		instance->SetPosition3D(position);
	}

	inline static void SetScaleZ(SkObject *instance, float z) {
		dmVMath::Vector3 scale = instance->GetScale3D();
		scale.setZ(z);
		instance->SetScale3D(scale);
	}

	static bool _CheckPropertyResult(dmGameObject::PropertyResult result) {
		switch (result) {
			case dmGameObject::PROPERTY_RESULT_OK:
				return true;
			case dmGameObject::PROPERTY_RESULT_NOT_FOUND:
				dmLogError("No such property");
				break;
			case dmGameObject::PROPERTY_RESULT_UNSUPPORTED_TYPE:
			case dmGameObject::PROPERTY_RESULT_TYPE_MISMATCH:
				dmLogError("Wrong property type");
				break;
			case dmGameObject::PROPERTY_RESULT_COMP_NOT_FOUND:
				dmLogError("Component not found");
				break;
			case dmGameObject::PROPERTY_RESULT_UNSUPPORTED_OPERATION:
				dmLogError("Unsupported operation");
				break;
				default:
				dmLogError("Failed with error code %d", result);
		}
		return false;
	}

	SkObject::SkObject()
	: instance(nullptr), identifier(0), parent(nullptr) {}

	SkObject::SkObject(SkObject *parent) {
		InstanceIdentifierPair game_object = Defold::NewGameObject();
		instance = game_object.instance;
		identifier = game_object.identifier;
		this->parent = parent;
		Init();
	}

	SkObject::SkObject(dmGameObject::HInstance instance, dmhash_t identifier, SkObject *parent)
	: instance(instance), identifier(identifier), parent(parent) {
		Init();
	}

	SkObject::~SkObject() {
		Delete();
	}

	void SkObject::Init() {
		if (parent != nullptr) {
			SetParent(parent);
		}
		dmMessage::ResetURL(&url);
		url.m_Socket = Spiralkit::urlSocket;
		url.m_Path = identifier;
	}

	void SkObject::AcquireInputFocus() {
		dmGameObject::AcquireInputFocus(Spiralkit::collection, instance);
	}

	void SkObject::Insert(SkObject *child, int32_t index) {
		assert(child != nullptr && child->instance != nullptr);
		if (child->parent != nullptr) {
			child->parent->Remove(child);
		}
		dmGameObject::SetParent(child->instance, instance);
		child->parent = this;
		if (children.Full()) {
			children.OffsetCapacity(32);
		}
		if (index != -1) {
			dmArrayUtil::Insert(&children, index, child);
			for (uint32_t i = 0; i < children.Size(); ++i) {
				SkObject *c = children[i];
				SetPositionZ(c, i);
			}
		} else {
			children.Push(child);
			SetPositionZ(child, children.Size());
		}
		SetScaleZ(this, 1.0 / (children.Size() + 1));
	}

	void SkObject::Remove(SkObject *child) {
		assert(child != nullptr && child->instance != nullptr);
		bool is_after = false;
		int32_t child_index = -1;
		for (uint32_t i = 0; i < children.Size(); ++i) {
			SkObject *c = children[i];
			if (is_after) {
				SetPositionZ(c, i);
			} else if (c == child) {
				is_after = true;
				child_index = i;
				child->parent = nullptr;
				dmGameObject::SetParent(child->instance, 0);
			}
		}
		if (child_index >= 0) {
			dmArrayUtil::Remove(&children, (uint32_t)child_index);
			SetScaleZ(this, 1.0 / (children.Size() + 1));
		}
	}

	void SkObject::ToBack(SkObject *child) {
		uint32_t index = dmArrayUtil::GetIndexOf(&children, child);
		if (index == 0) {
			return;
		}
		dmArrayUtil::Move(&children, index, 0);
		for (uint32_t i = 0; i <= index; ++i) {
			SetPositionZ(children[i], i);
		}
	}

	void SkObject::ToFront(SkObject *child) {
		uint32_t index = dmArrayUtil::GetIndexOf(&children, child);
		dmArrayUtil::Move(&children, index, children.Size() - 1);
		for (uint32_t i = index; i < children.Size(); ++i) {
			SetPositionZ(children[i], i);
		}
	}

	void SkObject::SetIsEnabled(bool is_enabled) {
		if (_isEnabled == _isVisible && is_enabled) {
			return;
		}
		_isEnabled = _isVisible && is_enabled;
		if (_isEnabled) {
			const dmGameObjectDDF::Enable message;
			dmMessage::PostDDF(&message, &url, &url, (uintptr_t)instance, 0, nullptr);
		} else {
			const dmGameObjectDDF::Disable message;
			dmMessage::PostDDF(&message, &url, &url, (uintptr_t)instance, 0, nullptr);
		}
		for (uint32_t i = 0; i < children.Size(); ++i) {
			children[i]->SetIsEnabled(_isVisible && _isEnabled);
		};
	}

	void SkObject::Animate(
		dmhash_t property_id, dmGameObject::PropertyVar to,
		float duration, float delay,
		dmGameObject::Playback playback, dmEasing::Type easing_curve,
		dmGameObject::AnimationStopped animation_stopped_callback, void *animation_stopped_userdata1, void *animation_stopped_userdata2
	) {
		dmGameObject::PropertyResult result = dmGameObject::Animate(Spiralkit::collection, instance, 0, property_id, playback, to, dmEasing::Curve(easing_curve), duration, delay, animation_stopped_callback, animation_stopped_userdata1, animation_stopped_userdata2);
		_CheckPropertyResult(result);
	}

	void SkObject::AnimateComponent(
		dmhash_t component_id, dmhash_t property_id, dmGameObject::PropertyVar to,
		float duration, float delay,
		dmGameObject::Playback playback, dmEasing::Type easing_curve,
		dmGameObject::AnimationStopped animation_stopped_callback, void *animation_stopped_userdata1, void *animation_stopped_userdata2
	) {
		dmGameObject::PropertyResult result = dmGameObject::Animate(Spiralkit::collection, instance, component_id, property_id, playback, to, dmEasing::Curve(easing_curve), duration, delay, animation_stopped_callback, animation_stopped_userdata1, animation_stopped_userdata2);
		_CheckPropertyResult(result);
	}

	void SkObject::AnimatePosition(
		Vector2 position,
		float duration, float delay,
		dmGameObject::Playback playback, dmEasing::Type easing_curve,
		dmGameObject::AnimationStopped animation_stopped_callback, void *animation_stopped_userdata1, void *animation_stopped_userdata2
	) {
		dmVMath::Point3 position3D = GetPosition3D();
		Animate(hashes::position, dmGameObject::PropertyVar(dmVMath::Vector3(position.x, position.y, position3D.getZ())), duration, delay, playback, easing_curve, animation_stopped_callback, animation_stopped_userdata1, animation_stopped_userdata2);
	}

	void SkObject::AnimateScale(
		Vector2 scale,
		float duration, float delay,
		dmGameObject::Playback playback, dmEasing::Type easing_curve,
		dmGameObject::AnimationStopped animation_stopped_callback, void *animation_stopped_userdata1, void *animation_stopped_userdata2
	) {
		dmVMath::Vector3 scale3D = GetScale3D();
		Animate(hashes::scale, dmGameObject::PropertyVar(dmVMath::Vector3(scale.x, scale.y, scale3D.getZ())), duration, delay, playback, easing_curve, animation_stopped_callback, animation_stopped_userdata1, animation_stopped_userdata2);
	}

	dmGameObject::PropertyResult SkObject::GetProperty(dmhash_t property_id, dmGameObject::PropertyDesc &out_value, dmhash_t component_id) {
		dmGameObject::PropertyOptions property_options;
		property_options.m_Index = 0;
		property_options.m_HasKey = 0;
		dmGameObject::PropertyResult result = dmGameObject::GetProperty(instance, component_id, property_id, property_options, out_value);
		_CheckPropertyResult(result);
		return result;
	}

	dmGameObject::PropertyResult SkObject::SetProperty(dmhash_t property_id, const dmGameObject::PropertyVar& property_var, dmhash_t component_id) {
		dmGameObject::PropertyOptions property_options;
		property_options.m_Index = 0;
		property_options.m_HasKey = 0;
		dmGameObject::PropertyResult result = dmGameObject::SetProperty(instance, component_id, property_id, property_options, property_var);
		_CheckPropertyResult(result);
		return result;
	}

	dmGameObject::PropertyResult SkObject::CancelAnimations(dmhash_t property_id, dmhash_t component_id) {
		dmGameObject::PropertyResult result = dmGameObject::CancelAnimations(Spiralkit::collection, instance, component_id, property_id);
		_CheckPropertyResult(result);
		return result;
	}

	void SkObject::Delete() {
		if (parent != nullptr) {
			parent->Remove(this);
		}
		for (int32_t i = children.Size() - 1; i > 0; --i) {
			children[i]->Delete();
		}
		if (instance != nullptr) {
			dmGameObject::Delete(Spiralkit::collection, instance, false);
		}
		instance = nullptr;
	}
}
