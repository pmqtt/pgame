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

	auto drawable = std::make_shared<PRect>(700, 228, 10, 10);
	auto object = std::make_shared<PPhysicObject>(drawable, std::make_shared<PSatColider>());
	object->gravity(0);
	object->velocity(-30, 0.000);
	object->restitution(0.8);
	event_loop.add_physics_object("obj", object);

	auto static_draw2 = std::make_shared<PRect>(400, 230, 140, 100, false);

	event_loop.add_physics_object("static_draw2", std::make_shared<PPhysicObject>(static_draw2));
	event_loop.run();
	return 0;
}
