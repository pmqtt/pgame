#ifndef PSTYLE_H
#define PSTYLE_H
#include <optional>
#include <string>

struct PStyle{
    std::optional<int> x;
    std::optional<int> y;
    std::optional<int> width;
    std::optional<int> height;
    std::optional<std::array<unsigned char, 4>> color;
    std::optional<std::string> font_path;
    std::optional<std::size_t> font_size;
    std::optional<std::array<unsigned char, 4>> font_color;
    std::optional<std::string> text;
    std::optional<std::array<unsigned char, 4>> hover_color;
    std::optional<int> spacing;
};

#endif
