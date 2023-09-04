#ifndef PDRAWSTRATEGY_H
#define PDRAWSTRATEGY_H
#include <SDL2/SDL.h>
#include <memory>
#include <array>
#include <iostream>

static std::array<float,2> world_to_iso(float x, float y){
    return std::array<float,2>{std::abs(x-y),(std::abs(x+y))/(float)2.0};
}

class PDrawStrategy{
    public:
        PDrawStrategy() : _rotate(false), _angle(0.0f) {}
        virtual ~PDrawStrategy() = default;
        virtual void draw_point(std::shared_ptr<SDL_Renderer> renderer, float x, float y) = 0;
        virtual void draw_line(std::shared_ptr<SDL_Renderer> renderer, float x1, float y1, float x2, float y2) = 0;
        void rotate(float angle, std::array<float,4> box){
            _rotate = true;
            _angle = angle;
            _box = box;
        }
    protected:
        bool _rotate;
        float _angle;
        std::array<float,4> _box;
};

class PDrawCartesianStrategy : public PDrawStrategy{
    public:
        PDrawCartesianStrategy() = default;
        ~PDrawCartesianStrategy() = default;
        void draw_point(std::shared_ptr<SDL_Renderer> renderer, float x, float y) override{
            if(_rotate){
                const float rad = _angle * M_PI / 180.0;
                const float xc = _box[0];
                const float yc = _box[1];
                const float xr = (x - xc) * cos(rad) - (y - yc) * sin(rad) + xc;
                const float yr = (x - xc) * sin(rad) + (y - yc) * cos(rad) + yc;
                SDL_RenderDrawPointF(renderer.get(), xr, yr);
                SDL_RenderDrawPointF(renderer.get(), xr+1, yr);
                SDL_RenderDrawPointF(renderer.get(), xr-1, yr);
                SDL_RenderDrawPointF(renderer.get(), xr, yr+1);
                SDL_RenderDrawPointF(renderer.get(), xr, yr-1);

            }else{
                SDL_RenderDrawPointF(renderer.get(), x, y);
            }
        }
        void draw_line(std::shared_ptr<SDL_Renderer> renderer, float x1, float y1, float x2, float y2) override{
           if(_rotate){
                const double rad = _angle * M_PI / 180.0;
                const float xc = _box[0];
                const float yc = _box[1];
                const float xr1 = (x1 - xc) * cos(rad) - (y1 - yc) * sin(rad) + xc;
                const float yr1 = (x1 - xc) * sin(rad) + (y1 - yc) * cos(rad) + yc;
                const float xr2 = (x2 - xc) * cos(rad) - (y2 - yc) * sin(rad) + xc;
                const float yr2 = (x2 - xc) * sin(rad) + (y2 - yc) * cos(rad) + yc;
                SDL_RenderDrawLineF(renderer.get(), xr1, yr1, xr2, yr2);
           }else{
            SDL_RenderDrawLineF(renderer.get(), x1, y1, x2, y2);
           }
        }
};

class PDrawIsometricStrategy: public PDrawStrategy{
    public:
        PDrawIsometricStrategy() = default;
        ~PDrawIsometricStrategy() = default;
        void draw_point(std::shared_ptr<SDL_Renderer> renderer, float x, float y) override{
            const auto p = world_to_iso(x,y);
            SDL_RenderDrawPointF(renderer.get(), p[0], p[1]);
        }
        void draw_line(std::shared_ptr<SDL_Renderer> renderer, float x1, float y1, float x2, float y2) override{
            const auto p1 = world_to_iso(x1,y1);
            const auto p2 = world_to_iso(x2,y2);
            SDL_RenderDrawLineF(renderer.get(), p1[0], p1[1], p2[0], p2[1]);
        }
};


#endif // PDRAWSTRATEGY_H
