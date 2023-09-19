#include "peventloop.h"

void PEventLoop::run() {
	SDL_Event event;
	_quit = false;
	_animation_timer.start();
	while (!_quit) {
        _animation_timer.delta_ticks();
		_delta_time = _animation_timer.pick_time();
        
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					_quit = true;
					break;
				case SDL_KEYDOWN:
					for (auto listener : _key_down_listeners) {
						listener->on_event(this, event);
					}
					break;
				default:
					break;
			}
		}
        if(_stopped){
            continue;
        }
		SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
		SDL_RenderClear(_renderer.get());

        for(auto & iter : _physic_objects){
            for(auto & sIter : _physic_objects){
                if(iter.first != sIter.first){
                    if(iter.second->colide(sIter.second, _delta_time)){
                        break;
                    }
                }
            }
        }

		for (auto & physic : _physic_objects) {
			physic.second->move(_delta_time);
		}
		for (auto & moveable : _moveables) {
			moveable.second->draw(_renderer);
		}
		for (auto & animation : _animations) {
			animation.second->draw(this, _renderer);
		}
		SDL_RenderPresent(_renderer.get());

        int delay = (1000.0f / 60.0f) - (_delta_time / 1000.0f);
        if(delay > 0){
            SDL_Delay(delay);
        }
	}
}

void PEventLoop::stop(){
    _stopped = true;
}

void PEventLoop::resume(){
    _stopped = false;
}

