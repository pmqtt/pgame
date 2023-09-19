#include "../pcolider.h"
#include "../pgame.h"

struct PressEscape : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			loop->quit();
		}
	}
};

struct PressRight : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) {
		if (event.key.keysym.sym == SDLK_RIGHT) {
			loop->physics_objects()["obj"]->acceleration(-4.0f);
		}
	}
};

auto create_physic_object(int x, int y, float vx,float vy) -> std::shared_ptr<PPhysicObject> {
    auto drawable = std::make_shared<PRect>(x, y, 10, 10);
    auto object = std::make_shared<PPhysicObject>(drawable, std::make_shared<PSatColider>());
    object->gravity(98.1);
    object->velocity(vx, vy);
    object->restitution(0.6);
    return object;
}

auto main(int argc, char** argv) -> int {
	P_UNUSED(argc);
	P_UNUSED(argv);
	PWindow window("PGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 600);
	auto event_loop = window.create_event_loop();
	event_loop.add_key_down_listener(std::make_shared<PressEscape>());
	event_loop.add_key_down_listener(std::make_shared<PressRight>());
    for(int i = 0; i < 10; ++i) {
	    event_loop.add_physics_object("obj_a" + std::to_string(i), create_physic_object(i*20+11,i*20+11,30,0.001));
    }
	auto object2 = std::make_shared<PPhysicObject>(std::make_shared<PRect>(0, 510, 800, 100, false));
	event_loop.add_physics_object("obj2", object2);
	event_loop.run();
	return 0;
}
