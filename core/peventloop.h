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
#include "../guielements/pguielement.h"
#include "../core/pmoveable.h"

class PButton;
class PEventLoop;

struct PEventListener {
	virtual void on_event(PEventLoop* loop, SDL_Event event) = 0;
};

struct PKeyDownListener {
	virtual void on_event(PEventLoop* loop, SDL_Event event) = 0;
};

struct PCollisionListener{
    virtual void on_event(PEventLoop* loop,const PCollisionItem & item, std::shared_ptr<PPhysicObject> object1, std::shared_ptr<PPhysicObject> object2) = 0;
};

class PEventLoop {
   public:
	PEventLoop() = default;
	PEventLoop(std::shared_ptr<SDL_Window> _window, std::shared_ptr<SDL_Renderer> _renderer)
		: _window(_window), _renderer(_renderer) {}

	void push(SDL_Event event) { _events.push_back(event); }

	void add_key_down_listener(std::shared_ptr<PKeyDownListener> listener) { _key_down_listeners.push_back(listener); }

    void add_collision_listener(const PCollisionItem & item,std::shared_ptr<PCollisionListener> listener) { _collision_listeners[item] = listener; }

	void quit() { _quit = true; }

	auto moveables() const -> std::map<std::string, std::shared_ptr<PDrawable>> { return _moveables; }

	auto physics_objects() -> std::map<std::string, std::shared_ptr<PPhysicObject>> { return _engine.all_objects(); }

	void add_physics_object(const std::string& name, std::shared_ptr<PPhysicObject> physic) {
		_engine.add_physic_object(name, physic);
		_moveables["physic_" + name] = physic->drawable();
	}

	void add_moveable(const std::string& name, std::shared_ptr<PDrawable> moveable) { _moveables[name] = moveable; }

	void add_collidable_moveable(const std::string & name, std::shared_ptr<PDrawable> moveable){
		_moveables[name] = moveable;
		_engine.add_moveable(name, std::make_shared<PMoveable>(moveable, std::make_shared<PSatCollider>()));
	}

	void add_animation(const std::string& name, std::shared_ptr<PAnimation> animation) {
		_animations[name] = animation;
	}

    void add_gui_element(const std::string& name, std::shared_ptr<PGuiElement> button) {
        _gui_elements[name] = button;
    }

	auto animation_timer() -> PTimer& { return _animation_timer; }

	auto delta_time() const -> double { return _delta_time; }

	void run();

	void stop();

	void resume();

	void enable_physic_simulation() { should_use_physic_engine = true; }

   private:
    bool should_use_physic_engine = false;
	bool _quit;
	std::list<SDL_Event> _events;
	std::list<std::shared_ptr<PKeyDownListener>> _key_down_listeners;
	std::shared_ptr<SDL_Window> _window;
	std::shared_ptr<SDL_Renderer> _renderer;
	std::map<std::string, std::shared_ptr<PDrawable>> _moveables;
	std::map<std::string, std::shared_ptr<PAnimation>> _animations;
    std::map<std::string, std::shared_ptr<PGuiElement>> _gui_elements;
    std::unordered_map<PCollisionItem, std::shared_ptr<PCollisionListener> > _collision_listeners;
	PEngine _engine;
	PTimer _animation_timer;
	double _delta_time;
	bool _stopped = false;
};

#endif
