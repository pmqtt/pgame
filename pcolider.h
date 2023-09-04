#ifndef PCOLIDER_H
#define PCOLIDER_H
#include "pprimitive.h"


struct PColider{
    virtual auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool = 0;
};


struct PBoxColider : public PColider{
    auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2 ) const -> bool {
        const auto box1 = p1->bounding_box(); // array of x,y,w,h
        const auto box2 = p2->bounding_box();

        // Berechne die Seiten des Rechtecks A
        const float leftA = box1[0];
        const float rightA = leftA + box1[2];
        const float topA = box1[1];
        const float bottomA = topA + box1[3];

        // Berechne die Seiten des Rechtecks B
        const float leftB = box2[0];
        const float rightB = leftB + box2[2];
        const float topB = box2[1];
        const float bottomB = topB + box2[3];


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
};

struct PCircleColider : public PColider{
    auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool override{
        const auto box1 = p1->bounding_box();
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
        return d < r1 + r2;
    }
};


#endif
