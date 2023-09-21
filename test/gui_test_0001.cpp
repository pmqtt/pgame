#include <iostream>

#include "../pdebug.h"
#include "../pgame.h"
#include "../pbutton.h"

struct PressEscape : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			loop->quit();
		}
	}
};

struct PressRight : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		if (event.key.keysym.sym == SDLK_RIGHT) {
			loop->moveables()["player1"]->add(std::array<float, 2>{1, 0});
		}
	}
};

struct PressLeft : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		if (event.key.keysym.sym == SDLK_LEFT) {
			loop->moveables()["player1"]->add(std::array<float, 2>{-1, 0});
		}
	}
};

struct PressUp : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		if (event.key.keysym.sym == SDLK_UP) {
			loop->moveables()["player1"]->add(std::array<float, 2>{0, -1});
		}
	}
};

struct PressDown : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		if (event.key.keysym.sym == SDLK_DOWN) {
			loop->moveables()["player1"]->add(std::array<float, 2>{0, 1});
		}
	}
};

auto main(int argc, char** argv) -> int {
	P_UNUSED(argc);
	P_UNUSED(argv);

	PWindow window("PGame");
	PEventLoop loop = window.create_event_loop();
	loop.add_key_down_listener(std::make_shared<PressEscape>());
	loop.add_key_down_listener(std::make_shared<PressRight>());
	loop.add_key_down_listener(std::make_shared<PressLeft>());
	loop.add_key_down_listener(std::make_shared<PressUp>());
	loop.add_key_down_listener(std::make_shared<PressDown>());

    loop.add_button("hello_button", std::make_shared<PButton>(50,50,100,20));

	loop.run();
	return 0;
}
