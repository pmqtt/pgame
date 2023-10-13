#include "../core/pgame.h"
#include "../datastructs/pquadtree.h"

struct TestObject{
	TestObject(std::shared_ptr<PDrawable> drawable,const std::string & name) : _drawable(drawable),_name(name){}
	auto drawable() -> std::shared_ptr<PDrawable> { return _drawable; }
	auto name() -> std::string { return _name; }
	std::shared_ptr<PDrawable> _drawable;
	std::string _name;
};

auto make_test_object(const std::string &name,float x, float y, float w, float h)->std::shared_ptr<TestObject>{
	auto drawable = std::make_shared<PRect>(x, y, w, h);
	return std::make_shared<TestObject>(drawable,name);
}

auto main(int argc, char** argv) -> int {
    P_UNUSED(argc);
    P_UNUSED(argv);
    
    // Initialisierung wie zuvor
    auto t1 = make_test_object("t1",0, 0, 10, 10);
    auto t2 = make_test_object("t2",900, 0, 10, 10);
    auto t3 = make_test_object("t3",0, 500, 10, 10);
    auto t4 = make_test_object("t4",900, 500, 10, 10);
    auto t5 = make_test_object("t5", 780, 390, 40, 40); // Objekt, das über mehrere Quadranten ragt
    PQuadtree<std::shared_ptr<TestObject>> quadtree(0, 0, 1600, 800);
    quadtree.insert(t1);
    quadtree.insert(t2);
    //quadtree.insert(t3);
    //quadtree.insert(t4);
    quadtree.insert(t5);
	quadtree.print();
    // Test 1: Objekt innerhalb eines Quadranten bewegen
    //t1->drawable()->add({10, 10});
    //quadtree.update(t1);
    auto vec1 = quadtree.retrieve(t1);
    if (vec1.size() != 1 || vec1[0]->name() != "t1") {
		std::cout<<"vec1.size()="<<vec1.size()<<std::endl;
		for(auto & obj : vec1){
			std::cout<<"obj->name()="<<obj->name()<<std::endl;
		}
		return 4;
	}
#if 0
    // Test 2: Objekt über die Grenzen eines Quadranten bewegen
    t2->drawable()->add({-110, 0});
    quadtree.update(t2);
    auto vec2 = quadtree.retrieve(t2);
    if (vec2.size() != 1 || vec2[0]->name() != "t2") return 5;

    // Test 3: Großes Objekt aktualisieren
    t5->drawable()->add({10, 10});
    quadtree.update(t5);
    auto vec3 = quadtree.retrieve(t5);
    if (vec3.size() != 1 || vec3[0]->name() != "t5") return 6;

    // Ursprünglicher Testfall
    auto p1 = make_test_object("p1",1,1,10,10);
    auto vec = quadtree.retrieve(p1);
    if (vec.size() > 1) return 1;
    if (vec.size() == 0) return 2;
    if (vec[0]->name() != "t1") return 3;
#endif
    return 0;
}
