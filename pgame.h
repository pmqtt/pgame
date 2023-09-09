#ifndef PGAME_H
#define PGAME_H
#include <SDL2/SDL.h>

#include <array>
#include <cmath>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "panimation.h"
#include "peventloop.h"
#include "ppath.h"
#include "pphysic.h"
#include "pprimitive.h"
#include "ptimer.h"
#include "pwindow.h"

class PGame {
   public:
	PGame() {}

	void init() {
		_window = std::make_shared<PWindow>("PGame");
		_event_loop = _window->create_event_loop();
	}

	void use_isometric_projection() {}

   private:
	std::shared_ptr<PWindow> _window;
	PEventLoop _event_loop;
};

#endif
