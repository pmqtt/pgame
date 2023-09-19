#ifndef PCOLIDER_H
#define PCOLIDER_H
#include <optional>
#include <tuple>

#include "pprimitive.h"
#include "pmath.h"

struct PColider {
	virtual auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool = 0;
	virtual auto normals() const -> PPoint2D { return {0, 0}; }
	virtual auto normals2() const -> PPoint2D { return {0, 0}; }

	virtual auto edge_point1() const -> std::pair<PPoint2D, PPoint2D> {
		return {{0, 0}, {0, 0}};
	}

	virtual auto edge_point2() const -> std::pair<PPoint2D, PPoint2D> {
		return {{0, 0}, {0, 0}};
	}

    virtual auto mtv() const -> PPoint2D { return {0, 0}; }
};

struct PBoxColider : public PColider {
	auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool {
		const auto box1 = p1->bounding_box();  // array of x,y,w,h
		const auto box2 = p2->bounding_box();

		// Berechne die Seiten des Rechtecks A
		const float leftA = box1[0][0];	  // x
		const float rightA = box1[1][0];  //
		const float topA = box1[0][1];	  // y
		const float bottomA = box1[2][1];

		// Berechne die Seiten des Rechtecks B
		const float leftB = box2[0][0];	   // x
		const float rightB = box2[0][1];   // x+w
		const float topB = box2[0][1];	   // y
		const float bottomB = box2[2][1];  // y+h

		// Überprüfe die Konditionen
		if (bottomA <= topB) {
			return false;
		}

		if (topA >= bottomB) {
			return false;
		}

		if (rightA <= leftB) {
			return false;
		}

		if (leftA >= rightB) {
			return false;
		}

		return true;
	}
};

struct PCircleColider : public PColider {
	auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool override {
		P_UNUSED(p1);
		P_UNUSED(p2);
		/*const auto box1 = p1->bounding_box();
		const auto box2 = p2->bounding_box();
		const auto x1 = box1[0] + box1[2]/2;
		const auto y1 = box1[1] + box1[3]/2;
		const auto x2 = box2[0] + box2[2]/2;
		const auto y2 = box2[1] + box2[3]/2;
		const auto r1 = box1[2]/2;
		const auto r2 = box2[2]/2;
		const auto dx = x1 - x2;
		const auto dy = y1 - y2;
		const auto d = sqrt(dx*dx + dy*dy);
		return d < r1 + r2;*/
		return false;
	}
};

using PVertices2D = std::array<PPoint2D, 4>;


constexpr auto dot(const PPoint2D &p1, const PPoint2D &p2) -> float { return p1[0] * p2[0] + p1[1] * p2[1]; }

constexpr auto perpendicular(const PPoint2D &p) -> PPoint2D { return {-p[1], p[0]}; }

struct PSatColider : PColider {
	void prin_point_2d(const PPoint2D &p) const { std::cout << "(" << p[0] << "," << p[1] << ")"; }

	auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool override {
		PPoint2D smallestOverlapAxis1 = {0, 0};
		float smallestOverlap1 = std::numeric_limits<float>::max();
		PPoint2D smallestOverlapAxis2 = {0, 0};
		float smallestOverlap2 = std::numeric_limits<float>::max();

		// Check axes of shape1
		auto shape1 = p1->bounding_box();
		auto shape2 = p2->bounding_box();
		for (size_t i = 0; i < shape1.size(); ++i) {
			PPoint2D point1 = shape1[i];
			PPoint2D point2 = shape1[(i + 1) % shape1.size()];
			PPoint2D edge = point2 - point1;
			PPoint2D axis = perpendicular(edge);
            axis.normalize();
			float overlap = overlapAmount(axis, shape1, shape2);
			if (overlap < 0) {
				return false;
			}

			if (overlap < smallestOverlap1) {
				smallestOverlap1 = overlap;
				smallestOverlapAxis1 = axis;
				_edge_point1 = point1;
				_edge_point2 = point2;
			}
		}
		for (size_t i = 0; i < shape2.size(); ++i) {
			PPoint2D point1 = shape2[i];
			PPoint2D point2 = shape2[(i + 1) % shape2.size()];
			PPoint2D edge = point2 - point1;
			PPoint2D axis = perpendicular(edge);
            axis.normalize();
			float overlap = overlapAmount(axis, shape1, shape2);
			if (overlap < 0) {
				return false;
			}
			if (overlap < smallestOverlap2) {
				smallestOverlap2 = overlap;
				smallestOverlapAxis2 = axis;
				_edge_point3 = point1;
				_edge_point4 = point2;
			}
		}
		if (smallestOverlap1 < smallestOverlap2) {
			_collision_normal = smallestOverlapAxis1;
            _collision_normal2 = smallestOverlapAxis2;
            _mtv = {smallestOverlap1 * smallestOverlapAxis1[0], smallestOverlap1 * smallestOverlapAxis1[1]};
		} else {
			_collision_normal = smallestOverlapAxis2;
            _collision_normal2 = smallestOverlapAxis1;
            _mtv = {smallestOverlap2 * smallestOverlapAxis2[0], smallestOverlap2 * smallestOverlapAxis2[1]};
		}
		return true;
	}

	auto overlapAmount(const PPoint2D &axis, const PVertices2D &shape1, const PVertices2D &shape2) const -> float {
		float min1 = std::numeric_limits<float>::max();
		float max1 = std::numeric_limits<float>::min();
		for (const auto &point : shape1) {
			float projection = dot(axis, point);
			min1 = std::min(min1, projection);
			max1 = std::max(max1, projection);
		}

		float min2 = std::numeric_limits<float>::max();
		float max2 = std::numeric_limits<float>::min();
		for (const auto &point : shape2) {
			float projection = dot(axis, point);
			min2 = std::min(min2, projection);
			max2 = std::max(max2, projection);
		}
		return std::min(max1, max2) - std::max(min1, min2);
	}

	auto normals() const -> PPoint2D override { return _collision_normal; }
	auto normals2() const -> PPoint2D override { return _collision_normal2; }

	auto edge_point1() const -> std::pair<PPoint2D, PPoint2D> override {
		return {_edge_point1, _edge_point2};
	}

	auto edge_point2() const -> std::pair<PPoint2D, PPoint2D> override {
		return {_edge_point3, _edge_point4};
	}

    auto mtv() const -> PPoint2D override { return _mtv; }

   private:
	mutable PPoint2D _collision_normal = {0.0f, 0.0f};
	mutable PPoint2D _collision_normal2 = {0.0f, 0.0f};
	mutable PPoint2D _edge_point1 = {0.0f, 0.0f};
	mutable PPoint2D _edge_point2 = {0.0f, 0.0f};
	mutable PPoint2D _edge_point3 = {0.0f, 0.0f};
	mutable PPoint2D _edge_point4 = {0.0f, 0.0f};
    mutable PPoint2D _mtv = {0.0f, 0.0f};
};

#endif
