#ifndef SK_SOUND_MANAGER_H
#define SK_SOUND_MANAGER_H

#include <spiralkit/asset.h>

namespace spiralkit {
	class SoundManager {
		private:
			static dmhash_t _assetPath;
			SoundManager();
		public:
			static bool isMusicOn;
			static bool isSoundOn;
			static void Init() {
				_assetPath = dmHashString64("/assets/sounds");
			}

			static void Play(asset::Sound sound);
			static void Stop(asset::Sound sound);
	};
}

#endif
