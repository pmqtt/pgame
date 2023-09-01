#include "pgame.h"

auto PAnimation::go_to(const std::array<int,2> &point, PEventLoop* loop) -> bool{
    int current_x = _drawable->x();
    int current_y = _drawable->y();

    // Zielposition
    int target_x = point[0];
    int target_y = point[1];

    // Berechnen der Richtung und Distanz
    int dx = target_x - current_x;
    int dy = target_y - current_y;
    float distance = sqrt(dx*dx + dy*dy);

    if(distance < 1) {
        return true; // Ziel erreicht
    }

    // Berechnen der Geschwindigkeit
    float speed = 100;  // Geschwindigkeit pro Tick

    // Berechnen der Bewegung pro Tick
    float move_x = (speed * dx / distance);
    float move_y = (speed * dy / distance);


    // Aktualisierung der Position basierend auf vergangenen Ticks

    _stepFrameX += move_x * loop->delta_time();
    _stepFrameY += move_y * loop->delta_time();
    if(std::abs(_stepFrameX) > 1){
        _drawable->add(std::array<int,2>{ (int)_stepFrameX, 0 });
        _stepFrameX = 0;
    }
    if(std::abs(_stepFrameY) > 1){
        _drawable->add(std::array<int,2>{ 0, (int)_stepFrameY });
        _stepFrameY = 0;
    }

    return false;
}
