#ifndef PDRAWSTRATEGY_H
#define PDRAWSTRATEGY_H
#include <SDL2/SDL.h>
#include <memory>
class PDrawStrategy{
    public:
        PDrawStrategy() = default;
        virtual ~PDrawStrategy() = default;
        virtual void draw_point(std::shared_ptr<SDL_Renderer> renderer, int x, int y) = 0;
        virtual void draw_line(std::shared_ptr<SDL_Renderer> renderer, int x1, int y1, int x2, int y2) = 0;
};

class PDrawCartesianStrategy : public PDrawStrategy{
    public:
        PDrawCartesianStrategy() = default;
        ~PDrawCartesianStrategy() = default;
        void draw_point(std::shared_ptr<SDL_Renderer> renderer, int x, int y) override;
        void draw_line(std::shared_ptr<SDL_Renderer> renderer, int x1, int y1, int x2, int y2) override;
};


#endif // PDRAWSTRATEGY_H
