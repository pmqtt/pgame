#ifndef PGAME_H
#define PGAME_H
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <list>
#include <array>
#include <map>
#include <vector>
#include <cmath>
#include <iostream>

#include "ptimer.h"
#include "pprimitive.h"
#include "ppath.h"
#include "pwindow.h"
#include "peventloop.h"
#include "panimation.h"


class PGame{
    public:
        PGame(){}

        void init(){
            _window = std::make_shared<PWindow>("PGame");
            _event_loop = _window->create_event_loop();
        }
        
        void use_isometric_projection(){
            
        }
        

    private:
        std::shared_ptr<PWindow> _window;
        PEventLoop _event_loop;

};




#endif
