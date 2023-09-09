#include "../pcolider.h"
#include "../pgame.h"

struct PressEscape : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			loop->stop();
		}
	}
};

struct PressRight : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) {
		P_UNUSED(loop);
		if (event.key.keysym.sym == SDLK_RIGHT) {
		}
	}
};

auto main(int argc, char** argv) -> int {
	P_UNUSED(argc);
	P_UNUSED(argv);
	PWindow window("PGame", 0, 0, 1600, 600);
	auto event_loop = window.create_event_loop();
	event_loop.add_key_down_listener(std::make_shared<PressEscape>());
	event_loop.add_key_down_listener(std::make_shared<PressRight>());

	auto drawable = std::make_shared<PRect>(500, 0, 10, 10);
	auto object = std::make_shared<PPhysicObject>(drawable, std::make_shared<PSatColider>());
	object->gravity(98.1);
	object->velocity(-30, 0.001);
	object->restitution(0.8);
	event_loop.add_physics_object("obj", object);

	auto static_draw1 = std::make_shared<PRect>(150, 300, 300, 50, false);
	auto static_draw2 = std::make_shared<PRect>(400, 170, 140, 50, false);

	static_draw1->rotate(-22.5);
	event_loop.add_physics_object("static_draw1", std::make_shared<PPhysicObject>(static_draw1));
	event_loop.add_physics_object("static_draw2", std::make_shared<PPhysicObject>(static_draw2));
	event_loop.run();
	return 0;
}
