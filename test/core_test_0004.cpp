#include "../pcolider.h"
#include "../pgame.h"

auto main(int argc, char** argv) -> int {
	P_UNUSED(argc);
	P_UNUSED(argv);
	auto drawable = std::make_shared<PRect>(500, 0, 10, 10);

	std::array<std::array<float, 2>, 4> boundix_box = drawable->bounding_box();
	std::array<std::array<float, 2>, 4> expected_box = {std::array<float, 2>{500, 0}, {510, 0}, {510, 10}, {500, 10}};

	for (std::size_t i = 0; i < 4; ++i) {
		if (boundix_box[i][0] != expected_box[i][0] || boundix_box[i][1] != expected_box[i][1]) {
			std::cout << "Error: " << boundix_box[i][0] << " != " << expected_box[i][0] << " || " << boundix_box[i][1]
					  << " != " << expected_box[i][1] << std::endl;
			return 1;
		}
	}

	return 0;
}
