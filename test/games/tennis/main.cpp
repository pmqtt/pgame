#include "../../../physic/pcollider.h"
#include "../../../core/pgame.h"

#define WIDTH 1200
#define HEIGHT 600

struct PressEscape : public PKeyDownListener {
	void on_event(PEventLoop* loop, SDL_Event event) override {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			loop->quit();
		}
	}
};

struct PressTop : public PKeyDownListener {
    void on_event(PEventLoop* loop, SDL_Event event) {
        if (event.key.keysym.sym == SDLK_UP) {
            auto velocity = loop->physics_objects()["human_player"]->velocity();
            loop->physics_objects()["human_player"]->velocity(0.0, velocity.y() - 20.0);
            loop->physics_objects()["computer_player"]->velocity(0.0, velocity.y() - 20.0);
        }
    }
};

struct PressBottom : public PKeyDownListener {
    void on_event(PEventLoop* loop, SDL_Event event) {
        if (event.key.keysym.sym == SDLK_DOWN) {
            auto velocity = loop->physics_objects()["human_player"]->velocity();
            loop->physics_objects()["human_player"]->velocity(0.0, velocity.y() + 20.0);
            loop->physics_objects()["computer_player"]->velocity(0.0, velocity.y() + 20.0);
        }
    }
};

struct BallWallCollisionListener : public PCollisionListener{
    void on_event(PEventLoop* loop,const PCollisionItem & item, std::shared_ptr<PPhysicObject> object1, std::shared_ptr<PPhysicObject> object2) override {
        P_UNUSED(loop);
        P_UNUSED(object1);
        P_UNUSED(object2);
        PCollisionItem ball_left = { "ball", "field_left" };
        PCollisionItem ball_right = { "ball", "field_right" };
        if (item == ball_left) {
            std::cout<<"YOU LOOSE"<<std::endl;
            exit(0);
        }
        if (item == ball_right) {
            std::cout<<"YOU WIN"<<std::endl;
            exit(0);
        }
    }
};

auto create_ball(float x, float y) -> std::shared_ptr<PPhysicObject> {
    auto drawable = std::make_shared<PRect>(x, y, 10, 10, true);
    auto object = std::make_shared<PPhysicObject>(drawable, std::make_shared<PSatCollider>());
    object->velocity(-80.0, 15.0);
    object->acceleration(0.0);
    object->restitution(1.0);
    object->mass(1.0);
    object->is_static(false);
    return object;
}

auto create_player(float x, float y) -> std::shared_ptr<PPhysicObject> {
    auto drawable = std::make_shared<PRect>(x,y, 10, 50, true);
    auto object = std::make_shared<PPhysicObject>(drawable, std::make_shared<PSatCollider>());
    object->velocity(0.0, 0.0);
    object->acceleration(0.0);
    object->restitution(1.0);
    object->mass(0.0);
    object->is_static(false);
    return object;
}

void create_field(PEventLoop& loop) {
    auto top = std::make_shared<PRect>(10, 10, WIDTH-20, 10, true);
    auto left = std::make_shared<PRect>(10, 10, 10, HEIGHT-20, true);
    auto right = std::make_shared<PRect>(WIDTH-20, 10, 10, HEIGHT-20, true);
    auto bottom = std::make_shared<PRect>(10, HEIGHT-20, WIDTH-20, 10, true);

    auto top_object = make_physic_object(top, std::make_shared<PSatCollider>());
    auto left_object = make_physic_object(left, std::make_shared<PSatCollider>());
    auto right_object = make_physic_object(right, std::make_shared<PSatCollider>());
    auto bottom_object = make_physic_object(bottom, std::make_shared<PSatCollider>());

    loop.add_physics_object("field_top", top_object);
    loop.add_physics_object("field_left", left_object);
    loop.add_physics_object("field_right", right_object);
    loop.add_physics_object("field_bottom", bottom_object);

    auto middle_line = std::make_shared<PRect>(WIDTH/2-5, 10, 10, HEIGHT-20, true);
    loop.add_moveable("middle_line", middle_line);
}


auto main(int argc, char** argv) -> int {
	P_UNUSED(argc);
	P_UNUSED(argv);
	PWindow window("PGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT);
	auto event_loop = window.create_event_loop();
    event_loop.add_collision_listener({"field_left","ball"},std::make_shared<BallWallCollisionListener>());
    event_loop.add_collision_listener({"field_right","ball"},std::make_shared<BallWallCollisionListener>());
    event_loop.add_key_down_listener(std::make_shared<PressEscape>());
	event_loop.add_key_down_listener(std::make_shared<PressTop>());
	event_loop.add_key_down_listener(std::make_shared<PressBottom>());
	event_loop.add_physics_object("human_player", create_player(40, HEIGHT/2-50)); 
	event_loop.add_physics_object("computer_player", create_player(WIDTH-50, HEIGHT/2-50));
    event_loop.add_physics_object("ball", create_ball(WIDTH/2-50, HEIGHT/2));
    create_field(event_loop);
    event_loop.run();
	return 0;
}
