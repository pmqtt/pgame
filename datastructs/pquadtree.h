#ifndef PQUDATREE_H
#define PQUDATREE_H
#include <vector>
#include <memory>
#include <algorithm>

template<typename T>
class PQuadtree {
public:
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
    std::vector<std::unique_ptr<PQuadtree>> _nodes;

    void split() {
        double sub_width = _width / 2.0;
        double sub_height = _height / 2.0;
        double x = _x;
        double y = _y;

        _nodes.push_back(std::make_unique<PQuadtree>(x + sub_width, y, sub_width, sub_height, _max_objects, _max_levels, _level + 1));
        _nodes.push_back(std::make_unique<PQuadtree>(x, y, sub_width, sub_height, _max_objects, _max_levels, _level + 1));
        _nodes.push_back(std::make_unique<PQuadtree>(x, y + sub_height, sub_width, sub_height, _max_objects, _max_levels, _level + 1));
        _nodes.push_back(std::make_unique<PQuadtree>(x + sub_width, y + sub_height, sub_width, sub_height, _max_objects, _max_levels, _level + 1));
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

#endif // PQUDATREE_H