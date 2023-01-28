#ifndef APP_GAME_H
#define APP_GAME_H

namespace app {
	class MenuScene;
	class AboutScene;
	class PlayScene;

	static const char *applicationId = "extension_spiralkit";
	class App {
		private:
			MenuScene *_menuScene;
			AboutScene *_aboutScene;
			PlayScene *_playScene;
		public:
			App();
			~App();
			void Update(float dt);
	};
}

#endif
