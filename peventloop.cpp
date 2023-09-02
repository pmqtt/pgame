#include "peventloop.h"

void PEventLoop::run(){
    SDL_Event event;
    _quit = false;
    _animation_timer.start();
    while(!_quit){
        _delta_time = _animation_timer.delta_ticks();
       // if(_delta_time < (1000.0/60.0)){
          //  SDL_Delay((1000.0/60.0) - _delta_time);
       // }
        while(SDL_PollEvent(&event)){
                switch(event.type){
                case SDL_QUIT:
                    _quit = true;
                    break;
                case SDL_KEYDOWN:
                    for(auto listener : _key_down_listeners){
                        listener->on_event(this, event);
                    }
                    break;
                default:
                    break;
                }
        }
        SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
        SDL_RenderClear(_renderer.get());
        for(auto moveable : _moveables){
            moveable.second->draw(_renderer);
        }
        for(auto animation : _animations){
            animation.second->draw(this, _renderer);
        }
        SDL_RenderPresent(_renderer.get());
                
    }
}
