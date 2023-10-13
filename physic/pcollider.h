#ifndef PCOLIDER_H
#define PCOLIDER_H
#include <optional>
#include <tuple>

#include "../core/pmath.h"
#include "../core/pprimitive.h"

struct PCollider {
	virtual auto collide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool = 0;
	virtual auto normals() const -> PVector2D { return {0, 0}; }
	virtual auto normals2() const -> PVector2D { return {0, 0}; }

	virtual auto edge_point1() const -> std::pair<PVector2D, PVector2D> { return {{0, 0}, {0, 0}}; }

	virtual auto edge_point2() const -> std::pair<PVector2D, PVector2D> { return {{0, 0}, {0, 0}}; }

	virtual auto mtv() const -> PVector2D { return {0, 0}; }
};

struct PBoxCollider : public PCollider {
	auto collide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool override;
};

struct PCircleCollider : public PCollider {
	auto collide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool override;
};

using PVertices2D = std::array<PVector2D, 4>;
constexpr auto dot(const PVector2D &p1, const PVector2D &p2) -> float { return p1[0] * p2[0] + p1[1] * p2[1]; }
constexpr auto perpendicular(const PVector2D &p) -> PVector2D { return {-p[1], p[0]}; }


struct PSatCollider : PCollider {
	void prin_point_2d(const PVector2D &p) const { std::cout << "(" << p[0] << "," << p[1] << ")"; }

	auto collide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool override ;

	auto overlapAmount(const PVector2D &axis, const PVertices2D &shape1, const PVertices2D &shape2) const -> float ;

	auto normals() const -> PVector2D override { return _collision_normal; }
	auto normals2() const -> PVector2D override { return _collision_normal2; }

	auto edge_point1() const -> std::pair<PVector2D, PVector2D> override { return {_edge_point1, _edge_point2}; }

	auto edge_point2() const -> std::pair<PVector2D, PVector2D> override { return {_edge_point3, _edge_point4}; }

	auto mtv() const -> PVector2D override { return _mtv; }

   private:
	mutable PVector2D _collision_normal = {0.0f, 0.0f};
	mutable PVector2D _collision_normal2 = {0.0f, 0.0f};
	mutable PVector2D _edge_point1 = {0.0f, 0.0f};
	mutable PVector2D _edge_point2 = {0.0f, 0.0f};
	mutable PVector2D _edge_point3 = {0.0f, 0.0f};
	mutable PVector2D _edge_point4 = {0.0f, 0.0f};
	mutable PVector2D _mtv = {0.0f, 0.0f};
};

#endif
