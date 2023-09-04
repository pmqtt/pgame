#ifndef PCOLIDER_H
#define PCOLIDER_H
#include "pprimitive.h"


struct PColider{
    virtual auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool = 0;
};


struct PBoxColider : public PColider{
    auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2 ) const -> bool {
        auto box1 = p1->bounding_box(); // array of x,y,w,h
        auto box2 = p2->bounding_box();
        float leftA, leftB;
        float rightA, rightB;
        float topA, topB;
        float bottomA, bottomB;

        // Berechne die Seiten des Rechtecks A
        leftA = box1[0];
        rightA = leftA + box1[2];
        topA = box1[1];
        bottomA = topA + box1[3];

        // Berechne die Seiten des Rechtecks B
        leftB = box2[0];
        rightB = leftB + box2[2];
        topB = box2[1];
        bottomB = topB + box2[3];


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
        auto box1 = p1->bounding_box();
        auto box2 = p2->bounding_box();
        auto x1 = box1[0] + box1[2]/2;
        auto y1 = box1[1] + box1[3]/2;
        auto x2 = box2[0] + box2[2]/2;
        auto y2 = box2[1] + box2[3]/2;
        auto r1 = box1[2]/2;
        auto r2 = box2[2]/2;
        auto dx = x1 - x2;
        auto dy = y1 - y2;
        auto d = sqrt(dx*dx + dy*dy);
        return d < r1 + r2;
    }
};


#endif
