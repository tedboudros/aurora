#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Pages/MainMenu.hpp"

MainMenuPage::MainMenuPage(RenderWindow* p_window) :window(p_window)  {
	SDL_Texture* wallpaper = window->loadTexture("res/images/bg.png");
	wallpaperEntity = Entity(MultiSize(Size(0, SIZE_WIDTH),Size(0, SIZE_HEIGHT),Size(100, SIZE_WIDTH),Size(100, SIZE_HEIGHT)), wallpaper);
};

void MainMenuPage::render(double deltaTime) {
	window->render(wallpaperEntity);
};