#include "peventloop.h"
#include "../guielements/pbutton.h"

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
				case SDL_KEYDOWN: {
					bool is_clicked = false;
					if (event.key.keysym.sym == SDLK_BACKSPACE) {
						for (auto& gui_element : _gui_elements) {
							int x, y;
							SDL_GetMouseState(&x, &y);
							if (gui_element.second->is_clicked(x, y)) {
								PStyle style = gui_element.second->element()->style();
								if (!gui_element.second->element()->fixed_content()) {
									if (!style.text->empty()) {
										style.text->pop_back();
										gui_element.second->element()->style(style);
									}
								}
								is_clicked = true;
							}
						}
					}
					if(!is_clicked) {
						for (const auto& listener : _key_down_listeners) {
							listener->on_event(this, event);
						}
					}
				}
				break;
                case SDL_MOUSEBUTTONDOWN: {
					bool is_clicked = false;
					for (auto& gui_element : _gui_elements) {
						int x, y;
						SDL_GetMouseState(&x, &y);
						if (gui_element.second->is_clicked(x, y)) {
							gui_element.second->on_click();
							is_clicked = true;
						}
					}
					if (!is_clicked) {
						for (const auto& listener : _mouse_down_listeners) {
							listener->on_event(this, event);
						}
					}
				}
				break;
				case SDL_MOUSEBUTTONUP: {
					for(const auto & listener : _mouse_up_listeners){
						listener->on_event(this, event);
					}
				}
				break;
                case SDL_MOUSEMOTION: {
					bool is_clicked = false;
					for (auto& gui_element : _gui_elements) {
						int x, y;
						SDL_GetMouseState(&x, &y);
						if (gui_element.second->is_clicked(x, y)) {
							gui_element.second->hover(true);
							is_clicked = true;
						} else {
							gui_element.second->hover(false);
						}
					}
					if(!is_clicked) {
						for (auto& listener : _mouse_move_listeners) {
							listener->on_event(this, event);
						}
					}
				}
				break;
                case SDL_TEXTINPUT:{
                    std::string s = event.text.text;
                    for( auto & gui_element : _gui_elements){
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        if(gui_element.second->is_clicked(x,y)){
                            PStyle style = gui_element.second->element()->style();
                            if(!gui_element.second->element()->fixed_content()){   
                                *style.text += s;
                                gui_element.second->element()->style(style);
                            }
                        }
                    }
                }
                    break;
				default:
					break;
			}

		}
		if (_stopped) {
			continue;
		}
        for(auto & collision: _engine.collisions()){
           if(_collision_listeners.find(collision) != _collision_listeners.end()){
                _collision_listeners[collision]->on_event(this, 
                                                      collision, 
                                                      _engine.all_objects()[collision.name1], 
                                                      _engine.all_objects()[collision.name2]);
           }
        }

		SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
		SDL_RenderClear(_renderer.get());
        
        _engine.simulate_step(_delta_time);
        
		for (auto& moveable : _moveables) {
			moveable.second->draw(_renderer);
		}
		for (auto& animation : _animations) {
			animation.second->draw(this, _renderer);
		}
        for (auto& gui_element : _gui_elements) {
            gui_element.second->draw(_renderer);
        }


		SDL_RenderPresent(_renderer.get());

		int delay = (1000.0f / 60.0f) - (_delta_time * 1000.0f);
		if (delay > 0) {
			SDL_Delay(delay);
		}
        //stop();
	}
    std::exit(0);
}

void PEventLoop::stop() { _stopped = true; }

void PEventLoop::resume() { _stopped = false; }
