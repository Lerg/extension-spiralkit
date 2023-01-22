#ifndef APP_GAME_H
#define APP_GAME_H

namespace app {
	class MenuScene;

	static const char *applicationId = "extension_spiralkit";
	class App {
		private:
			MenuScene *_menu_scene;
		public:
			App();
			~App();
			void Update(float dt);
	};
}

#endif
