#ifndef PTIMER_H
#define PTIMER_H
#include <SDL2/SDL.h>
#include <chrono>
#include <iostream>
#include "pmath.h"

constexpr auto cast_mu_to_sec(unsigned long long mu) -> float {
    return mu / 1000000.0f;
}


struct PTimer {
	PTimer() = default;

	void start() { _start = std::chrono::high_resolution_clock::now(); }

	auto delta_ticks() -> double {
		const auto current = std::chrono::high_resolution_clock::now();
		const auto mu_delta = std::chrono::duration_cast<std::chrono::microseconds>(current - _start).count();
		_start = current;
        _current_time += mu_delta;
		return cast_mu_to_sec(mu_delta);
	}

   private:
	unsigned long long  _current_time = 0.0;
	std::chrono::high_resolution_clock::time_point _start;
	unsigned int _mu_sum;
};

#endif
