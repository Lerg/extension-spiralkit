#ifndef SK_MOUSE_INTERACTIVE_H
#define SK_MOUSE_INTERACTIVE_H

#include "input_event.h"

namespace spiralkit {
	class MouseInteractive; // Forward reference.
	class SkObject;
	typedef bool (*MouseInteractiveOnMouseCallback)(MouseInteractive &mouse_interactive, const MouseEvent &mouse_event);
	typedef void (*MouseInteractiveCallback)(MouseInteractive &mouse_interactive, const MouseEvent &mouse_event);

	class MouseInteractive {
		private:
			static bool OnMouseDefault(MouseInteractive &mouse_interactive, const MouseEvent &mouse_event);
			bool _isWorkAroundExecuted = false;

		public:
			SkObject *object = nullptr;
			MouseInteractiveOnMouseCallback onMouse = nullptr;
			MouseInteractiveCallback onPress = nullptr;
			MouseInteractiveCallback onMove = nullptr;
			MouseInteractiveCallback onRelease = nullptr;
			bool isOver = false;

			MouseInteractive(SkObject *object);
			~MouseInteractive();

			bool IsInside(Vector2i position);
	};
}

#endif
