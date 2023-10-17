#include "pcollider.h"

auto PBoxCollider::collide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool {
    const auto box1 = p1->bounding_box();  // array of x,y,_w,h
    const auto box2 = p2->bounding_box();

    // Berechne die Seiten des Rechtecks A
    const float leftA = box1[0][0];	  // x
    const float rightA = box1[1][0];  //
    const float topA = box1[0][1];	  // y
    const float bottomA = box1[2][1];

    // Berechne die Seiten des Rechtecks B
    const float leftB = box2[0][0];	   // x
    const float rightB = box2[0][1];   // x+_w
    const float topB = box2[0][1];	   // y
    const float bottomB = box2[2][1];  // y+h

    // Überprüfe die Konditionen
    if (bottomA <= topB) {
        return false;
    }

    if (topA >= bottomB) {
        return false;
    }

    if (rightA <= leftB) {
        return false;
    }

    if (leftA >= rightB) {
        return false;
    }

    return true;
}


auto PCircleCollider::collide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool  {
    P_UNUSED(p1);
    P_UNUSED(p2);
    /*const auto box1 = p1->bounding_box();
    const auto box2 = p2->bounding_box();
    const auto x1 = box1[0] + box1[2]/2;
    const auto y1 = box1[1] + box1[3]/2;
    const auto x2 = box2[0] + box2[2]/2;
    const auto y2 = box2[1] + box2[3]/2;
    const auto r1 = box1[2]/2;
    const auto r2 = box2[2]/2;
    const auto dx = x1 - x2;
    const auto dy = y1 - y2;
    const auto d = sqrt(dx*dx + dy*dy);
    return d < r1 + r2;*/
    return false;
}

auto PSatCollider::collide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool  {
    if(false){
        std::cout<<"P1: "<<p1->bounding_box()[0]
                    <<","<<p1->bounding_box()[1]
                    <<","<<p1->bounding_box()[2]
                    <<","<<p1->bounding_box()[3]<<"\n";
        std::cout<<"P2: "<<p2->bounding_box()[0]
                    <<","<<p2->bounding_box()[1]
                    <<","<<p2->bounding_box()[2]
                    <<","<<p2->bounding_box()[3]<<std::endl;
    }
    PVector2D smallestOverlapAxis1 = {0, 0};
    float smallestOverlap1 = std::numeric_limits<float>::max();
    PVector2D smallestOverlapAxis2 = {0, 0};
    float smallestOverlap2 = std::numeric_limits<float>::max();

    // Check axes of shape1
    auto shape1 = p1->bounding_box();
    auto shape2 = p2->bounding_box();
    for (size_t i = 0; i < shape1.size(); ++i) {
        PVector2D point1 = shape1[i];
        PVector2D point2 = shape1[(i + 1) % shape1.size()];
        PVector2D edge = point2 - point1;
        PVector2D axis = perpendicular(edge);
        axis.normalize();
        float overlap = overlapAmount(axis, shape1, shape2);
        if (overlap < 0) {
            return false;
        }

        if (overlap < smallestOverlap1) {
            smallestOverlap1 = overlap;
            smallestOverlapAxis1 = axis;
            _edge_point1 = point1;
            _edge_point2 = point2;
        }
    }
    for (size_t i = 0; i < shape2.size(); ++i) {
        PVector2D point1 = shape2[i];
        PVector2D point2 = shape2[(i + 1) % shape2.size()];
        PVector2D edge = point2 - point1;
        PVector2D axis = perpendicular(edge);
        axis.normalize();
        float overlap = overlapAmount(axis, shape1, shape2);
        if (overlap < 0) {
            return false;
        }
        if (overlap < smallestOverlap2) {
            smallestOverlap2 = overlap;
            smallestOverlapAxis2 = axis;
            _edge_point3 = point1;
            _edge_point4 = point2;
        }
    }
    
    if (smallestOverlap1 < smallestOverlap2) {
        _collision_normal = smallestOverlapAxis1;
        _collision_normal2 = smallestOverlapAxis2;
        _mtv = {smallestOverlap1 * smallestOverlapAxis1[0], smallestOverlap1 * smallestOverlapAxis1[1]};
    } else {
        _collision_normal = smallestOverlapAxis2;
        _collision_normal2 = smallestOverlapAxis1;
        _mtv = {smallestOverlap2 * smallestOverlapAxis2[0], smallestOverlap2 * smallestOverlapAxis2[1]};
    }

    // Überprüfung und Anpassung der Kollisionsnormale, falls notwendig
    PVector2D directionFromP1toP2 = p2->center() - p1->center();  // angenommen, dass PDrawable eine center() Methode hat
    if (dot(_collision_normal, directionFromP1toP2) < 0) {
        _collision_normal = _collision_normal*-1;  // Umkehren der Kollisionsnormale
        _collision_normal2 = _collision_normal2*-1;
        _mtv = _mtv *-1;
    }

    return true;
}

auto PSatCollider::overlapAmount(const PVector2D &axis, const PVertices2D &shape1, const PVertices2D &shape2) const -> float {
	float min1 = std::numeric_limits<float>::max();
    float max1 = std::numeric_limits<float>::min();
    for (const auto &point : shape1) {
        float projection = dot(axis, point);
        min1 = std::min(min1, projection);
        max1 = std::max(max1, projection);
    }

    float min2 = std::numeric_limits<float>::max();
    float max2 = std::numeric_limits<float>::min();
    for (const auto &point : shape2) {
        float projection = dot(axis, point);
        min2 = std::min(min2, projection);
        max2 = std::max(max2, projection);
    }
    return std::min(max1, max2) - std::max(min1, min2);
}
