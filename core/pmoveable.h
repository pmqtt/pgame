#ifndef PMOVEABLE_H
#define PMOVEABLE_H
#include "pprimitive.h"
#include "ppoint2d.h"
#include "../physic/pcollider.h"

#include <optional>

enum class PLineKind{
    VERTICAL,
    HORIZONTAL,
    DIAGONAL
};


class PMoveable{
    public:
        PMoveable(std::shared_ptr<PDrawable> drawable, std::shared_ptr<PCollider> colider) : _drawable(drawable), _collider(colider) {}
        PMoveable(std::shared_ptr<PDrawable> drawable) : _drawable(drawable) {}



        void move(const PPoint2D& v) {
            _drawable->position(_drawable->x() + v.x(), _drawable->y() + v.y());
        }

        auto inpact_on_way(const PPoint2D &v, std::shared_ptr<PDrawable> other) -> std::optional<PPoint2D> {
            if(_collider){
                if(_collider->collide(_drawable, other)){
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
                
                //better with binary search
                switch (line_kind)
                {
                    case PLineKind::HORIZONTAL:
                        for(float y = y1; y < y2; y += 0.1){
                            _drawable->position(x1,y);
                            if(_collider->collide(_drawable, other)){
                                return PPoint2D(x1, y);
                            }
                        }
                        return std::nullopt;
                    case PLineKind::VERTICAL:
                        for(float x = x1; x < x2; x += 0.1){
                            _drawable->position(x,y1);
                            if(_collider->collide(_drawable, other)){
                                return PPoint2D(x, y1);
                            }
                        }
                        return std::nullopt;
                    case PLineKind::DIAGONAL:
                        for(float x = x1; x < x2; x += 0.1){
                            _drawable->position(x, line_function(x));
                            if(_collider->collide(_drawable, other)){
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
        std::shared_ptr<PCollider> _collider;
};



#endif // PMOVEABLE_H