#ifndef APP_ASSETS_H
#define APP_ASSETS_H

#include <spiralkit/asset.h>

using namespace spiralkit;

namespace app::assets {
	namespace sprites {
		static const asset::Sprite popup_shade("/assets/sprites#atlas0", "popup_shade", 1, 1);
		static const asset::Sprite background("/assets/sprites#atlas0", "background", 1920, 1080);
		static const asset::Sprite placeholder("/assets/sprites#atlas0", "placeholder", 128, 128);
		static const asset::Sprite piece("/assets/sprites#atlas0", "piece", 128, 128);
		static const asset::Sprite piece_pressed("/assets/sprites#atlas0", "piece_pressed", 128, 128);
		static const asset::Sprite buttons$button_pressed("/assets/sprites#atlas0", "buttons/button_pressed", 256, 96);
		static const asset::Sprite buttons$button("/assets/sprites#atlas0", "buttons/button", 256, 96);
	}
	namespace labels {
		static const asset::Label main("/assets/labels#main", 48);
	}
	namespace particlefx {
		static const asset::ParticleFX blob("/assets/particlefx#blob");
	}
}

#endif
