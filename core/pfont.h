#ifndef PFONT_H
#define PFONT_H
#include <string>
#include <SDL2/SDL_ttf.h>

struct PFont{
    PFont(const std::string& path, std::size_t size): 
        _font(TTF_OpenFont(path.c_str(), size)),
        _path(path), _size(size){}

    PFont(const PFont& font){
        _font = TTF_OpenFont(font._path.c_str(), font._size);
        _path = font._path;
        _size = font._size;
    }

    PFont(PFont&& font){
        _font = font._font;
        _path = font._path;
        _size = font._size;
        font._font = nullptr;
    }

    auto operator=(const PFont& font) -> PFont&{
        _font = TTF_OpenFont(font._path.c_str(), font._size);
        _path = font._path;
        _size = font._size;
        return *this;
    }

    auto operator=(PFont&& font) -> PFont&{
        _font = font._font;
        _path = font._path;
        _size = font._size;
        font._font = nullptr;
        return *this;
    }

    auto font() -> TTF_Font*{
        return _font;
    }

    ~PFont(){
        if(_font != nullptr){
            TTF_CloseFont(_font);
        }
    }

private:
    TTF_Font* _font;
    std::string _path;
    std::size_t _size;
};

#endif // PFONT_H
