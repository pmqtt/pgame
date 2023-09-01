#ifndef PGAME_H
#define PGAME_H
#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <list>
#include <array>
#include <map>
#include <vector>
#include <cmath>
#include <iostream>

struct SDLWindowDeleter{
    void operator()(SDL_Window* window){
        if(window != nullptr){
            SDL_DestroyWindow(window);
        }
    }
};

struct SDLRendererDeleter{
    void operator()(SDL_Renderer* renderer){
        if(renderer != nullptr){
            SDL_DestroyRenderer(renderer);
        }
    }
};

class PWindow;
class PEventLoop;


struct PTimer{
        PTimer():
            _startTicks(0){
        }

        void start(){
            _startTicks = SDL_GetTicks();
        }

        auto delta_ticks()->double{
            auto currentTicks = SDL_GetTicks();
            auto delta = (currentTicks - _startTicks) / 1000.0;
            _startTicks = currentTicks;
            return delta;
        }

    private:
        int _startTicks;

};

struct PEventListener{
    virtual void on_event(PEventLoop* loop,SDL_Event event) = 0;
};

struct PKeyDownListener{
    virtual void on_event(PEventLoop* loop, SDL_Event event) = 0;
};

struct PDrawable{
    public:
        PDrawable(float x,float y) : _color({255,0,0,255}),_x(x),_y(y) {}
            
        virtual void draw(std::shared_ptr<SDL_Renderer> renderer) = 0;
        virtual void add(const std::array<float,2> &v) { }

        void change_color(const std::array<unsigned char,4> &color){
            _color = color;
        }

        auto x() const -> float{
            return _x;
        }

        auto y() const -> float{
            return _y;
        }

    protected:
        std::array<unsigned char,4> _color;
        float _x;
        float _y;

};
struct PRect : public PDrawable{
    public:
        PRect(float x, float y, float w, float h)
            : PDrawable(x,y), w(w), h(h){
        }
        
        void draw(std::shared_ptr<SDL_Renderer> renderer){
            SDL_FRect rect;
            rect.x = _x;
            rect.y = _y;
            rect.w = w;
            rect.h = h;
            SDL_SetRenderDrawColor(renderer.get(), _color[0], _color[1], _color[2], _color[3]);
            SDL_RenderFillRectF(renderer.get(), &rect);
        }
        
        void add(const std::array<float,2> &v){
            _x += v[0];
            _y += v[1];
        }
    private:
        float  w, h;
};


struct PCircle : public PDrawable{
    public:
        PCircle(float x, float y, float r, bool fill = false)
            : PDrawable(x,y), r(r), _fill(fill){
        }

        void draw(std::shared_ptr<SDL_Renderer> renderer){
            if(_fill){
                draw_fill(renderer);
            }else{
                draw_empty(renderer);
            }
        }
    
        void fill(bool fill){
            _fill = fill;
        }

        auto fill() const -> bool{
            return _fill;
        }

        void draw_empty(std::shared_ptr<SDL_Renderer> renderer){
            const int points = 100;  // Anzahl der Punkte für die Kreiszeichnung
            float angle = 0.0f;
            float angleStep = 2.0f * M_PI / points;
            for (int i = 0; i < points; ++i){
                float x1 = _x + r * cos(angle);
                float y1 = _y + r * sin(angle);
                angle += angleStep;
                float x2 = _x + r * cos(angle);
                float y2 = _y + r * sin(angle);
                SDL_RenderDrawLineF(renderer.get(), x1, y1, x2, y2);
            }
        }


        void draw_fill(std::shared_ptr<SDL_Renderer> renderer){
            for (float y = _y - r; y <= _y + r; y+=0.1){
                for (float x = _x - r; x <= _x + r; x +=0.1){
                    float dx = x - _x;
                    float dy = y - _y;
                    if (dx * dx + dy * dy <= r * r){
                        SDL_RenderDrawPointF(renderer.get(), x, y);
                    }
                }
            }
        }



        void add(const std::array<float,2> &v){
            _x += v[0];
            _y += v[1];
        }

    private:
        float r;
        bool _fill;
};


struct PPath{
    public:
        PPath(): _loop(false) {}
        
        void add(const std::array<float,2> &v){
            _points.push_back(v);
        }

        void loop(bool loop){
            this->_loop = loop;
        }

        auto loop() const -> bool{
            return _loop;
        }

        auto points() const -> const std::vector<std::array<float,2>>&{
            return _points;
        }

    private:
        bool _loop;
        std::vector<std::array<float,2>> _points;
};

