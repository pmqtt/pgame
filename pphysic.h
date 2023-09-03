#ifndef PPHYSIC_H
#define PPHYSIC_H
#include <SDL2/SDL.h>
#include "pprimitive.h"

class PPhysicObject {
    public:
        PPhysicObject(std::shared_ptr<PDrawable> drawable) :
            _drawable(drawable), _gravity(0),_velocity{0,0},_velocity_direction(0),_restition(1){
            
                y_bottom = 500;

            }
        virtual ~PPhysicObject() = default;
        
        //Set the gravity for this object in pixels per second
        void set_gravity(float gravity){
            _gravity = gravity;
        }
        
        // Set the velocity for this object in pixels per second
        void set_velocity(float x, float y){
            _velocity[0] = x;
            _velocity[1] = y;
        }

        // Set the velocity direction for this object in degrees
        void set_velocity_direction(float direction){
            _velocity_direction = direction;
        }

        void set_restitution(float value){
            _restition = value;
        }

        void move(float delta_time){
            if(std::abs(_velocity[0]) < 0.001 && std::abs(_velocity[1]) < 0.001){
                return;
            }

            if( y_bottom -_drawable->y() < 0.001 && _velocity[1] > 0){
                _velocity[1] = _velocity[1] * -1 * _restition;
            }else{
                _velocity[1] += _gravity * delta_time;
            }
            float x = _velocity[0] * delta_time;
            float y = _velocity[1] * delta_time;
            if(_drawable->y() + y > y_bottom){
                _drawable->add(std::array<float,2>{x,y_bottom - _drawable->y()});
            }else{
                _drawable->add(std::array<float,2>{x,y});
            }
        }

        auto drawable() const{
            return _drawable;
        }


    private:
        float y_bottom;
        std::shared_ptr<PDrawable> _drawable;
        float _gravity;
        std::array<float,2> _velocity;
        float _velocity_direction;
        float _restition;
};

#endif // PPHYSIC_H


