#ifndef SK_SKOBJECT_H
#define SK_SKOBJECT_H

#include <godot/core/math/vector2.h>
#include <godot/core/math/vector2i.h>
#include <dmsdk/gameobject/gameobject.h>
#include "dmsdk.h"

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

			SkObject();
			SkObject(SkObject *parent);
			SkObject(dmGameObject::HInstance instance, dmhash_t identifier, SkObject *parent = nullptr);
			~SkObject();
			void Init();
			void AcquireInputFocus();

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

			void Insert(SkObject *child, int32_t index = -1) ;
			void Remove(SkObject *child) ;

			inline void ToBack() {
				assert(parent != nullptr);
				SkObject *saved_parent = parent;
				saved_parent->Remove(this);
				saved_parent->Insert(this, 0);
			}

			void ToBack(SkObject *child);

			inline void ToFront() {
				assert(parent != nullptr);
				parent->ToFront(this);
			}

			void ToFront(SkObject *child);
			void Delete();

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

			void SetIsEnabled(bool is_enabled);

			void Animate(
				dmhash_t property_id, dmGameObject::PropertyVar to,
				float duration = 1.0, float delay = 0.0,
				dmGameObject::Playback playback = dmGameObject::PLAYBACK_ONCE_FORWARD, dmEasing::Type easing_curve = dmEasing::TYPE_LINEAR,
				dmGameObject::AnimationStopped animation_stopped_callback = nullptr, void *animation_stopped_userdata1 = nullptr, void *animation_stopped_userdata2 = nullptr
			);

			void AnimateComponent(
				dmhash_t component_id, dmhash_t property_id, dmGameObject::PropertyVar to,
				float duration = 1.0, float delay = 0.0,
				dmGameObject::Playback playback = dmGameObject::PLAYBACK_ONCE_FORWARD, dmEasing::Type easing_curve = dmEasing::TYPE_LINEAR,
				dmGameObject::AnimationStopped animation_stopped_callback = nullptr, void *animation_stopped_userdata1 = nullptr, void *animation_stopped_userdata2 = nullptr
			);

			void AnimatePosition(
				Vector2 position,
				float duration = 1.0, float delay = 0.0,
				dmGameObject::Playback playback = dmGameObject::PLAYBACK_ONCE_FORWARD, dmEasing::Type easing_curve = dmEasing::TYPE_LINEAR,
				dmGameObject::AnimationStopped animation_stopped_callback = nullptr, void *animation_stopped_userdata1 = nullptr, void *animation_stopped_userdata2 = nullptr
			);

			void AnimateScale(
				Vector2 scale,
				float duration = 1.0, float delay = 0.0,
				dmGameObject::Playback playback = dmGameObject::PLAYBACK_ONCE_FORWARD, dmEasing::Type easing_curve = dmEasing::TYPE_LINEAR,
				dmGameObject::AnimationStopped animation_stopped_callback = nullptr, void *animation_stopped_userdata1 = nullptr, void *animation_stopped_userdata2 = nullptr
			);

			dmGameObject::PropertyResult GetProperty(dmhash_t property_id, dmGameObject::PropertyDesc &out_value, dmhash_t component_id = 0);
			dmGameObject::PropertyResult SetProperty(dmhash_t property_id, const dmGameObject::PropertyVar& property_var, dmhash_t component_id = 0);
			dmGameObject::PropertyResult CancelAnimations(dmhash_t property_id, dmhash_t component_id = 0);
	};
}

#endif