struct PAnimation{
    public:
        PAnimation(std::shared_ptr<PDrawable> drawable)
            : _drawable(drawable), _path_index(-1){
        }

        PAnimation(std::shared_ptr<PDrawable> drawable, std::shared_ptr<PPath> path)
            : _drawable(drawable), _path(path), _path_index(0){
            
        }

        void draw(PEventLoop * loop,std::shared_ptr<SDL_Renderer> renderer){
            if(_path){
                if(_path_index < _path->points().size()){
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

        virtual void animate(PEventLoop* loop) { }
        virtual void animate(std::array<float,2> point,PEventLoop* loop) { }
        virtual void animate_on_path(PEventLoop* loop) { }

    private:
        auto go_to(const std::array<float,2> &point, PEventLoop* loop) -> bool;
    protected:
        std::shared_ptr<PDrawable> _drawable;
        std::shared_ptr<PPath> _path;
        int _path_index;
};


class PEventLoop{
    public:
        PEventLoop(std::shared_ptr<SDL_Window> _window, 
                  std::shared_ptr<SDL_Renderer> _renderer)
            : _window(_window), _renderer(_renderer){
        }

        void push(SDL_Event event){
            _events.push_back(event);
        }

        void add_key_down_listener(std::shared_ptr<PKeyDownListener> listener){
            _key_down_listeners.push_back(listener);
        }

        void stop(){
            _quit = true;
        }
        
        auto moveables()const -> std::map<std::string, std::shared_ptr<PDrawable>>{
            return _moveables;
        }

        void add_moveable(const std::string & name, std::shared_ptr<PDrawable> moveable){
            _moveables[name] = moveable;
        }

        void add_animation(const std::string & name, std::shared_ptr<PAnimation> animation){
            _animations[name] = animation;
        }


        auto animation_timer() -> PTimer&{
            return _animation_timer;
        }

        auto delta_time()->double{
            return _delta_time;
        }

        void run(){
            SDL_Event event;
            _quit = false;
            _animation_timer.start();
            while(!_quit){
                _delta_time = _animation_timer.delta_ticks();
                if(_delta_time < (1000.0/60.0)){
                    SDL_Delay((1000.0/60.0) - _delta_time);
                }

                while(SDL_PollEvent(&event)){
                        switch(event.type){
                        case SDL_QUIT:
                            _quit = true;
                            break;
                        case SDL_KEYDOWN:
                            for(auto listener : _key_down_listeners){
                                listener->on_event(this, event);
                            }
                            break;
                        default:
                            break;
                    }
                }
                SDL_SetRenderDrawColor(_renderer.get(), 0, 0, 0, 255);
                SDL_RenderClear(_renderer.get());
                for(auto moveable : _moveables){
                    moveable.second->draw(_renderer);
                }
                for(auto animation : _animations){
                    animation.second->draw(this, _renderer);
                }
                SDL_RenderPresent(_renderer.get());
                
            }
        }


    private:
        bool _quit;
        std::list<SDL_Event> _events;
        std::list<std::shared_ptr<PKeyDownListener>> _key_down_listeners;
        std::shared_ptr<SDL_Window> _window;
        std::shared_ptr<SDL_Renderer> _renderer;
        std::map<std::string, std::shared_ptr<PDrawable>> _moveables;
        std::map<std::string, std::shared_ptr<PAnimation>> _animations;
        PTimer _animation_timer;
        double _delta_time;
};

class PWindow{
    public:
        PWindow(const std::string & title,
                int x = SDL_WINDOWPOS_CENTERED, 
                int y = SDL_WINDOWPOS_CENTERED, 
                int w = 800, int h = 600, 
                unsigned int flags = SDL_WINDOW_SHOWN){
            SDL_Init(SDL_INIT_EVERYTHING);
            _window = std::shared_ptr<SDL_Window>(
                                                SDL_CreateWindow(title.c_str(), x, y, w, h, flags),
                                                SDLWindowDeleter());
            _renderer = std::shared_ptr<SDL_Renderer>(
                                                SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED),
                                                SDLRendererDeleter());
        }

        ~PWindow(){
            _window.reset();
            _renderer.reset();
            SDL_Quit();
        }

        PEventLoop create_event_loop(){
            return PEventLoop(_window, _renderer);
        }

    private:
        std::string _title;
        int _x, _y, _w, _h;
        unsigned int _flags;
        std::shared_ptr<SDL_Window> _window;
        std::shared_ptr<SDL_Renderer> _renderer;
};


#endif
