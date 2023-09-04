#include "../pgame.h"
#include "../pcolider.h"

struct PressEscape : public PKeyDownListener{
    void on_event(PEventLoop* loop, SDL_Event event) override{
        if(event.key.keysym.sym == SDLK_ESCAPE){
            loop->stop();
        }
    }
};

struct PressRight : public PKeyDownListener{
    void on_event(PEventLoop* loop, SDL_Event event){
        if(event.key.keysym.sym == SDLK_RIGHT){
            loop->physics_objects()["obj"]->set_acceleration(-4.0f);
        }
    }
};



auto main(int argc, char** argv) -> int{
    P_UNUSED(argc);
    P_UNUSED(argv);
    PWindow window("PGame",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1200,600);
    auto event_loop = window.create_event_loop();
    event_loop.add_key_down_listener(std::make_shared<PressEscape>());
    event_loop.add_key_down_listener(std::make_shared<PressRight>());
    auto drawable = std::make_shared<PRect>(200,0,10,10);
    auto object = std::make_shared<PPhysicObject>(drawable,std::make_shared<PBoxColider>());
    object->set_gravity(98.1);
    object->set_velocity(30,0.001);
    object->set_restitution(0.6);

    object->set_velocity_direction(2*M_PI);
    event_loop.add_physics_object("obj", object);

    auto object2 = std::make_shared<PPhysicObject>(std::make_shared<PRect>(0,510,800,100,false));
    event_loop.add_physics_object("obj2", object2);


    PBoxColider colider;
    event_loop.run();
    return 0;
}
