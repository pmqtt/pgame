#ifndef PPRIMITIVE_H
#define PPRIMITIVE_H

#include <SDL2/SDL.h>
#include <memory>
#include <array>

struct PDrawable{
    public:
        PDrawable(float x,float y) : _color({255,0,0,255}),_x(x),_y(y) {}
            
        virtual void draw(std::shared_ptr<SDL_Renderer> renderer) = 0;
        virtual void add(const std::array<float,2> &v) { }

        void change_color(const std::array<unsigned char,4> &color){
            _color = color;
        }

        auto x() const -> float{
            return _x;
        }

        auto y() const -> float{
            return _y;
        }

    protected:
        std::array<unsigned char,4> _color;
        float _x;
        float _y;

};

struct PRect : public PDrawable{
    public:
        PRect(float x, float y, float w, float h)
            : PDrawable(x,y), w(w), h(h){
        }
        
        void draw(std::shared_ptr<SDL_Renderer> renderer){
            SDL_FRect rect;
            rect.x = _x;
            rect.y = _y;
            rect.w = w;
            rect.h = h;
            SDL_SetRenderDrawColor(renderer.get(), _color[0], _color[1], _color[2], _color[3]);
            SDL_RenderFillRectF(renderer.get(), &rect);
        }
        
        void add(const std::array<float,2> &v){
            _x += v[0];
            _y += v[1];
        }
    private:
        float  w, h;
};

struct PCircle : public PDrawable{
    public:
        PCircle(float x, float y, float r, bool fill = false)
            : PDrawable(x,y), r(r), _fill(fill){
        }

        void draw(std::shared_ptr<SDL_Renderer> renderer){
            if(_fill){
                draw_fill(renderer);
            }else{
                draw_empty(renderer);
            }
        }
    
        void fill(bool fill){
            _fill = fill;
        }

        auto fill() const -> bool{
            return _fill;
        }

        void draw_empty(std::shared_ptr<SDL_Renderer> renderer){
            const int points = 100;  // Anzahl der Punkte für die Kreiszeichnung
            float angle = 0.0f;
            float angleStep = 2.0f * M_PI / points;
            for (int i = 0; i < points; ++i){
                float x1 = _x + r * cos(angle);
                float y1 = _y + r * sin(angle);
                angle += angleStep;
                float x2 = _x + r * cos(angle);
                float y2 = _y + r * sin(angle);
                SDL_RenderDrawLineF(renderer.get(), x1, y1, x2, y2);
            }
        }

        void draw_fill(std::shared_ptr<SDL_Renderer> renderer){
            for (float y = _y - r; y <= _y + r; y+=0.1){
                for (float x = _x - r; x <= _x + r; x +=0.1){
                    float dx = x - _x;
                    float dy = y - _y;
                    if (dx * dx + dy * dy <= r * r){
                        SDL_RenderDrawPointF(renderer.get(), x, y);
                    }
                }
            }
        }

        void add(const std::array<float,2> &v){
            _x += v[0];
            _y += v[1];
        }

    private:
        float r;
        bool _fill;
};

#endif
