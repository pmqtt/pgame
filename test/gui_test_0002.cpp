#include <iostream>

#include "../core/pdebug.h"
#include "../core/pgame.h"
#include "../guielements/ptextfield.h"
#include "../guielements/pbutton.h"
#include "../guielements/pstacklayout.h"

struct PressEscape : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		P_UNUSED(loop);
        if (event.key.keysym.sym == SDLK_ESCAPE) {
			loop->quit();
		}
	}
};

int startX = 0;
int startY = 0;
std::shared_ptr<PRect> rect;

struct MouseDown : public PMouseDownListener{
	void on_event(PEventLoop* loop, SDL_Event event) override {
		rect = std::make_shared<PRect>(event.button.x, event.button.y, 1, 1, false);
		loop->add_moveable("test", rect);
		startX = event.button.x;
		startY = event.button.y;
	}
};

struct MouseMove: public PMouseMoveListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		if (rect) {
			rect->width(event.button.x - startX);
			rect->height(event.button.y - startY);
		}
	}
};

struct MouseUp: public PMouseUpListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		rect = nullptr;
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
	loop.add_mouse_down_listener(std::make_shared<MouseDown>());
	loop.add_mouse_move_listener(std::make_shared<MouseMove>());
	loop.add_mouse_up_listener(std::make_shared<MouseUp>());



    auto stacklayout = std::make_shared<PStackLayout>(PStackLayoutDirection::VERTICAL,10,10);
    auto button1 = std::make_shared<PButton>(0,10,100,20);
	auto style = button1->style();
	style.text = "run";
	button1->style(style);

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
