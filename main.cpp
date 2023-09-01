#include <iostream>
#include "pgame.h"
#include "pdebug.h"

struct PressEscape : public PKeyDownListener{
    void on_event(PEventLoop* loop, SDL_Event event) override{
        if(event.key.keysym.sym == SDLK_ESCAPE){
            loop->stop();
        }
    }
};

struct PressRight : public PKeyDownListener{
    void on_event(PEventLoop* loop, SDL_Event event) override{
        if(event.key.keysym.sym == SDLK_RIGHT){
            loop->moveables()["player1"]->add(std::array<float,2>{1,0});
        }
    }
};

struct PressLeft : public PKeyDownListener{
    void on_event(PEventLoop* loop, SDL_Event event) override{
        if(event.key.keysym.sym == SDLK_LEFT){
            loop->moveables()["player1"]->add(std::array<float,2>{-1,0});
        }
    }
};

struct PressUp : public PKeyDownListener{
    void on_event(PEventLoop* loop, SDL_Event event) override{
        if(event.key.keysym.sym == SDLK_UP){
            loop->moveables()["player1"]->add(std::array<float,2>{0,-1});
        }
    }
};

struct PressDown : public PKeyDownListener{
    void on_event(PEventLoop* loop, SDL_Event event) override{
        if(event.key.keysym.sym == SDLK_DOWN){
            loop->moveables()["player1"]->add(std::array<float,2>{0,1});
        }
    }
};

struct RectAnimation : public PAnimation{
    RectAnimation(std::shared_ptr<PDrawable> drawable) : PAnimation(drawable){
        direction = 0;
        steps = 0;
        stepFrame = 0.0;
    }

    void animate(PEventLoop* loop) override{
        P_UNUSED(loop);
        float stepSize = 100 *loop->delta_time();

        if (direction == 0) {
            _drawable->add(std::array<float,2>{stepSize,0});
        }
        if (direction == 1) {
            _drawable->add(std::array<float,2>{0,stepSize});
        }
        if (direction == 2) {
            _drawable->add(std::array<float,2>{-stepSize,0});
        }
        if (direction == 3) {
            _drawable->add(std::array<float,2>{0,-stepSize});
        }

        steps += stepSize;
        //Change the direction after 100 steps
        if (steps >= 100){
            steps = 0;
            direction = (direction + 1) % 4;
        }
        stepFrame = 0.0;
        
    }
    int direction;
    float steps;
    float stepFrame;
};

struct PathAnimation : public PAnimation{
    PathAnimation(std::shared_ptr<PRect> rect, std::shared_ptr<PPath> path) : PAnimation(rect,path){
        index = 0;
        index2 = 0;
    }

    void animate(std::array<float,2> point,PEventLoop* loop) override{
        P_UNUSED(point);
        P_UNUSED(loop);
        _drawable->change_color({128,index,128,255});
        index = (index + 10) % 255;
    }

    void animate_on_path(PEventLoop* loop) override{
        P_UNUSED(loop);
        _drawable->change_color({index2,index,128,255});
        index2 = (index2 + 13) % 255;
    }

    unsigned char index;
    unsigned char index2;
};

auto main(int argc,char **argv) -> int{
    P_UNUSED(argc);
    P_UNUSED(argv);
    PWindow window("PGame");
    PEventLoop loop = window.create_event_loop();
    loop.add_key_down_listener(std::make_shared<PressEscape>());
    loop.add_key_down_listener(std::make_shared<PressRight>());
    loop.add_key_down_listener(std::make_shared<PressLeft>());
    loop.add_key_down_listener(std::make_shared<PressUp>());
    loop.add_key_down_listener(std::make_shared<PressDown>());
    loop.add_moveable("player1",std::make_shared<PRect>(100,100,4,4));
    loop.moveables()["player1"]->change_color({255,0,0,255});
    loop.add_animation("animation1",std::make_shared<RectAnimation>(
                                                        std::make_shared<PCircle>(50,50,8)));
 
    PPath path;
    path.add(std::array<float,2>{200,200});
    path.add(std::array<float,2>{300,200});
    path.add(std::array<float,2>{400,170});
    path.loop(true);
    loop.add_animation("animation2",std::make_shared<PathAnimation>(
                                                        std::make_shared<PRect>(200,200,16,16),
                                                        std::make_shared<PPath>(path)));

    loop.run();
    return 0;
}
