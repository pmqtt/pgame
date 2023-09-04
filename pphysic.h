#ifndef PPHYSIC_H
#define PPHYSIC_H
#include <SDL2/SDL.h>
#include "pprimitive.h"
#include "pcolider.h"
class PPhysicObject {
    public:
        PPhysicObject(std::shared_ptr<PDrawable> drawable) :
            _drawable(drawable), _gravity(0),_velocity{0,0},_velocity_direction(0),_restition(1){

        }
        
        PPhysicObject(std::shared_ptr<PDrawable> drawable, std::shared_ptr<PColider> colider) :
            _drawable(drawable), _gravity(0.0),_velocity{0,0},_velocity_direction(0),_restition(1),_colider(colider){
                _colide = false;
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

        void add_velocity(std::array<float,2> v){
            _velocity[0] += v[0];
            _velocity[1] += v[1];
        }

        // Set the velocity direction for this object in degrees
        void set_velocity_direction(float direction){
            _velocity_direction = direction;
        }

        void set_restitution(float value){
            _restition = value;
        }

        void colide(std::shared_ptr<PPhysicObject> other){
            if(_colider){
               _colide =  _colider->colide(_drawable,other->_drawable);
               _colide_point = std::array<float,2>{other->_drawable->x(),other->_drawable->y()};
            }else{
                _colide = false;
            }
        }

        void move(float delta_time){
            if(std::abs(_velocity[0]) < 0.001 && std::abs(_velocity[1]) < 0.001){
                return;
            }
            if( _colide ){
                _velocity[1] = _velocity[1] * -1 * _restition;
                auto box = _drawable->bounding_box();
                _drawable->add(std::array<float,2>{0,_colide_point[1] - box[1] - box[3] -0.001f});
                
            }else{
                _velocity[1] += _gravity * delta_time;
            }
            float x = _velocity[0] * delta_time;
            float y = _velocity[1] * delta_time;
            _drawable->add(std::array<float,2>{x,y});
        }

        auto drawable() const{
            return _drawable;
        }


    private:
        std::shared_ptr<PDrawable> _drawable;
        float _gravity;
        std::array<float,2> _velocity;
        float _velocity_direction;
        float _restition;
        std::shared_ptr<PColider> _colider;
        bool _colide;
        std::array<float,2> _colide_point;
};

#endif // PPHYSIC_H


