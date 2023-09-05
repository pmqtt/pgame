#ifndef PPHYSIC_H
#define PPHYSIC_H
#include <SDL2/SDL.h>
#include "pprimitive.h"
#include "pcolider.h"
#include "pmath.h"

class PPhysicObject;

struct PColision{
    PColision( std::array<float,2> point, float direction_angle, std::shared_ptr<PPhysicObject> colide_with) :
        point(point), direction_angle(direction_angle), colide_with(colide_with){
    }
    std::array<float,2> point;
    float direction_angle;
    std::shared_ptr<PPhysicObject> colide_with;
};

class PPhysicObject {
    public:
        PPhysicObject(std::shared_ptr<PDrawable> drawable) :
            _drawable(drawable), _gravity(0),_velocity{0,0},_velocity_direction(0),_restition(1),_acceleration(0){
                _colide = false;

        }
        
        PPhysicObject(std::shared_ptr<PDrawable> drawable, std::shared_ptr<PColider> colider) :
            _drawable(drawable), _gravity(0.0),_velocity{0,0},_velocity_direction(0),_restition(1),_colider(colider){
                _colide = false;
        }

        virtual ~PPhysicObject() = default;
        
        //Set the gravity for this object in pixels per second
        void gravity(float gravity){
            _gravity = gravity;
        }
        
        // Set the velocity for this object in pixels per second
        void velocity(float x, float y){
            _velocity[0] = x;
            _velocity[1] = y;
        }

        void velocity(const std::array<float,2> &v){
            _velocity[0] += v[0];
            _velocity[1] += v[1];
        }

        void acceleration(float value){
            _acceleration = value;;
        }

        // Set the velocity direction for this object in degrees
        auto velocity_direction() -> float{
            const float rad = atan2(_velocity[1],_velocity[0]);
            return radian_to_degree( rad );
        }

        void restitution(float value){
            _restition = value;
        }

        //TODO:
        // store direction_this and compare with previous direction_this to see if bouncing effect
        // create a colision object that stores the colision point and the direction_this of the colision...
        void colide(std::shared_ptr<PPhysicObject> other){
            if(_colider){
               _colide =  _colider->colide(_drawable,other->_drawable);
               if(_colide){
                   if(_colide_with == other){
                        float direction_this = velocity_direction();
                        if(P_DIRECTION::P_UP == angle_to_direction(direction_this)){
                            _colide = false;
                            return;
                        }
                   }
                   _colide_point = std::array<float,2>{_drawable->x(),_drawable->y()};
                   _colide_with = other;
                   return;
               }
            }
            _colide = false;
            _colide_point = std::array<float,2>{-1.0f,-1.0f};
            _colide_with = nullptr;
            
        }

        void move(float delta_time){
            if(NEAR_ZERO(_velocity[0]) && NEAR_ZERO(_velocity[1])){
                return;
            }
            
            if( _colide ){
                _velocity[1] = _velocity[1] * -1 * _restition;
            }else{
                _velocity[1] += _gravity * delta_time;
            }
            _velocity[0] += _acceleration * delta_time;
            
            if( NEAR_ZERO(_velocity[0]) ){
                _velocity[0] = 0;
            }

            float x = _velocity[0] * delta_time;
            float y = _velocity[1] * delta_time;
            if(abs(_velocity[1]) < 0.1){
                y = 0;
                _velocity[1] = 0.001;
            }
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
        float _acceleration;
        std::shared_ptr<PPhysicObject> _colide_with;
        std::shared_ptr<PColision> _colision;
};

#endif // PPHYSIC_H


