#include "../pgame.h"
#include "../pcolider.h"


auto main(int argc, char** argv) -> int{
    P_UNUSED(argc);
    P_UNUSED(argv);
    auto drawable = std::make_shared<PRect>(205,160,10,10); 
    auto rect = std::make_shared<PRect>(200,110,300,50);

    PSatColider colider;
    auto result = colider.colide(drawable,rect);


    if(!result){
        std::cout<<"Collision not found"<<std::endl;
        return 1;
    }else{
        std::cout<<"Collision found"<<std::endl;
        auto normal = colider.normals();
        std::cout<<"Normal: "<<normal[0]<<" "<<normal[1]<<std::endl;
    }



    return 0;
}
