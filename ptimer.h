#ifndef PTIMER_H
#define PTIMER_H
#include <SDL2/SDL.h>

#include <chrono>
#include <iostream>

#include "pmath.h"
struct PTimer {
	PTimer() = default;

	void start() { _start = std::chrono::high_resolution_clock::now(); }

	auto delta_ticks() -> double {
		const auto current = std::chrono::high_resolution_clock::now();
		const auto mu_delta = std::chrono::duration_cast<std::chrono::microseconds>(current - _start).count();
		_start = current;
		float x = mu_delta / (1000000.0f);
		return x;
	}

   private:
	int _startTicks;
	std::chrono::high_resolution_clock::time_point _start;
	unsigned int _mu_sum;
};

#endif
