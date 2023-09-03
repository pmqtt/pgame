#include "../pgame.h"

struct PressEscape : public PKeyDownListener{
    void on_event(PEventLoop* loop, SDL_Event event) override{
        if(event.key.keysym.sym == SDLK_ESCAPE){
            loop->stop();
        }
    }
};



auto main(int argc, char** argv) -> int{
    P_UNUSED(argc);
    P_UNUSED(argv);
    PWindow window("PGame");
    auto event_loop = window.create_event_loop();
    event_loop.add_key_down_listener(std::make_shared<PressEscape>());

    event_loop.run();
    return 0;
}
