#include <iostream>

#include "../pdebug.h"
#include "../pgame.h"
#include "../ptextfield.h"
#include "../pbutton.h"
#include "../pstacklayout.h"

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

void t1(){
    std::cout<<"Button 1"<<"\n";
}
void t2(){
    std::cout<<"Button 2"<<"\n";
}
void t3(){
    std::cout<<"Button 3"<<"\n";
}
void t4(){
    std::cout<<"Button 4"<<"\n";
}

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


    auto stacklayout = std::make_shared<PStackLayout>(PStackLayoutDirection::VERTICAL,10,10);
    auto button1 = std::make_shared<PButton>(0,10,100,20);
    button1->listener(t1);
    auto button2 = std::make_shared<PButton>(0,0,100,20);
    button2->listener(t2);
    auto button3 = std::make_shared<PButton>(0,0,100,20);
    button3->listener(t3);
    auto button4 = std::make_shared<PButton>(0,0,100,20);
    button4->listener(t4);
    
    auto textfield = std::make_shared<PTextField>(0,0,100,20);

    stacklayout->add(button1);
    stacklayout->add(button2);
    stacklayout->add(button3);
    stacklayout->add(button4);
    stacklayout->add(textfield);
    stacklayout->spacing(10);


    loop.add_gui_element("stacklayout", stacklayout);



	loop.run();
	return 0;
}
