#ifndef PDRAWSTRATEGY_H
#define PDRAWSTRATEGY_H
#include <SDL2/SDL.h>

#include <array>
#include <iostream>
#include <memory>

static std::array<float, 2> world_to_iso(float x, float y) {
	return std::array<float, 2>{std::abs(x - y), (std::abs(x + y)) / (float)2.0};
}

class PDrawStrategy {
   public:
	PDrawStrategy() = default;
	virtual ~PDrawStrategy() = default;
	virtual void draw_point(std::shared_ptr<SDL_Renderer> renderer, float x, float y) = 0;
	virtual void draw_line(std::shared_ptr<SDL_Renderer> renderer, float x1, float y1, float x2, float y2) = 0;

   protected:
};

class PDrawCartesianStrategy : public PDrawStrategy {
   public:
	PDrawCartesianStrategy() = default;
	~PDrawCartesianStrategy() = default;
	void draw_point(std::shared_ptr<SDL_Renderer> renderer, float x, float y) override {
		SDL_RenderDrawPointF(renderer.get(), x, y);
	}
	void draw_line(std::shared_ptr<SDL_Renderer> renderer, float x1, float y1, float x2, float y2) override {
		SDL_RenderDrawLineF(renderer.get(), x1, y1, x2, y2);
	}
};

class PDrawIsometricStrategy : public PDrawStrategy {
   public:
	PDrawIsometricStrategy() = default;
	~PDrawIsometricStrategy() = default;
	void draw_point(std::shared_ptr<SDL_Renderer> renderer, float x, float y) override {
		const auto p = world_to_iso(x, y);
		SDL_RenderDrawPointF(renderer.get(), p[0], p[1]);
	}
	void draw_line(std::shared_ptr<SDL_Renderer> renderer, float x1, float y1, float x2, float y2) override {
		const auto p1 = world_to_iso(x1, y1);
		const auto p2 = world_to_iso(x2, y2);
		SDL_RenderDrawLineF(renderer.get(), p1[0], p1[1], p2[0], p2[1]);
	}
};

#endif	// PDRAWSTRATEGY_H
