#ifndef PPHYSIC_H
#define PPHYSIC_H
#include <SDL2/SDL.h>
#include "pprimitive.h"
#include "pcolider.h"
#include "pmath.h"

class PPhysicObject;

static int id = 0;

struct PColision{
    PColision( const std::array<float,2> & point, float direction_angle, std::shared_ptr<PPhysicObject> colide_with, const std::array<float,2> & normals) :
        point(point), direction_angle(direction_angle), colide_with(colide_with), normals(normals), handled(false){
        name = "colision_" + std::to_string(id++);
    }
    std::array<float,2> point;
    float direction_angle;
    std::shared_ptr<PPhysicObject> colide_with;
    std::array<float,2> normals;
    bool handled;
    std::string name;

};

class PPhysicObject {
    public:
        PPhysicObject(std::shared_ptr<PDrawable> drawable) :
            _drawable(drawable), _gravity(0),_velocity{0,0},_velocity_direction(0),_restition(1),_acceleration(0),_colision(nullptr){
                _colide = false;

        }
        
        PPhysicObject(std::shared_ptr<PDrawable> drawable, std::shared_ptr<PColider> colider) :
            _drawable(drawable), _gravity(0.0),_velocity{0,0},_velocity_direction(0),_restition(1),_colider(colider),_colision(nullptr){
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

        auto collision_detected()const{
            return _colide;
        }

        auto colide(std::shared_ptr<PPhysicObject> other) -> bool {
            if(_colider){
               _colide =  _colider->colide(_drawable,other->_drawable);
               if(_colide){
                   if(_colision && _colision->colide_with == other && _colision->handled){
                        float direction_this = velocity_direction();
                        if(angle_to_direction(_colision->direction_angle) != angle_to_direction(direction_this)){
                            _colide = false;
                            return true;
                        }
                   }
                   if(_colision){
                       std::cout<<"Colision with "<<_colision->name<<"\n";
                       _colision->handled = false;
                       return true;
                   }
                   const auto normals = _colider->normals();
                   auto point = std::array<float,2>{_drawable->x(),_drawable->y()};
                   _colision = std::make_shared<PColision>(point,velocity_direction(),other,normals);
                   return true;
               }
            }
            _colide = false;
            _colision = nullptr;
            return false;
        }

        void move(float delta_time){
            if(delta_time == 0){
                return;
            }
            if(NEAR_ZERO(_velocity[0]) && NEAR_ZERO(_velocity[1])){
                return;
            }
            if( _colide  && _colision && !_colision->handled){
                std::cout<<"Handle collision:"<<_colision->name<<"\n";
                auto collision_normal = normalize(_colision->normals);
                float dot_product = _velocity[0] * collision_normal[0] + _velocity[1] * collision_normal[1];
                _velocity[0] -= (1 + _restition) * dot_product * collision_normal[0];
                _velocity[1] -= (1 + _restition) * dot_product * collision_normal[1];
                _colision->handled = true;
                _drawable->add(std::array<float,2>{_colision->point[0] - _drawable->x(),_colision->point[1] - _drawable->y()-1});
            }else{
                _velocity[1] += _gravity * delta_time;
                _velocity[0] += _acceleration * delta_time;
            }
            
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


