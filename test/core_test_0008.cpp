#include "../pcolider.h"
#include "../pgame.h"
std::array<float, 2> intersectionWithSide(const std::array<float, 2>& start, const std::array<float, 2>& end, float m,
										  float c);

std::vector<std::array<float, 2>> findIntersections(const std::array<std::array<float, 2>, 4>& rect,
													const std::array<float, 2>& point,
													const std::array<float, 2>& velocity) {
	std::vector<std::array<float, 2>> intersections;

	// Determine the equation of the line from the point and velocity
	float m = (std::abs(velocity[0]) > 1e-6) ? velocity[1] / velocity[0] : 1e6;	 // slope of the line
	float c = point[1] - m * point[0];											 // y-intercept

	for (int i = 0; i < 4; i++) {
		auto intersection = intersectionWithSide(rect[i], rect[(i + 1) % 4], m, c);
		if (!std::isnan(intersection[0])) {
			intersections.push_back(intersection);
		}
	}

	return intersections;
}

std::array<float, 2> intersectionWithSide(const std::array<float, 2>& start, const std::array<float, 2>& end, float m2,
										  float c2) {
	float dx = end[0] - start[0];
	float dy = end[1] - start[1];

	float m1 = (std::abs(dx) > 1e-6) ? dy / dx : 1e6;  // slope of the side
	float c1 = start[1] - m1 * start[0];			   // y-intercept of the side

	if (std::abs(m1 - m2) < 1e-6) {				// Parallel lines
		return {std::nanf(""), std::nanf("")};	// No intersection
	}

	float x = (c1 - c2) / (m2 - m1);
	float y = m2 * x + c2;

	if (x >= std::min(start[0], end[0]) && x <= std::max(start[0], end[0]) && y >= std::min(start[1], end[1]) &&
		y <= std::max(start[1], end[1])) {
		return {x, y};
	}

	return {std::nanf(""), std::nanf("")};
}

auto main(int argc, char** argv) -> int {
	P_UNUSED(argc);
	P_UNUSED(argv);
	auto drawable = std::make_shared<PRect>(446.183 - 5, 224.934 - 5, 10, 10);
	auto object = std::make_shared<PPhysicObject>(drawable, std::make_shared<PSatColider>());
	object->velocity(117.178, -48.3187);

	auto static_draw2 = std::make_shared<PRect>(400, 170, 140, 50, false);

	auto box = static_draw2->bounding_box();

	float m = (std::abs(object->velocity()[0]) > 1e-6) ? object->velocity()[1] / object->velocity()[0]
													   : 1e6;  // slope of the line
	float c = drawable->y() - m * drawable->x();			   // y-intercept
	auto res = intersectionWithSide(box[2], box[3], m, c);

	std::cout << drawable->x() << " , " << drawable->y() << std::endl;
	std::cout << res[0] << "," << res[1] << std::endl;

	return 0;
}
