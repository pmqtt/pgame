#include "panimation.h"
#include "peventloop.h"

void PAnimation::draw(PEventLoop * loop,std::shared_ptr<SDL_Renderer> renderer){
    if(_path){
        if(static_cast<std::size_t>(_path_index) < _path->points().size()){
            bool point_arrived = go_to(_path->points().at(_path_index), loop);
            if(point_arrived){
                animate(_path->points().at(_path_index), loop);    
                _path_index++;
            }else{
                animate_on_path(loop);
            }
        }else{
            if(_path->loop()){
                _path_index = 0;
            }
        }
    }else{
        animate(loop);
    }
    _drawable->draw(renderer);
}

auto PAnimation::go_to(const std::array<float,2> &point, PEventLoop* loop) -> bool{
    float current_x = _drawable->x();
    float current_y = _drawable->y();

    // Zielposition
    float target_x = point[0];
    float target_y = point[1];

    // Berechnen der Richtung und Distanz
    int dx = target_x - current_x;
    int dy = target_y - current_y;
    float distance = sqrt(dx*dx + dy*dy);

    if(distance < 0.001) {
        return true; // Ziel erreicht
    }

    // Berechnen der Geschwindigkeit
    float speed = 100;  // Geschwindigkeit pro Tick

    // Berechnen der Bewegung pro Tick
    float move_x = (speed * dx / distance);
    float move_y = (speed * dy / distance);


    // Aktualisierung der Position basierend auf vergangenen Ticks

    _drawable->add(std::array<float,2>{  move_x * (float)loop->delta_time(), move_y * (float)loop->delta_time() });

    return false;
}
