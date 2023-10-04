#ifndef PGENERATOR_H
#define PGENERATOR_H
#include <memory>
#include <thread>

#include "pmath.h"

class PEventLoop;
class PWindow;

struct PGeneratorResult {
	float xvelocity;
	float yvelocity;
	float wdim;
	float hdim;
	float xpos;
	float ypos;
	float time;
};

class PGenerator {
   public:
	PGenerator(std::shared_ptr<PEventLoop> event_loop, std::shared_ptr<PWindow> window);
	~PGenerator();

	void xvelocity(float min, float max) { _xvelocity = PInterval(min, max); }
	void yvelocity(float min, float max) { _yvelocity = PInterval(min, max); }
	void wdim(float min, float max) { _wdim = PInterval(min, max); }
	void hdim(float min, float max) { _hdim = PInterval(min, max); }
	void xpos(float min, float max) { _xpos = PInterval(min, max); }
	void ypos(float min, float max) { _ypos = PInterval(min, max); }
	void time(float min, float max) { _time = PInterval(min, max); }

	auto generate() -> PGeneratorResult {
		PGeneratorResult result;
		result.xvelocity = _xvelocity.pick_random();
		result.yvelocity = _yvelocity.pick_random();
		result.wdim = _wdim.pick_random();
		result.hdim = _hdim.pick_random();
		result.xpos = _xpos.pick_random();
		result.ypos = _ypos.pick_random();
		result.time = _time.pick_random();
		return result;
	}

   private:
	std::shared_ptr<PEventLoop> _event_loop;
	std::shared_ptr<PWindow> _window;
	PInterval _xvelocity;
	PInterval _yvelocity;
	PInterval _wdim;
	PInterval _hdim;
	PInterval _xpos;
	PInterval _ypos;
	PInterval _time;
};

#endif	// PGENERATOR_H
