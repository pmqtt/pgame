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
                   const auto normals = _colider->normals();
                   const auto dot_product = _velocity[0] * normals[0] + _velocity[1] * normals[1]; 
                   P_DIRECTION vel_direction = angle_to_direction( velocity_direction() );
                   P_DIRECTION normal_direction = angle_to_direction( atan2(normals[1],normals[0] ));

                   if( dot_product < 0 ){
                        if( vel_direction == P_DIRECTION::P_DOWN && normal_direction == P_DIRECTION::P_UP ){
                            _colide = false;
                            return false;
                        }
                   }

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
            if( _colide  ){
                delta_time += 0.05;
                auto collision_normal = normalize(_colision->normals);
                const float dot_product = _velocity[0] * collision_normal[0] + _velocity[1] * collision_normal[1];
                _velocity[0] -= (1 + _restition) * dot_product * collision_normal[0];
                _velocity[1] -= (1 + _restition) * dot_product * collision_normal[1];
            }else{
                _velocity[1] += _gravity * delta_time;
                _velocity[0] += _acceleration * delta_time;
            }
            
            if( NEAR_ZERO(_velocity[0]) ){
                _velocity[0] = 0;
                if( _colide ){
                    delta_time -= 0.05;
                }
            }

            float x = _velocity[0] * delta_time;
            float y = _velocity[1] * delta_time;
            if(abs(_velocity[1]) < 0.001){
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


