#ifndef PPRIMITIVE_H
#define PPRIMITIVE_H

#include <SDL2/SDL.h>
#include <memory>
#include <array>
#include <vector>

#include "pdebug.h"
#include "pdrawstrategy.h"

struct PDrawable{
    public:
        PDrawable(float x,float y, bool fill, std::shared_ptr<PDrawStrategy> strategy) :
            _color({255,0,0,255}),_x(x),_y(y),_fill(fill),_strategy(strategy),_changed(true){ 
            }
            
         void draw(std::shared_ptr<SDL_Renderer> renderer){
            SDL_SetRenderDrawColor(renderer.get(), _color[0], _color[1], _color[2], _color[3]);
            if(_fill){
                draw_fill(renderer);
            }
            else{
                draw_empty(renderer);
            }
            _changed = false;
        }
        virtual void add(const std::array<float,2> &v) { 
            _x += v[0];
            _y += v[1];
            _changed = true;
        }

        virtual void draw_fill(std::shared_ptr<SDL_Renderer> renderer) = 0;
        virtual void draw_empty(std::shared_ptr<SDL_Renderer> renderer) = 0;
        
        void draw_line(std::shared_ptr<SDL_Renderer> renderer, float x1, float y1, float x2, float y2){
            _strategy->draw_line(renderer, x1, y1, x2, y2);
        }
        void draw_point(std::shared_ptr<SDL_Renderer> renderer, float x, float y){
            _strategy->draw_point(renderer, x, y);
        }

        void change_color(const std::array<unsigned char,4> &color){
            _color = color;
        }

        auto x() const -> float{
            return _x;
        }

        auto y() const -> float{
            return _y;
        }
        
        void fill(bool fill){
            _fill = fill;
            _changed = true;
        }

        auto fill() const -> bool{
            return _fill;
        }


    protected:
        std::array<unsigned char,4> _color;
        float _x;
        float _y;
        bool _fill;
        std::shared_ptr<PDrawStrategy> _strategy;
        bool _changed;

};

struct PRect : public PDrawable{
    public:
        PRect(float x, float y, 
              float w, float h, 
              bool fill = false, 
              std::shared_ptr<PDrawStrategy> strategy = std::make_shared<PDrawCartesianStrategy>())
            : PDrawable(x,y,fill,strategy), w(w), h(h){
        }

        void draw_fill(std::shared_ptr<SDL_Renderer> renderer) override{
            SDL_FRect rect;
            rect.x = _x;
            rect.y = _y;
            rect.w = w;
            rect.h = h;
            SDL_RenderFillRectF(renderer.get(), &rect);
        }

        void draw_empty(std::shared_ptr<SDL_Renderer> renderer) override{
            draw_line(renderer, _x, _y, _x+w, _y);
            draw_line(renderer, _x+w, _y, _x+w, _y+h);
            draw_line(renderer, _x+w, _y+h, _x, _y+h);
            draw_line(renderer, _x, _y+h, _x, _y);
        }
        

    private:
        float  w, h;
};

struct PRectIso: public PRect{
    PRectIso(float x, float y, float w, float h, bool fill = false)
        : PRect(x,y,w,h,fill, std::make_shared<PDrawIsometricStrategy>()){
        }
};


struct PCircle : public PDrawable{
    public:
        PCircle(float x, float y, 
                float r, bool fill = false,
                std::shared_ptr<PDrawStrategy> strategy = std::make_shared<PDrawCartesianStrategy>()
                )
            : PDrawable(x,y,fill,strategy), r(r) {
        }

        void draw_empty(std::shared_ptr<SDL_Renderer> renderer) override{
            const int points = 100;  // Anzahl der Punkte für die Kreiszeichnung
            float angle = 0.0f;
            float angleStep = 2.0f * M_PI / points;
            for (int i = 0; i < points; ++i){
                float x1 = _x + r * cos(angle);
                float y1 = _y + r * sin(angle);
                angle += angleStep;
                float x2 = _x + r * cos(angle);
                float y2 = _y + r * sin(angle);
                draw_line(renderer, x1, y1, x2, y2);
            }
        }

        void draw_fill(std::shared_ptr<SDL_Renderer> renderer) override{
            for (float y = _y - r; y <= _y + r; y+=1){
                for (float x = _x - r; x <= _x + r; x +=1){
                    float dx = x - _x;
                    float dy = y - _y;
                    if (dx * dx + dy * dy <= r * r){
                        draw_point(renderer, x, y);
                    }
                }
            }
        }


    private:
        float r;
        bool _fill;
};

struct PCircleIso : public PCircle{
    PCircleIso(float x, float y, float r, bool fill = false)
        : PCircle(x,y,r,fill, std::make_shared<PDrawIsometricStrategy>()){
        }
};


#endif
