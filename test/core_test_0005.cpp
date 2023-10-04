#include "../core/pgame.h"
#include "../core/pmath.h"

auto main(int argc, char** argv) -> int {
	P_UNUSED(argc);
	P_UNUSED(argv);
	unsigned long long a = std::numeric_limits<unsigned long long>::max();
	unsigned long long b = 1000 * 1000 * 60;
	unsigned long long c = a / b;
	c = c / 60;	  // hours
	c = c / 24;	  // days
	c = c / 365;  // years

	std::cout << sizeof(unsigned long long) * 8 << " " << c << std::endl;
	return 0;
}
