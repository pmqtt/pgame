#ifndef PQUDATREE_H
#define PQUDATREE_H
#include <vector>
#include <memory>
#include <algorithm>
#include "../core/pprinter.h"
#if 1
template<class T>
class PQuadtree{
    private:
        static const unsigned char NW = 0;
        static const unsigned char NE = 1;
        static const unsigned char SW = 2;
        static const unsigned char SE = 3;
        double _x;
        double _y;
        double _width;
        double _height;
        std::size_t _max_objects;
        std::vector<T> _objects;
        std::array<std::shared_ptr<PQuadtree>,4> _nodes;
        bool _splitted;
        PPrinter _printer;
    private:
        
        auto contains(const T& item) const -> bool {
            auto bb = item->drawable()->bounding_box();
            bool result = !((bb[3][0] < _x || bb[2][0] > _x + _width) ||
                    (bb[3][1] < _y || bb[1][1] > _y + _height));
#if 0
            std::cout << "Checking if object " << item->name() << " at ({" << bb[0][0] << ", " << bb[0][1] << "},{ " 
                                                                          << bb[1][0] << ", " << bb[1][1] << "},{ "
                                                                          << bb[2][0] << ", " << bb[2][1] << "},{ " 
                                                                          << bb[3][0] << ", " << bb[3][1] << "} " <<
                                                                 ") is contained in quadtree region (" << _x << ", " << _y << ", " << _width << ", " << _height << "): " << result << "\n";
#endif
            return result;
        }

        auto intersects(const T& item) const -> bool {
            auto bb = item->drawable()->bounding_box();

            // Check if any corner of the item's bounding box is inside the region
            for (int i = 0; i < 4; i++) {
                if (bb[i][0] >= _x && bb[i][0] <= _x + _width &&
                    bb[i][1] >= _y && bb[i][1] <= _y + _height) {
                    return true;
                }
            }

            // Check if any corner of the region is inside the item's bounding box
            std::array<std::array<double, 2>, 4> region_corners = {
                std::array<double, 2>{_x, _y},
                std::array<double, 2>{_x + _width, _y},
                std::array<double, 2>{_x + _width, _y + _height},
                std::array<double, 2>{_x, _y + _height}
            };
            for (const auto& corner : region_corners) {
                if (corner[0] >= bb[0][0] && corner[0] <= bb[2][0] &&
                    corner[1] >= bb[0][1] && corner[1] <= bb[2][1]) {
                    return true;
                }
            }

            return false;
        }
        
        void split(){
            const float x = _x;
            const float y = _y;
            const float w = _width;
            const float h = _height;

            _nodes[NW] = std::make_shared<PQuadtree>(x           , y           , w / 2.0f, h / 2.0f);
            _nodes[NE] = std::make_shared<PQuadtree>(x + w / 2.0f, y           , w / 2.0f, h / 2.0f);
            _nodes[SW] = std::make_shared<PQuadtree>(x           , y + h / 2.0f, w / 2.0f, h / 2.0f);
            _nodes[SE] = std::make_shared<PQuadtree>(x + w / 2.0f, y + h / 2.0f, w / 2.0f, h / 2.0f);
        }

        void insert_into_sub_nodes(T object) {
             bool inserted = false;
    
            if (_nodes[NW]->intersects(object)) {
                _nodes[NW]->insert(object);
                inserted = true;
            }
            if (_nodes[NE]->intersects(object)) {
                _nodes[NE]->insert(object);
                inserted = true;
            }
            if (_nodes[SW]->intersects(object)) {
                _nodes[SW]->insert(object);
                inserted = true;
            }
            if (_nodes[SE]->intersects(object)) {
                _nodes[SE]->insert(object);
                inserted = true;
            }

            if (!inserted) {
                std::cout<<"PANIC \n";
                _objects.push_back(object);
            }
        }

        
    public:
        PQuadtree(float x, float y, float width, float height, std::size_t max_objects = 12)
            : _x(x), _y(y), _width(width), _height(height), _max_objects(max_objects),_splitted(false) {}

        void insert(T object){
            //std::cout << "Inserting object " << object->name() << " at (" << object->drawable()->bounding_box()[0][0] << ", " << object->drawable()->bounding_box()[0][1] << ") in quadtree region (" << _x << ", " << _y << ", " << _width << ", " << _height << ")\n";

            if( !_splitted){
                if (_objects.size() < _max_objects || (_width <= 1 && _height <= 1)) {
                    _objects.push_back(object);
                    return;
                }
            }

            if (!_nodes[NW]){
                split();
                _splitted = true;
                auto temp = _objects;
                _objects.clear();
                for (const auto& item : temp) {
                    insert_into_sub_nodes(item);
                }
            }
            insert_into_sub_nodes(object);
        }

        auto retrieve(T object)const ->std::vector<T>{
            std::vector<T> found;
            if(!intersects(object)){
                return found;
            }

            for (const auto& item : _objects) {
                if (intersects(item)) {
                    found.push_back(item);
                }
            }

            auto nw = _nodes[NW];
            auto ne = _nodes[NE];
            auto sw = _nodes[SW];
            auto se = _nodes[SE];

            if (nw && nw->intersects(object)) {
                auto nw_items = nw->retrieve(object);
                found.insert(found.end(), nw_items.begin(), nw_items.end());
            }
            if (ne && ne->intersects(object)) {
                auto ne_items = ne->retrieve(object);
                found.insert(found.end(), ne_items.begin(), ne_items.end());
            }
            if (sw && sw->intersects(object)) {
                auto sw_items = sw->retrieve(object);
                found.insert(found.end(), sw_items.begin(), sw_items.end());
            }
            if (se && se->intersects(object)) {
                auto se_items = se->retrieve(object);
                found.insert(found.end(), se_items.begin(), se_items.end());
            }
            return found;
        }

