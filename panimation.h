#ifndef PANIMATION_H
#define PANIMATION_H
#include <SDL2/SDL.h>

#include "pprimitive.h"
#include "ppath.h"

class PEventLoop;

struct PAnimation{
    public:
        PAnimation(std::shared_ptr<PDrawable> drawable)
            : _drawable(drawable), _path_index(-1){
        }

        PAnimation(std::shared_ptr<PDrawable> drawable, std::shared_ptr<PPath> path)
            : _drawable(drawable), _path(path), _path_index(0){
            
        }

        void draw(PEventLoop * loop,std::shared_ptr<SDL_Renderer> renderer);

        virtual void animate(PEventLoop* loop) { }
        virtual void animate(std::array<float,2> point,PEventLoop* loop) { }
        virtual void animate_on_path(PEventLoop* loop) { }

    private:
        auto go_to(const std::array<float,2> &point, PEventLoop* loop) -> bool;
    protected:
        std::shared_ptr<PDrawable> _drawable;
        std::shared_ptr<PPath> _path;
        int _path_index;
};


#endif



