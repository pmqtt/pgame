#include "pwindow.h"
#include "pgame.h"
#include <SDL2/SDL_ttf.h>

PWindow::PWindow(const std::string& title, int x, int y, int w, int h, unsigned int flags) {
	SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
	_window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(title.c_str(), x, y, w, h, flags), SDLWindowDeleter());
	if( !(flags & SDL_WINDOW_VULKAN)) {
		_renderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED),
												  SDLRendererDeleter());
		_type = WINDOW_TYPE::SDL;
	}else{
		std::cout<<"Vulkan is not supported yet"<<std::endl;
		// USE VULKAN AS RENDERER
		_type = WINDOW_TYPE::VULKAN;
	}
}

auto PWindow::create_event_loop() -> PEventLoop { return {_window, _renderer}; }
