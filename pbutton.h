#ifndef PBUTTON_H
#define PBUTTON_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "pprimitive.h"
#include "pfont.h"


class PButton{
public:
    PButton(int x,int y,int w,int h): _rect(x,y,w,h){}


    auto is_clicked(int mouseX, int mouseY)const -> bool {
        const auto box = _rect.bounding_box();
        return mouseX >= box[0].x()  && mouseX <= box[1].x() &&
               mouseY >= box[0].y() && mouseY <= box[2].y();
    }

    void draw(std::shared_ptr<SDL_Renderer> renderer) {
        _rect.draw(renderer);
        std::string text = "close";
        SDL_Color textColor = {64, 64, 64, 255};
        _font = std::make_shared<PFont>("/usr/share/fonts/TTF/Hack-Bold.ttf", 12);
        SDL_Surface* textSurface = TTF_RenderText_Solid(_font->font(), text.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer.get(), textSurface);
        float text_width = textSurface->w;
        float text_height = textSurface->h;
        auto box = _rect.bounding_box();
        SDL_FRect text_rect = 
            {box[0].x() + ( std::abs(box[0].x() - box[1].x()) - text_width) / 2,
             box[1].y() + ( std::abs(box[1].y() - box[2].y()) - text_height) / 2, 
             text_width, text_height};
        
        SDL_RenderCopyF(renderer.get(), textTexture, nullptr, &text_rect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    void on_click(){
        std::cout << "Button clicked" << std::endl;
    }
private:
    PRect _rect;
    std::shared_ptr<PFont> _font;
    
};

#endif // PBUTTON_H


