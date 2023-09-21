#ifndef PEVENTLOOP_H
#define PEVENTLOOP_H
#include <SDL2/SDL.h>

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "panimation.h"
#include "pengine.h"
#include "pprimitive.h"
#include "ptimer.h"

class PButton;
class PEventLoop;

struct PEventListener {
	virtual void on_event(PEventLoop* loop, SDL_Event event) = 0;
};

struct PKeyDownListener {
	virtual void on_event(PEventLoop* loop, SDL_Event event) = 0;
};

class PEventLoop {
   public:
	PEventLoop() = default;
	PEventLoop(std::shared_ptr<SDL_Window> _window, std::shared_ptr<SDL_Renderer> _renderer)
		: _window(_window), _renderer(_renderer) {}

	void push(SDL_Event event) { _events.push_back(event); }

	void add_key_down_listener(std::shared_ptr<PKeyDownListener> listener) { _key_down_listeners.push_back(listener); }

	void quit() { _quit = true; }

	auto moveables() const -> std::map<std::string, std::shared_ptr<PDrawable>> { return _moveables; }

	auto physics_objects() -> std::map<std::string, std::shared_ptr<PPhysicObject>> { return _engine.objects(); }

	void add_physics_object(const std::string& name, std::shared_ptr<PPhysicObject> physic) {
		_engine.add_physic_object(name, physic);
		_moveables["physic_" + name] = physic->drawable();
	}

	void add_moveable(const std::string& name, std::shared_ptr<PDrawable> moveable) { _moveables[name] = moveable; }

	void add_animation(const std::string& name, std::shared_ptr<PAnimation> animation) {
		_animations[name] = animation;
	}

    void add_button(const std::string& name, std::shared_ptr<PButton> button) {
        _gui_elements[name] = button;
    }

	auto animation_timer() -> PTimer& { return _animation_timer; }

	auto delta_time() const -> double { return _delta_time; }

	void run();

	void stop();

	void resume();

   private:
	bool _quit;
	std::list<SDL_Event> _events;
	std::list<std::shared_ptr<PKeyDownListener>> _key_down_listeners;
	std::shared_ptr<SDL_Window> _window;
	std::shared_ptr<SDL_Renderer> _renderer;
	std::map<std::string, std::shared_ptr<PDrawable>> _moveables;
	std::map<std::string, std::shared_ptr<PAnimation>> _animations;
    std::map<std::string, std::shared_ptr<PButton>> _gui_elements;
	PEngine _engine;
	PTimer _animation_timer;
	double _delta_time;
	bool _stopped = false;
};

#endif
