#ifndef PCONFIGURATION_H
#define PCONFIGURATION_H
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>

class PConfiguration {
   public:
	PConfiguration() = default;

	template <class T>
	auto get(const std::string &key) const -> std::optional<T> {
		if constexpr (std::is_same_v<T, float>) {
			if (_floats.find(key) != _floats.end()) {
				return _floats.at(key);
			}
			return {};
		}
		if constexpr (std::is_same_v<T, int>) {
			if (_ints.find(key) != _ints.end()) {
				return _ints.at(key);
			}
			return {};
		}
		if constexpr (std::is_same_v<T, std::string>) {
			if (_strings.find(key) != _strings.end()) {
				return _strings.at(key);
			}
			return {};
		}
		if constexpr (std::is_same_v<T, bool>) {
			if (_bools.find(key) != _bools.end()) {
				return _bools.at(key);
			}
			return {};
		}
		if (std::is_same_v<T, std::array<float, 2>>) {
			if (_points.find(key) != _points.end()) {
				return _points.at(key);
			}
			return {};
		}
		if (std::is_same_v<T, std::array<float, 4>>) {
			if (_colors.find(key) != _colors.end()) {
				return _colors.at(key);
			}
			return {};
		} else {
			static_assert(std::is_same_v<T, float> || std::is_same_v<T, int> || std::is_same_v<T, std::string> ||
							  std::is_same_v<T, bool> || std::is_same_v<T, std::array<float, 2>> ||
							  std::is_same_v<T, std::array<float, 4>>,
						  "Type not supported");
		}
	}

	template <class T>
	void set(const std::string &key, const T &value) {
		if constexpr (std::is_same_v<T, float>) {
			_floats[key] = value;
		}
		if constexpr (std::is_same_v<T, int>) {
			_ints[key] = value;
		}
		if constexpr (std::is_same_v<T, std::string>) {
			_strings[key] = value;
		}
		if constexpr (std::is_same_v<T, bool>) {
			_bools[key] = value;
		} else {
			static_assert(std::is_same_v<T, float> || std::is_same_v<T, int> || std::is_same_v<T, std::string> ||
							  std::is_same_v<T, bool> || std::is_same_v<T, std::array<float, 2>> ||
							  std::is_same_v<T, std::array<float, 4>>,
						  "Type not supported");
		}
	}

   private:
	std::map<std::string, float> _floats;
	std::map<std::string, int> _ints;
	std::map<std::string, std::string> _strings;
	std::map<std::string, bool> _bools;
	std::map<std::string, std::array<float, 2>> _points;
	std::map<std::string, std::array<float, 4>> _colors;
};

#endif
