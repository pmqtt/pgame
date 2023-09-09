#include "../pcolider.h"
#include "../pgame.h"
#include "../pmath.h"

auto main(int argc, char** argv) -> int {
	P_UNUSED(argc);
	P_UNUSED(argv);
	auto drawable = std::make_shared<PRect>(205, 160, 10, 10);
	auto rect = std::make_shared<PRect>(200, 110, 300, 50);

	PSatColider colider;
	auto result = colider.colide(drawable, rect);

	PPhysicObject physic_object(drawable, std::make_shared<PSatColider>());

	std::array<float, 2> normals_up{0, 1};
	std::array<float, 2> normals_down{0, -1};
	std::array<float, 2> normals_left{-1, 0};
	std::array<float, 2> normals_right{1, 0};

	std::cout << "Direction: "
			  << physic_object.dir_to_string(angle_to_direction(radian_to_degree(atan2(normals_up[0], normals_up[1]))))
			  << std::endl;
	std::cout << "Direction: "
			  << physic_object.dir_to_string(
					 angle_to_direction(radian_to_degree(atan2(normals_down[0], normals_down[1]))))
			  << std::endl;
	std::cout << "Direction: "
			  << physic_object.dir_to_string(
					 angle_to_direction(radian_to_degree(atan2(normals_left[0], normals_left[1]))))
			  << std::endl;
	std::cout << "Direction: "
			  << physic_object.dir_to_string(
					 angle_to_direction(radian_to_degree(atan2(normals_right[0], normals_right[1]))))
			  << std::endl;

	return 0;
}
