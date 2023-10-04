#ifndef PMOVEABLE_H
#define PMOVEABLE_H
#include "pprimitive.h"
#include "ppoint2d.h"
#include "pcolider.h"

#include <optional>

enum class PLineKind{
    VERTICAL,
    HORIZONTAL,
    DIAGONAL
}


class PMoveable{
    public:
        PMoveable(std::shared_ptr<PDrawable> drawable, std::shared_ptr<PColider> colider) : _drawable(drawable), _colider(colider) {}
        PMoveable(std::shared_ptr<PDrawable> drawable) : _drawable(drawable) {}



        void move(const PPoint2D& v) {

        }

    private:
        auto inpact_on_way(const PPoint2D &v, std::shared_ptr<PDrawable> other) -> std::optional<PPoint2D> {
            if(_colider){
                if(_colider->colide(_drawable, other)){
                    return PPoint2D(0,0);
                }

                const float x1 = _drawable->x();
                const float y1 = _drawable->y();
                const float x2 = _drawable->x() + v.x();
                const float y2 = _drawable->y() + v.y();

                const float dx = x2 - x1;
                const float dy = y2 - y1;
                float a = 0;
                float b = 0;

                auto line_function = [&](float x) -> float {
                    return a * x + b;
                };
                PLineKind line_kind;
                if(dx == 0){
                    b = x1;
                    line_kind = PLineKind::HORIZONTAL;
                }else if ( dy == 0){
                    b = y1;
                    line_kind = PLineKind::VERTICAL;    
                }else{
                    a = dy / dx;
                    b = y1 - a * x1;
                    line_kind = PLineKind::DIAGONAL;
                }
                

                switch (line_kind)
                {
                    case PLineKind::HORIZONTAL:
                        for(float y = y1; y < y2; y += 0.1){
                            _drawable->position(x1,y);
                            if(_colider->colide(_drawable, other)){
                                return PPoint2D(x1, y);
                            }
                        }
                        return std::nullopt;
                    case PLineKind::VERTICAL:
                        for(float x = x1; x < x2; x += 0.1){
                            _drawable->position(x,y1);
                            if(_colider->colide(_drawable, other)){
                                return PPoint2D(x, y1);
                            }
                        }
                        return std::nullopt;
                    case PLineKind::DIAGONAL:
                        for(float x = x1; x < x2; x += 0.1){
                            _drawable->position(x, line_function(x));
                            if(_colider->colide(_drawable, other)){
                                return PPoint2D(x, line_function(x));
                            }
                        }
                        return std::nullopt;
                    break;
                }
            }
            return std::nullopt;
        }
    private:
        std::shared_ptr<PDrawable> _drawable;
        std::shared_ptr<PColider> _colider;
};



#endif // PMOVEABLE_H