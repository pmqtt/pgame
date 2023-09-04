#include "panimation.h"
#include "peventloop.h"
#include <iostream>

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
    const float current_x = _drawable->x();
    const float current_y = _drawable->y();

    // Zielposition
    const float target_x = point[0];
    const float target_y = point[1];

    // Berechnen der Richtung und Distanz
    const float dx = target_x - current_x;
    const float dy = target_y - current_y;
    const float distance = sqrt(dx*dx + dy*dy);

    if(distance < 1) {
        return true; // Ziel erreicht
    }

    const float speed = 40; // Pixel pro Sekunde

    // Berechnen der Bewegung pro Tick

    const float move_x = (speed * dx / distance);
    const float move_y = (speed * dy / distance);

    const float new_x = current_x + move_x * loop->delta_time();
    const float new_y = current_y + move_y * loop->delta_time();
    
    const float future_distance = sqrt((new_x - target_x)*(new_x - target_x) + (new_y - target_y)*(new_y - target_y));

    // Überprüfung, ob das Ziel überschritten wird
    if (future_distance > distance) {
        // Bewegung auf die verbleibende Distanz beschränken
        float remaining_distance = distance;
        float ratio = remaining_distance / distance;
        _drawable->add({ move_x * ratio * (float)loop->delta_time(), move_y * ratio * (float)loop->delta_time()});
        return true; // Ziel erreicht
    }

    _drawable->add(std::array<float,2>{  move_x * (float)loop->delta_time(), move_y * (float)loop->delta_time() });

    return false;
}
