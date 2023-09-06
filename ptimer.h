#ifndef PTIMER_H
#define PTIMER_H
#include <SDL2/SDL.h>
#include <chrono>
#include <iostream>
#include "pmath.h"
struct PTimer{
        PTimer():
            _startTicks(0),_mu_sum(0){
        }

        void start(){
            //_startTicks = SDL_GetTicks();
            _start = std::chrono::high_resolution_clock::now();
        }

        auto delta_ticks()->double{
            //const auto currentTicks = SDL_GetTicks();
            const auto current = std::chrono::high_resolution_clock::now();
            const auto mu_delta = std::chrono::duration_cast<std::chrono::microseconds>(current - _start).count();
            _start = current;

            //auto delta = (currentTicks - _startTicks) / 1000.0;
            _mu_sum += mu_delta;
            if( _mu_sum  > 1000 ){
                const auto delta = _mu_sum / (1000.0*1000.0);
                _mu_sum = 0;
                return delta;
            }
            return 0; 
            //_startTicks = currentTicks;
            //return delta;
        }

    private:
        int _startTicks;
        std::chrono::high_resolution_clock::time_point _start;
        unsigned int _mu_sum;

};

#endif
