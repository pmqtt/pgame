#ifndef PSTACKLAYOUT_H
#define PSTACKLAYOUT_H
#include "../core/pdebug.h"
#include "pguielement.h"
#include <vector>
enum class PStackLayoutDirection {
    HORIZONTAL,
    VERTICAL
};

class PStackLayout : public PGuiElement{
public:
    PStackLayout(PStackLayoutDirection direction,int x,int y): _direction(direction),_x(x),_y(y){
        _style.spacing = 0;
        _style.x = x;
        _style.y = y;
    }

    void add(std::shared_ptr<PGuiElement> element){
        _elements.push_back(element);
        update_layout();
    }

    void draw(std::shared_ptr<SDL_Renderer> renderer) override{
        for(auto & element : _elements){
            element->draw(renderer);
        }
    }

    auto is_clicked(int x,int y)const -> bool override{
        for(std::size_t i = 0; i < _elements.size();++i){
            if(_elements[i]->is_clicked(x,y)){
                _last_index = i;
                return true;
            }
        }
        _last_index = -1;
        return false;
    }

    void on_click() override{
        if(_last_index != -1){
            _elements[_last_index]->on_click();
        }
    }

    void hover(bool hover) override{
        for(auto & element : _elements){
            element->hover(false);
        }
        if(_last_index != -1){
            _elements[_last_index]->hover(hover);
        }
        
    }

    void style(const PStyle & style) override{
        _style = style;
        update_layout();
    }

    auto style()const -> PStyle override{
        return _style;
    }

    void spacing(int spacing){
        _style.spacing = spacing;
        update_layout();
    }

    void listener(std::function<void()> listener) override{
        P_UNUSED(listener);
    }
    
    auto element() -> std::shared_ptr<PGuiElement> override{
        if(_last_index != -1){
            return _elements[_last_index]->element();
        }
        return std::shared_ptr<PGuiElement>(this);
    }

    auto fixed_content()const -> bool override{
        return true;
    }

private:

    void update_layout(){
        int current_x = *_style.x;
        int current_y = *_style.y;
        int max_width = 0;
        int max_height = 0;
        for (auto& element : _elements) {
            PStyle style = element->style();
            style.x = current_x;
            style.y = current_y;
            element->style(style);

            if (_direction == PStackLayoutDirection::VERTICAL) {
                current_y += style.height.value_or(0) + _style.spacing.value_or(0);
            } else {
                current_x += style.width.value_or(0) + _style.spacing.value_or(0);
            }
            max_width = std::max(max_width, style.width.value_or(0));
            max_height = std::max(max_height, style.height.value_or(0));
        }
        if (_direction == PStackLayoutDirection::VERTICAL) {
            *_style.height = current_y - *_style.y;
            *_style.width = max_width;
        } else {
            *_style.width = current_x - *_style.x;
            *_style.height = max_height;
        }
    }

    PStackLayoutDirection _direction;
    int _x;
    int _y;
    std::vector<std::shared_ptr<PGuiElement>> _elements;
    mutable int _last_index = -1;
    PStyle _style;

};


#endif // PSTACKLAYOUT_H
