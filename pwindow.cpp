#include "pwindow.h"
#include "pgame.h"



PWindow::PWindow(const std::string & title,
                int x , 
                int y , 
                int w , int h , 
                unsigned int flags ){
            SDL_Init(SDL_INIT_EVERYTHING);
            _window = std::shared_ptr<SDL_Window>(
                                                SDL_CreateWindow(title.c_str(), x, y, w, h, flags),
                                                SDLWindowDeleter());
            _renderer = std::shared_ptr<SDL_Renderer>(
                                                SDL_CreateRenderer(_window.get(), -1, 
                                                                    SDL_RENDERER_ACCELERATED),
                                                SDLRendererDeleter());
}

PEventLoop PWindow::create_event_loop(){
    return PEventLoop(_window, _renderer);
}
