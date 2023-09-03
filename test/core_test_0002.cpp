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
    PWindow window("PGame",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1200,600);
    auto event_loop = window.create_event_loop();
    event_loop.add_key_down_listener(std::make_shared<PressEscape>());
    auto drawable = std::make_shared<PRect>(200,0,10,10);
    auto object = std::make_shared<PPhysicObject>(drawable);
    object->set_gravity(98.1);
    object->set_velocity(0,0.001);
    object->set_restitution(0.8);

    object->set_velocity_direction(M_PI);
    event_loop.add_physics_object("obj", object);
    event_loop.add_moveable("static", std::make_shared<PRect>(0,510,800,100,false));
    event_loop.moveables()["static"]->change_color({0,255,0,255});


    event_loop.run();
    return 0;
}
