#ifndef SK_UI_UI_H
#define SK_UI_UI_H

#include <godot/core/math/vector2i.h>

namespace spiralkit {
	class SkObject;
}

namespace spiralkit::ui {
	class UI {
		public:
			static void ZoomFit(SkObject *object, Vector2i size);
			static void LetterboxFit(SkObject *object, Vector2i size);
			static void ZoomFit(SkObject *object, SkObject *area_object);
			static void LetterboxFit(SkObject *object, SkObject *area_object);
			static Vector2i ClampAspect(Vector2i size, float aspect_min, float aspect_max);
	};
}

#endif
