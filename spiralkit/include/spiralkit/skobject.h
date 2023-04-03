#ifndef SK_SKOBJECT_H
#define SK_SKOBJECT_H

#include <godot/core/math/vector2.h>
#include <godot/core/math/vector2i.h>
#include <dmsdk/gameobject/gameobject.h>
#include <gameobject/gameobject_ddf.h>
#include "dmsdk.h"
#include "array.h"
#include "defold.h"
#include "hashes.h"

namespace spiralkit {
	class SkObject {
		private:
			bool _isVisible = true;
			bool _isEnabled = true;
		public:
			dmGameObject::HInstance instance;
			dmhash_t identifier;
			dmMessage::URL url;
			SkObject *parent;
			dmArray<SkObject*> children;
			Vector2i size = {0, 0};

		private:
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

			bool _CheckPropertyResult(dmGameObject::PropertyResult result) {
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

		public:
			SkObject()
			: instance(nullptr), identifier(0), parent(nullptr) {
			}

			SkObject(SkObject *parent) {
				InstanceIdentifierPair game_object = Defold::NewGameObject();
				instance = game_object.instance;
				identifier = game_object.identifier;
				this->parent = parent;
				Init();
			}

			SkObject(dmGameObject::HInstance instance, dmhash_t identifier, SkObject *parent = nullptr)
			: instance(instance), identifier(identifier), parent(parent) {
				Init();
			}

			~SkObject() {
				Delete();
			}

			inline void Init() {
				if (parent != nullptr) {
					SetParent(parent);
				}
				dmMessage::ResetURL(&url);
				url.m_Socket = dmGameObject::GetMessageSocket(dmGameObject::GetCollection(instance));
				url.m_Path = identifier;
			}

			inline void AcquireInputFocus() {
				dmGameObject::AcquireInputFocus(dmGameObject::GetCollection(instance), instance);
			}

			inline Vector2 GetPosition() {
				dmVMath::Point3 position = GetPosition3D();
				return Vector2{position.getX(), position.getY()};
			}

			inline dmVMath::Point3 GetPosition3D() {
				return dmGameObject::GetPosition(instance);
			}

			inline void SetPosition(Vector2 position) {
				dmVMath::Point3 position3D = GetPosition3D();
				position3D.setX(position.x);
				position3D.setY(position.y);
				SetPosition3D(position3D);
			}

			inline void SetPosition3D(dmVMath::Point3 position) {
				dmGameObject::SetPosition(instance, position);
			}

			inline Vector2 GetScale() {
				const dmVMath::Vector3 scale3D = GetScale3D();
				return Vector2{scale3D.getX(), scale3D.getY()};
			}

			inline dmVMath::Vector3 GetScale3D() {
				return dmGameObject::GetScale(instance);
			}

			inline void SetScale(Vector2 scale) {
				dmVMath::Vector3 scale3D = GetScale3D();
				scale3D.setX(scale.x);
				scale3D.setY(scale.y);
				SetScale3D(scale3D);
			}

			inline void SetScale3D(dmVMath::Vector3 scale) {
				dmGameObject::SetScale(instance, scale);
			}

			inline void SetScale(float scale) {
				dmVMath::Vector3 s = dmGameObject::GetScale(instance);
				s.setX(scale);
				s.setY(scale);
				dmGameObject::SetScale(instance, s);
			}

			inline float GetUniformScale() {
				return dmGameObject::GetUniformScale(instance);
			}

			inline void SetUniformScale(float scale) {
				dmGameObject::SetScale(instance, {scale, scale, scale});
			}

			inline dmVMath::Quat GetRotation() {
				return dmGameObject::GetRotation(instance);
			}

			inline void SetRotation(dmVMath::Quat rotation) {
				dmGameObject::SetRotation(instance, rotation);
			}

			inline Vector2 GetWorldPosition() {
				const dmVMath::Point3 position3D = GetWorldPosition3D();
				return Vector2{position3D.getX(), position3D.getY()};
			}

			inline dmVMath::Point3 GetWorldPosition3D() {
				return dmGameObject::GetWorldPosition(instance);
			}

			inline Vector2 GetWorldScale() {
				const dmVMath::Vector3 scale = GetWorldScale3D();
				return Vector2(scale.getX(), scale.getY());
			}

			inline dmVMath::Vector3 GetWorldScale3D() {
				return dmGameObject::GetWorldScale(instance);
			}

			inline float GetWorldUniformScale() {
				return dmGameObject::GetWorldUniformScale(instance);
			}

			inline dmVMath::Quat GetWorldRotation() {
				return dmGameObject::GetWorldRotation(instance);
			}

			inline Vector2 GetScaledSize() {
				Vector2 v = GetScale();
				v.x *= size.width;
				v.y *= size.height;
				return v;
			}

			inline void SetParent(SkObject *parent) {
				assert(parent != nullptr && parent->instance != nullptr);
				parent->Insert(this);
			}

			void Insert(SkObject *child, int32_t index = -1) {
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

			void Remove(SkObject *child) {
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

			inline void ToBack() {
				assert(parent != nullptr);
				SkObject *saved_parent = parent;
				saved_parent->Remove(this);
				saved_parent->Insert(this, 0);
			}

			inline void ToBack(SkObject *child) {
				uint32_t index = dmArrayUtil::GetIndexOf(&children, child);
				if (index == 0) {
					return;
				}
				dmArrayUtil::Move(&children, index, 0);
				for (uint32_t i = 0; i <= index; ++i) {
					SetPositionZ(children[i], i);
				}
			}

			inline void ToFront() {
				assert(parent != nullptr);
				parent->ToFront(this);
			}

			inline void ToFront(SkObject *child) {
				uint32_t index = dmArrayUtil::GetIndexOf(&children, child);
				dmArrayUtil::Move(&children, index, children.Size() - 1);
				for (uint32_t i = index; i < children.Size(); ++i) {
					SetPositionZ(children[i], i);
				}
			}

			inline void Delete() {
				if (parent != nullptr) {
					parent->Remove(this);
				}
				for (int32_t i = children.Size() - 1; i > 0; --i) {
					children[i]->Delete();
				}
				if (instance != nullptr) {
					dmGameObject::Delete(dmGameObject::GetCollection(instance), instance, false);
				}
				instance = nullptr;
			}

			inline bool IsVisible() {
				return _isVisible;
			}

			inline bool IsEnabled() {
				return _isEnabled;
			}

			inline void SetIsVisible(bool is_visible) {
				if (_isVisible == is_visible) {
					return;
				}
				_isVisible = is_visible;
				SetIsEnabled(_isVisible);
			}

			inline void SetIsEnabled(bool is_enabled) {
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

			//void AnimationStopped(dmGameObject::HInstance instance, dmhash_t component_id, dmhash_t property_id, bool finished, void* userdata1, void* userdata2)
			inline void Animate(
				dmhash_t property_id, dmGameObject::PropertyVar to,
				float duration = 1.0, float delay = 0.0,
				dmGameObject::Playback playback = dmGameObject::PLAYBACK_ONCE_FORWARD, dmEasing::Type easing_curve = dmEasing::TYPE_LINEAR,
				dmGameObject::AnimationStopped animation_stopped_callback = nullptr, void *animation_stopped_userdata1 = nullptr, void *animation_stopped_userdata2 = nullptr
			) {
				dmGameObject::PropertyResult result = dmGameObject::Animate(dmGameObject::GetCollection(instance), instance, 0, property_id, playback, to, dmEasing::Curve(easing_curve), duration, delay, animation_stopped_callback, animation_stopped_userdata1, animation_stopped_userdata2);
				_CheckPropertyResult(result);
			}

			inline void AnimateComponent(
				dmhash_t component_id, dmhash_t property_id, dmGameObject::PropertyVar to,
				float duration = 1.0, float delay = 0.0,
				dmGameObject::Playback playback = dmGameObject::PLAYBACK_ONCE_FORWARD, dmEasing::Type easing_curve = dmEasing::TYPE_LINEAR,
				dmGameObject::AnimationStopped animation_stopped_callback = nullptr, void *animation_stopped_userdata1 = nullptr, void *animation_stopped_userdata2 = nullptr
			) {
				dmGameObject::PropertyResult result = dmGameObject::Animate(dmGameObject::GetCollection(instance), instance, component_id, property_id, playback, to, dmEasing::Curve(easing_curve), duration, delay, animation_stopped_callback, animation_stopped_userdata1, animation_stopped_userdata2);
				_CheckPropertyResult(result);
			}

			inline void AnimatePosition(
				Vector2 position,
				float duration = 1.0, float delay = 0.0,
				dmGameObject::Playback playback = dmGameObject::PLAYBACK_ONCE_FORWARD, dmEasing::Type easing_curve = dmEasing::TYPE_LINEAR,
				dmGameObject::AnimationStopped animation_stopped_callback = nullptr, void *animation_stopped_userdata1 = nullptr, void *animation_stopped_userdata2 = nullptr
			) {
				dmVMath::Point3 position3D = GetPosition3D();
				Animate(hashes::position, dmGameObject::PropertyVar(dmVMath::Vector3(position.x, position.y, position3D.getZ())), duration, delay, playback, easing_curve, animation_stopped_callback, animation_stopped_userdata1, animation_stopped_userdata2);
			}

			inline void AnimateScale(
				Vector2 scale,
				float duration = 1.0, float delay = 0.0,
				dmGameObject::Playback playback = dmGameObject::PLAYBACK_ONCE_FORWARD, dmEasing::Type easing_curve = dmEasing::TYPE_LINEAR,
				dmGameObject::AnimationStopped animation_stopped_callback = nullptr, void *animation_stopped_userdata1 = nullptr, void *animation_stopped_userdata2 = nullptr
			) {
				dmVMath::Vector3 scale3D = GetScale3D();
				Animate(hashes::scale, dmGameObject::PropertyVar(dmVMath::Vector3(scale.x, scale.y, scale3D.getZ())), duration, delay, playback, easing_curve, animation_stopped_callback, animation_stopped_userdata1, animation_stopped_userdata2);
			}

			inline dmGameObject::PropertyResult GetProperty(dmhash_t property_id, dmGameObject::PropertyDesc &out_value, dmhash_t component_id = 0) {
				dmGameObject::PropertyOptions property_options;
				property_options.m_Index = 0;
				property_options.m_HasKey = 0;
				dmGameObject::PropertyResult result = dmGameObject::GetProperty(instance, component_id, property_id, property_options, out_value);
				_CheckPropertyResult(result);
				return result;
			}

			inline dmGameObject::PropertyResult SetProperty(dmhash_t property_id, const dmGameObject::PropertyVar& property_var, dmhash_t component_id = 0) {
				dmGameObject::PropertyOptions property_options;
				property_options.m_Index = 0;
				property_options.m_HasKey = 0;
				dmGameObject::PropertyResult result = dmGameObject::SetProperty(instance, component_id, property_id, property_options, property_var);
				_CheckPropertyResult(result);
				return result;
			}

			inline dmGameObject::PropertyResult CancelAnimations(dmhash_t property_id, dmhash_t component_id = 0) {
				dmGameObject::PropertyResult result = dmGameObject::CancelAnimations(dmGameObject::GetCollection(instance), instance, component_id, property_id);
				_CheckPropertyResult(result);
				return result;
			}
	};
}

#endif
