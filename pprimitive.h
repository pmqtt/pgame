#ifndef PPRIMITIVE_H
#define PPRIMITIVE_H

#include <SDL2/SDL.h>

#include <array>
#include <memory>
#include <vector>

#include "pdebug.h"
#include "pdrawstrategy.h"
#include "pmath.h"

struct PDrawable {
   public:
	PDrawable(float x, float y, bool fill, std::shared_ptr<PDrawStrategy> strategy)
		: _color({255, 0, 0, 255}), _x(x), _y(y), _fill(fill), _strategy(strategy), _changed(true), _angle(0) {}

    PDrawable(const PDrawable& a){
        _color = a._color;
        _x = a._x;
        _y = a._y;
        _fill = a._fill;
        _strategy = a._strategy;
        _changed = a._changed;
        _angle = a._angle;
    }

	void draw(std::shared_ptr<SDL_Renderer> renderer) {
		SDL_SetRenderDrawColor(renderer.get(), _color[0], _color[1], _color[2], _color[3]);
		if (_fill) {
			draw_fill(renderer);
		} else {
			draw_empty(renderer);
		}
		_changed = false;
	}
	virtual void add(const std::array<float, 2> &v) {
		_x += v[0];
		_y += v[1];
		_changed = true;
	}

	virtual void position(float x, float y) {
		_x = x;
		_y = y;
		_changed = true;
	}

	virtual void draw_fill(std::shared_ptr<SDL_Renderer> renderer) = 0;
	virtual void draw_empty(std::shared_ptr<SDL_Renderer> renderer) = 0;

	void draw_line(std::shared_ptr<SDL_Renderer> renderer, float x1, float y1, float x2, float y2) {
		_strategy->draw_line(renderer, x1, y1, x2, y2);
	}
	void draw_point(std::shared_ptr<SDL_Renderer> renderer, float x, float y) { _strategy->draw_point(renderer, x, y); }

	void change_color(const std::array<unsigned char, 4> &color) { _color = color; }

	auto x() const -> float { return _x; }

	auto y() const -> float { return _y; }

	void fill(bool fill) {
		_fill = fill;
		_changed = true;
	}

	auto fill() const -> bool { return _fill; }

	virtual auto bounding_box() const -> std::array<std::array<float, 2>, 4> = 0;
	virtual auto bounding_circle() const -> std::array<float, 3> = 0;

	void rotate(float angle) { _angle = angle; };

	auto angle() const -> float { return _angle; }

    virtual auto clone() const -> std::shared_ptr<PDrawable> { return nullptr;}

   protected:
	std::array<unsigned char, 4> _color;
	float _x;
	float _y;
	bool _fill;
	std::shared_ptr<PDrawStrategy> _strategy;
	bool _changed;
	float _angle;
};

struct PRect : public PDrawable {
   public:
	PRect(float x, float y, float w, float h, bool fill = false,
		  std::shared_ptr<PDrawStrategy> strategy = std::make_shared<PDrawCartesianStrategy>())
		: PDrawable(x + w / 2, y + h / 2, fill, strategy), w(w), h(h) {}

    PRect(const PRect& a) : PDrawable(a){
        w = a.w;
        h = a.h;
    }

	void draw_fill(std::shared_ptr<SDL_Renderer> renderer) override {
		for (float y = _y - h / 2; y < _y + h / 2; y += 1) {
			for (float x = _x - x / 2; x < _x + w / 2; x += 1) {
				const auto point = rotate_point(_x, _y, x, y, degree_to_radian(_angle));
				draw_point(renderer, point[0], point[1]);
			}
		}
	}

	void draw_empty(std::shared_ptr<SDL_Renderer> renderer) override {
		const auto top_left = rotate_point(_x, _y, _x - w / 2, _y - h / 2, degree_to_radian(_angle));
		const auto top_right = rotate_point(_x, _y, _x + w / 2, _y - h / 2, degree_to_radian(_angle));
		const auto bottom_left = rotate_point(_x, _y, _x - w / 2, _y + h / 2, degree_to_radian(_angle));
		const auto bottom_right = rotate_point(_x, _y, _x + w / 2, _y + h / 2, degree_to_radian(_angle));
		draw_line(renderer, top_left[0], top_left[1], top_right[0], top_right[1]);
		draw_line(renderer, top_right[0], top_right[1], bottom_right[0], bottom_right[1]);
		draw_line(renderer, bottom_right[0], bottom_right[1], bottom_left[0], bottom_left[1]);
		draw_line(renderer, bottom_left[0], bottom_left[1], top_left[0], top_left[1]);
	}

