#ifndef PTIMER_H
#define PTIMER_H
#include <SDL2/SDL.h>
#include "pmath.h"
struct PTimer{
        PTimer():
            _startTicks(0){
        }

        void start(){
            _startTicks = SDL_GetTicks();
        }

        auto delta_ticks()->double{
            const auto currentTicks = SDL_GetTicks();
            auto delta = (currentTicks - _startTicks) / 1000.0;
            if(delta <= 0){
                return delta;
            }
            _startTicks = currentTicks;
            return delta;
        }

    private:
        int _startTicks;

};

#endif
