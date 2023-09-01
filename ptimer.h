#ifndef PTIMER_H
#define PTIMER_H
#include <SDL2/SDL.h>
struct PTimer{
        PTimer():
            _startTicks(0){
        }

        void start(){
            _startTicks = SDL_GetTicks();
        }

        auto delta_ticks()->double{
            auto currentTicks = SDL_GetTicks();
            auto delta = (currentTicks - _startTicks) / 1000.0;
            _startTicks = currentTicks;
            return delta;
        }

    private:
        int _startTicks;

};

#endif