        void remove(T object){
            //std::cout << "Removing object " << object->name() << " from quadtree region (" << _x << ", " << _y << ", " << _width << ", " << _height << ")\n";
            _objects.erase(std::remove(_objects.begin(), _objects.end(), object), _objects.end());
            if (_nodes[NW]) {
                _nodes[NW]->remove(object);
                _nodes[NE]->remove(object);
                _nodes[SW]->remove(object);
                _nodes[SE]->remove(object);
            }
        }

        void update(T object) {
            remove(object);
            insert(object);
        }
        void print(){
            _printer.indent();
            for(int i = 0; i < _objects.size(); ++i){
                _printer.print("name:"+_objects[i]->name());
            }
            if(_nodes[NW]){
                _printer.indent();
                _printer.print("NW");
                _nodes[NW]->print();
                _printer.print("NE");
                _nodes[NE]->print();
                _printer.print("SW");
                _nodes[SW]->print();
                _printer.print("SE");
                _nodes[SE]->print();
            }
            _printer.unindent();
        }   
};

#else
template<typename T>
class PQuadtree {
public:
    PQuadtree() = default;
    PQuadtree(double x, double y, double width, double height, int max_objects = 10, int max_levels = 5, int level = 0)
        : _x(x), _y(y), _width(width), _height(height), _max_objects(max_objects), _max_levels(max_levels), _level(level) {}

    void insert(T object) {
        if (_nodes.size() > 0) {
            int index = get_index(object);
            if (index != -1) {
                _nodes[index]->insert(object);
                return;
            }
        }

        _objects.push_back(object);

        if (_objects.size() > _max_objects && _level < _max_levels) {
            if (_nodes.size() == 0) {
                split();
            }

            std::size_t i = 0;
            while (i < _objects.size()) {
                int index = get_index(_objects[i]);
                if (index != -1) {
                    _nodes[index]->insert(_objects[i]);
                    _objects.erase(_objects.begin() + i);
                }
                else {
                    i++;
                }
            }
        }
    }

    void update(T object) {
        // Remove the object from the tree if it already exists
        remove(object);

        // Re-insert the object with its updated position
        insert(object);
    }

    void remove(T object) {
        int index = get_index(object);
        if (index != -1 && _nodes.size() > 0) {
            _nodes[index]->remove(object);
            return;
        }

        auto iter = std::find(_objects.begin(), _objects.end(), object);
        if (iter != _objects.end()) {
            _objects.erase(iter);
        }
    }

    std::vector<T> retrieve(T object) {
        std::vector<T> return_objects = _objects;

        if (_nodes.size() > 0) {
            int index = get_index(object);
            if (index != -1) {
                std::vector<T> child_objects = _nodes[index]->retrieve(object);
                return_objects.insert(return_objects.end(), child_objects.begin(), child_objects.end());
            }
            else {
                for (auto& node : _nodes) {
                    std::vector<T> child_objects = node->retrieve(object);
                    return_objects.insert(return_objects.end(), child_objects.begin(), child_objects.end());
                }
            }
        }

        return return_objects;
    }

    void clear() {
        _objects.clear();

        for (auto& node : _nodes) {
            node->clear();
        }

        _nodes.clear();
    }

private:
    double _x;
    double _y;
    double _width;
    double _height;
    std::size_t _max_objects;
    std::size_t _max_levels;
    std::size_t _level;
    std::vector<T> _objects;
    std::vector<std::shared_ptr<PQuadtree>> _nodes;

    void split() {
        double sub_width = _width / 2.0;
        double sub_height = _height / 2.0;
        double x = _x;
        double y = _y;

        _nodes.push_back(std::make_shared<PQuadtree>(x + sub_width, y, sub_width, sub_height, _max_objects, _max_levels, _level + 1));
        _nodes.push_back(std::make_shared<PQuadtree>(x, y, sub_width, sub_height, _max_objects, _max_levels, _level + 1));
        _nodes.push_back(std::make_shared<PQuadtree>(x, y + sub_height, sub_width, sub_height, _max_objects, _max_levels, _level + 1));
        _nodes.push_back(std::make_shared<PQuadtree>(x + sub_width, y + sub_height, sub_width, sub_height, _max_objects, _max_levels, _level + 1));
    }

    int get_index(T object) {
        double vertical_midpoint = _x + (_width / 2.0);
        double horizontal_midpoint = _y + (_height / 2.0);

        auto box = object->drawable()->bounding_box();
    
        bool top_quadrant = (box[0][1] < horizontal_midpoint && box[2][1] < horizontal_midpoint);
        bool bottom_quadrant = (box[0][1] > horizontal_midpoint);

        if (box[0][0] < vertical_midpoint && box[1][0] < vertical_midpoint) {
            if (top_quadrant) {
                return 1;
            }
            else if (bottom_quadrant) {
                return 2;
            }
        }
        else if (box[0][0] > vertical_midpoint) {
            if (top_quadrant) {
                return 0;
            }
            else if (bottom_quadrant) {
                return 3;
            }
        }
        return -1;
    }
};

#endif

#endif // PQUDATREE_H