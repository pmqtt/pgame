#ifndef PPATH_H
#define PPATH_H
#include <array>
#include <vector>

struct PPath {
   public:
	PPath() : _loop(false) {}

	void add(const std::array<float, 2> &v) { _points.push_back(v); }

	void loop(bool loop) { this->_loop = loop; }

	auto loop() const -> bool { return _loop; }

	auto points() const -> const std::vector<std::array<float, 2>> & { return _points; }

   private:
	bool _loop;
	std::vector<std::array<float, 2>> _points;
};

#endif