	auto bounding_box() const -> std::array<std::array<float, 2>, 4> override {
		const float angle_rad = degree_to_radian(_angle);
		const float x1 = _x - w / 2;
		const float y1 = _y - h / 2;
		const float x2 = _x + w / 2;
		const float y2 = _y + h / 2;

		const auto top_left = rotate_point(_x, _y, x1, y1, angle_rad);
		const auto top_right = rotate_point(_x, _y, x2, y1, angle_rad);
		const auto bottom_left = rotate_point(_x, _y, x1, y2, angle_rad);
		const auto bottom_right = rotate_point(_x, _y, x2, y2, angle_rad);

		return {{top_left, top_right, bottom_right, bottom_left}};
	}

	auto bounding_circle() const -> std::array<float, 3> override {
		return {_x + w / 2, _y + h / 2, std::sqrt(w * w + h * h) / 2};
	}

    auto clone() const -> std::shared_ptr<PDrawable> override{
        return std::make_shared<PRect>(*this);
    }

   private:
	float w, h;
};

struct PRectIso : public PRect {
	PRectIso(float x, float y, float w, float h, bool fill = false)
		: PRect(x, y, w, h, fill, std::make_shared<PDrawIsometricStrategy>()) {}
};

struct PCircle : public PDrawable {
   public:
	PCircle(float x, float y, float r, bool fill = false,
			std::shared_ptr<PDrawStrategy> strategy = std::make_shared<PDrawCartesianStrategy>())
		: PDrawable(x, y, fill, strategy), r(r) {}

	void draw_empty(std::shared_ptr<SDL_Renderer> renderer) override {
		const int points = 100;	 // Anzahl der Punkte für die Kreiszeichnung
		float angle = 0.0f;
		float angleStep = 2.0f * M_PI / points;
		for (int i = 0; i < points; ++i) {
			float x1 = _x + r * cos(angle);
			float y1 = _y + r * sin(angle);
			angle += angleStep;
			float x2 = _x + r * cos(angle);
			float y2 = _y + r * sin(angle);
			draw_line(renderer, x1, y1, x2, y2);
		}
	}

	void draw_fill(std::shared_ptr<SDL_Renderer> renderer) override {
		for (float y = _y - r; y <= _y + r; y += 1) {
			for (float x = _x - r; x <= _x + r; x += 1) {
				float dx = x - _x;
				float dy = y - _y;
				if (dx * dx + dy * dy <= r * r) {
					draw_point(renderer, x, y);
				}
			}
		}
	}

	auto bounding_box() const -> std::array<std::array<float, 2>, 4> override {
		// calculate the 4 points of the bounding box
		const float x1 = _x - r;
		const float y1 = _y - r;
		const float x2 = _x + r;
		const float y2 = _y + r;
		const auto top_left = rotate_point(_x, _y, x1, y1, degree_to_radian(_angle));
		const auto top_right = rotate_point(_x, _y, x2, y1, degree_to_radian(_angle));
		const auto bottom_left = rotate_point(_x, _y, x1, y2, degree_to_radian(_angle));
		const auto bottom_right = rotate_point(_x, _y, x2, y2, degree_to_radian(_angle));

		return {{top_left, top_right, bottom_left, bottom_right}};
	}

	auto bounding_circle() const -> std::array<float, 3> override { return {_x, _y, r}; }

   private:
	float r;
	bool _fill;
};

struct PCircleIso : public PCircle {
	PCircleIso(float x, float y, float r, bool fill = false)
		: PCircle(x, y, r, fill, std::make_shared<PDrawIsometricStrategy>()) {}
};

#endif
