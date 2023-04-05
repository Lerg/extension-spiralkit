#include <spiralkit/skobject.h>

#include <spiralkit/ui/ui.h>


namespace spiralkit::ui {
	void UI::ZoomFit(SkObject *object, Vector2i size) {
		float scale_h = (float)size.width / object->size.width;
		float scale_v = (float)size.height / object->size.height;
		object->SetScale(fmax(scale_h, scale_v));
	}

	void UI::LetterboxFit(SkObject *object, Vector2i size) {
		float scale_h = (float)size.width / object->size.width;
		float scale_v = (float)size.height / object->size.height;
		object->SetScale(fmin(scale_h, scale_v));
	}

	void UI::ZoomFit(SkObject *object, SkObject *area_object) {
		ZoomFit(object, area_object->size);
		object->SetPosition(area_object->GetPosition());
	}

	void UI::LetterboxFit(SkObject *object, SkObject *area_object) {
		LetterboxFit(object, area_object->size);
		object->SetPosition(area_object->GetPosition());
	}

	Vector2i UI::ClampAspect(Vector2i size, float aspect_min, float aspect_max) {
		float aspect = (float)size.width / size.height;
		if (aspect < aspect_min) {
			size.height = (float)size.width / aspect_min;
		} else if (aspect > aspect_max) {
			size.width = (float)size.height * aspect_max;
		}
		return size;
	}
}
