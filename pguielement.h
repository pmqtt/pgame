#ifndef PGUIELEMENT_H
#define PGUIELEMENT_H
#include <SDL2/SDL.h>
#include <memory>
#include <functional>

#include "pstyle.h"

class PGuiElement{
public:
    PGuiElement() = default;
    virtual ~PGuiElement() = default;
    virtual void draw(std::shared_ptr<SDL_Renderer> renderer) = 0;
    virtual auto is_clicked(int mouseX,int mouseY)const -> bool = 0;
    virtual void on_click() = 0;
    virtual void hover(bool hover) = 0;
    virtual void style(const PStyle & style) = 0;
    virtual auto style()const -> PStyle = 0;
    virtual void listener(std::function<void()> listener) = 0;
    virtual auto element() -> std::shared_ptr<PGuiElement> = 0;

    virtual auto fixed_content()const -> bool = 0;
};

#endif // PGUIELEMENT_H
