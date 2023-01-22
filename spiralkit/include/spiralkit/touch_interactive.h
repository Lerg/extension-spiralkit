#ifndef SK_TOUCH_INTERACTIVE_H
#define SK_TOUCH_INTERACTIVE_H

#include "input_event.h"

namespace spiralkit {
	class TouchInteractive; // Forward reference.
	class SkObject;
	typedef bool (*TouchInteractiveOnTouchCallback)(TouchInteractive &touch_interactive, const TouchEvent &touch_event);
	typedef void (*TouchInteractiveCallback)(TouchInteractive &touch_interactive, const TouchEvent &touch_event);

	class TouchInteractive {
		private:
			static bool OnTouchDefault(TouchInteractive &touch_interactive, const TouchEvent &touch_event);

		public:
			SkObject *object = nullptr;
			TouchInteractiveOnTouchCallback onTouch = nullptr;
			TouchInteractiveCallback onPress = nullptr;
			TouchInteractiveCallback onMove = nullptr;
			TouchInteractiveCallback onRelease = nullptr;
			bool isFocused = false;

			TouchInteractive(SkObject *object);
			~TouchInteractive();

			bool IsInside(Vector2i position);
	};
}

#endif
