#ifndef PTEXTFIELD_H
#define PTEXTFIELD_H

#include "../core/pprimitive.h"
#include "../core/pfont.h"
#include "../core/pdebug.h"
#include "pguielement.h"

#include <chrono>
#include <memory>

class PTextField : public PGuiElement, public std::enable_shared_from_this<PTextField>{
public:
    PTextField(int x,int y ,int width,int height): _x(x),_y(y),_width(width),_height(height),_rect(x,y,width,height){
        _style.x = x;
        _style.y = y;
        _style.width = width;
        _style.height = height;
        _style.font_path = "/usr/share/fonts/TTF/Hack-Regular.ttf";
        _style.font_size = 12;
        _style.font_color = {{64,64,64,255}};
        _style.text = " ";
        _style.fixed_content = false;
        _last_blink_time = std::chrono::steady_clock::now();
    }

    auto is_clicked(int mouseX, int mouseY)const -> bool override {
        const auto box = _rect.bounding_box();
        return mouseX >= box[0].x()  && mouseX <= box[1].x() &&
               mouseY >= box[0].y() && mouseY <= box[2].y();
    }

    auto style()const -> PStyle override{
        return _style;
    }

    void style(const PStyle & style) override{
        _style = style;
        apply_style();
    }
   
    void draw(std::shared_ptr<SDL_Renderer> renderer) override{
        _rect.draw(renderer);
        // Render text
        std::string text = *_style.text;
        std::array<unsigned char, 4> font_color = *_style.font_color;
        SDL_Color text_color = {font_color[0],font_color[1],font_color[2],font_color[3]};
        SDL_Surface* text_surface = TTF_RenderText_Solid(_font->font(), text.c_str(), text_color);
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer.get(), text_surface);
        float text_width = text_surface->w;
        float text_height = text_surface->h;
        auto box = _rect.bounding_box();
        float h = std::abs(box[2].y() - box[0].y());

        SDL_FRect text_rect = {
                                box[0].x() + 5, 
                                box[0].y() + (h - text_height) / 2, 
                                text_width, text_height};

        SDL_RenderCopyF(renderer.get(), text_texture, nullptr, &text_rect);

        // Render blinking cursor if focused
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _last_blink_time).count();
        if (_mouse_over && duration < 500) {
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);  // Black color
            SDL_FRect cursor_rect = {text_rect.x + text_rect.w, text_rect.y, 2, text_rect.h};
            SDL_RenderFillRectF(renderer.get(), &cursor_rect);
        } else if (duration > 1000) {
            _last_blink_time = now;
        }

        // Cleanup
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }

    void on_click() override{
    }

    void hover(bool hover) override{
        _mouse_over = hover;
    }

    void listener(std::function<void()> listener) override{
        P_UNUSED(listener);
    }

    auto element() -> std::shared_ptr<PGuiElement> override{
        return shared_from_this();
    }

    auto fixed_content()const -> bool override{
        return false;
    }

private:

    void apply_style(){
        PRect tmp(*_style.x, *_style.y, *_style.width, *_style.height,true);
        _rect = tmp;
        _font = std::make_shared<PFont>(*_style.font_path, *_style.font_size);
        _rect.change_color(*_style.color);
    }

    PStyle _style;
    int _x;
    int _y;
    int _width;
    int _height;
    bool _mouse_over = false;
    PRect _rect;
    std::shared_ptr<PFont> _font;
    std::chrono::steady_clock::time_point _last_blink_time;


};


#endif // PTEXTFIELD_H
