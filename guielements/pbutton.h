#ifndef PBUTTON_H
#define PBUTTON_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../core/pprimitive.h"
#include "../core/pfont.h"
#include "../core/pstyle.h"
#include "pguielement.h"

#include <memory>

class PButton : public PGuiElement, public std::enable_shared_from_this<PButton>{
public:
    PButton() : _rect(0,0,0,0,true){
        std::cout<<"Constructor PButton 1"<<std::endl;
    }

    PButton(int x,int y,int w,int h): _rect(x,y,w,h,true){
        std::cout<<"Constructor PButton 2"<<std::endl;
        _style.x = x;
        _style.y = y;
        _style.width = w;
        _style.height = h;
        _style.color = {{255,255,255,255}};
        _style.font_path = "/usr/share/fonts/TTF/Hack-Regular.ttf";
        _style.font_size = 12;
        _style.font_color = {{64,64,64,255}};
        _style.text = "Button";
        _style.hover_color = {{128,128,255,255}};
        _style.fixed_content = true;
        apply_style();
    }

    PButton(const PStyle & style) : _style(style),_rect(0,0,0,0,true){
        apply_style();
    }


    auto is_clicked(int mouseX, int mouseY)const -> bool override {
        const auto box = _rect.bounding_box();
        return mouseX >= box[0].x()  && mouseX <= box[1].x() &&
               mouseY >= box[0].y() && mouseY <= box[2].y();
    }

    void draw(std::shared_ptr<SDL_Renderer> renderer) override {
        _rect.draw(renderer);
        std::array<unsigned char, 4> backgorund_color = *_style.color;
        _rect.change_color(backgorund_color);
        if(_mouse_over){
            std::array<unsigned char, 4> hover_color = *_style.hover_color;
            _rect.change_color(hover_color);
        }
        std::string text = *_style.text;
        std::array<unsigned char, 4> font_color = *_style.font_color;
        SDL_Color text_color = {font_color[0],font_color[1],font_color[2],font_color[3]};
        SDL_Surface* text_surface = TTF_RenderText_Solid(_font->font(), text.c_str(), text_color);
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer.get(), text_surface);
        float text_width = text_surface->w;
        float text_height = text_surface->h;
        auto box = _rect.bounding_box();
        SDL_FRect text_rect = 
            {box[0].x() + ( std::abs(box[0].x() - box[1].x()) - text_width) / 2,
             box[1].y() + ( std::abs(box[1].y() - box[2].y()) - text_height) / 2, 
             text_width, text_height};
        SDL_RenderCopyF(renderer.get(), text_texture, nullptr, &text_rect);
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);
    }

    void on_click() override{
        if(_listeners){
            _listeners();
        }
    }

    void hover(bool flag) override{
        _mouse_over = flag;
    }

    auto style()const -> PStyle override{
        return _style;
    }

    void style(const PStyle & style) override{
        _style = style;
        apply_style();
    }

    void listener(std::function<void()> listener) override{
        _listeners = listener;
    }
    
    auto element() -> std::shared_ptr<PGuiElement> override{
        return shared_from_this();
    }

    auto fixed_content()const -> bool override{
        return true;
    }

private:
    void apply_style(){
        PRect tmp(*_style.x, *_style.y, *_style.width, *_style.height,true);
        _rect = tmp;
        _font = std::make_shared<PFont>(*_style.font_path, *_style.font_size);
        _rect.change_color(*_style.color);
    }

    PStyle _style;
    PRect _rect;
    std::shared_ptr<PFont> _font;
    bool _mouse_over = false;
    std::function<void()> _listeners;
    
};

#endif // PBUTTON_H
