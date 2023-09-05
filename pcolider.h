#ifndef PCOLIDER_H
#define PCOLIDER_H
#include "pprimitive.h"
#include <optional>


struct PColider{
    virtual auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool = 0;
    virtual auto normals()const -> std::array<float,2>{
        return {{0,0}};
    }
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


using PVertices2D = std::array<std::array<float,2>,4>;

struct PSatColider : PColider{
    
    auto colide(std::shared_ptr<PDrawable> p1, std::shared_ptr<PDrawable> p2) const -> bool override{
        const float angle_p1_rad = p1->angle() * M_PI / 180;
        const float angle_p2_rad = p2->angle() * M_PI / 180;
        const PVertices2D axes = {{
            {{ cos(angle_p1_rad), sin(angle_p1_rad)  }},
            {{ (float)-sin(angle_p1_rad), (float)cos(angle_p1_rad) }} ,
            {{ cos(angle_p2_rad), sin(angle_p2_rad)  }},
            {{ -sin(angle_p2_rad), cos(angle_p2_rad) }} 
        }};

        const auto A = p1->bounding_box();
        const auto B = p2->bounding_box();
        const float p1_h = A[3];
        const float p1_w = A[2];
        const float p1_w2 = p1_w / 2;
        const float p1_h2 = p1_h / 2;
        const float p1_center_xx = A[0] + p1_w2;
        const float p1_center_yy = A[1] + p1_h2;
        const auto p1_center = rotate_middpoint(A,p1_center_xx,p1_center_yy,angle_p1_rad);
        const float p1_center_x = p1_center[0];
        const float p1_center_y = p1_center[1];

        const float p2_h = B[3];
        const float p2_w = B[2];
        const float p2_w2 = p2_w / 2;
        const float p2_h2 = p2_h / 2;
        const float p2_center_xx = B[0] + p2_w2;
        const float p2_center_yy = B[1] + p2_h2;
        const auto p2_center = rotate_middpoint(B,p2_center_xx,p2_center_yy,angle_p2_rad);
        const float p2_center_x = p2_center[0];
        const float p2_center_y = p2_center[1];

        const PVertices2D verticesA = create_vertices(p1_center_x,p1_center_y,p1_w2,p1_h2,angle_p1_rad);
        const PVertices2D verticesB = create_vertices(p2_center_x,p2_center_y,p2_w2,p2_h2,angle_p2_rad);

        float minOverlap = INFINITY;
        for (auto axis : axes) {
            auto res = is_separating_axis(axis, verticesA, verticesB);
            if (!res){
                return false;
            }else{
                const float overlap = *res;
                if (overlap < minOverlap) {
                    minOverlap = overlap;
                    _collision_normal = axis;  // Speichern Sie die Achse mit der minimalen Überlappung
                }
            }
        }
        return true;
    }

    auto rotate_middpoint(const std::array<float,4> &box,float x, float y, float rad) const -> std::array<float,2>{
        const float xc =  box[0];
        const float yc =  box[1];
        const float xr = (x - xc) * cos(rad) - (y - yc) * sin(rad) + xc;
        const float yr = (x - xc) * sin(rad) + (y - yc) * cos(rad) + yc;
        return {{xr,yr}};
    }

    PVertices2D create_vertices(float p1_center_x,float p1_center_y,float p1_w2,float p1_h2,float angle_p1_rad)const{
        return {{
            {{p1_center_x + cos(angle_p1_rad) * p1_w2 - sin(angle_p1_rad) * p1_h2, p1_center_y + sin(angle_p1_rad) * p1_w2 + cos(angle_p1_rad) * p1_h2}},
            {{p1_center_x - cos(angle_p1_rad) * p1_w2 - sin(angle_p1_rad) * p1_h2, p1_center_y - sin(angle_p1_rad) * p1_w2 + cos(angle_p1_rad) * p1_h2}},
            {{p1_center_x - cos(angle_p1_rad) * p1_w2 + sin(angle_p1_rad) * p1_h2, p1_center_y - sin(angle_p1_rad) * p1_w2 - cos(angle_p1_rad) * p1_h2}},
            {{p1_center_x + cos(angle_p1_rad) * p1_w2 + sin(angle_p1_rad) * p1_h2, p1_center_y + sin(angle_p1_rad) * p1_w2 - cos(angle_p1_rad) * p1_h2}}
        }};
    }

    auto is_separating_axis(const std::array<float,2> & axis, 
                            const PVertices2D & verticesA, const PVertices2D & verticesB)const -> std::optional<float>{
        float minA = INFINITY, maxA = -INFINITY;
        float minB = INFINITY, maxB = -INFINITY;

        for(const auto & vertex : verticesA){
            const auto projection = axis[0] * vertex[0] + axis[1] * vertex[1];
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }

        for(const auto & vertex : verticesB){
            const auto projection = axis[0] * vertex[0] + axis[1] * vertex[1];
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }

        if (maxA < minB || maxB < minA) {
            return {};
        }
        return std::min(maxA, maxB) - std::max(minA, minB);
    }

    auto normals()const -> std::array<float, 2> override{
        return _collision_normal;
    }


private:
    mutable std::array<float, 2> _collision_normal = {{0.0f, 0.0f}};
};


#endif
