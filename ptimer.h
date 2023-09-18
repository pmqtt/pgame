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

    PTimer(const PTimer& a){
        _current_time = a._current_time;
        _start = a._start;
        _mu_delta = a._mu_delta;
    }

    PTimer& operator=(const PTimer& a){
        _current_time = a._current_time;
        _start = a._start;
        _mu_delta = a._mu_delta;
        return *this;
    }

	void start() { _start = std::chrono::high_resolution_clock::now(); }

	void delta_ticks() {
		const auto current = std::chrono::high_resolution_clock::now();
		const auto mu_delta = std::chrono::duration_cast<std::chrono::microseconds>(current - _start).count();
		_start = current;
        _current_time += mu_delta;
        _mu_delta = mu_delta;
	}

    auto pick_time() const  -> double {
        return cast_mu_to_sec(_mu_delta);
    }

   private:
	unsigned long long  _current_time = 0.0;
	std::chrono::high_resolution_clock::time_point _start;
	double _mu_delta = 0.0f;
};

#endif
