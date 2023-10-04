#include "../datastructs/phashmap.h"
#include <string>
#include <iostream>

auto main() -> int{
    PHashMap<std::string,int> map;
    map.insert("one",1);
    map.insert("two",2);
    map.insert("three",3);
    map.insert("four",4);
    map.insert("five",5);
    map.insert("six",6);
    map.insert("seven",7);
    map.insert("eight",8);
    map.insert("nine",9);
    map.insert("ten",10);
    map.insert("eleven",11);
    map.insert("twelve",12);
    map.insert("thirteen",13);
    map.insert("fourteen",14);
    map.insert("fifteen",15);
    map.insert("sixteen",16);
    map.insert("seventeen",17);


    int count = 0;
    if(map.contains("one")){
        count++;
    }
    if(map.contains("two")){
        count++;
    }
    if(map.contains("three")){
        count++;
    }
    if(map.contains("four")){
        count++;
    }
    if(map.contains("five")){
        count++;
    }
    if(map.contains("six")){
        count++;
    }
    if(map.contains("seven")){
        count++;
    }
    if(map.contains("eight")){
        count++;
    }
    if(map.contains("nine")){
        count++;
    }
    if(map.contains("ten")){
        count++;
    }
    if(map.contains("eleven")){
        count++;
    }
    if(map.contains("twelve")){
        count++;
    }
    if(map.contains("thirteen")){
        count++;
    }
    if(map.contains("fourteen")){
        count++;
    }
    if(map.contains("fifteen")){
        count++;
    }
    if(map.contains("sixteen")){
        count++;
    }
    if(map.contains("seventeen")){
        count++;
    }
    if(map.contains("eighteen")){
        count--;
    }

    if(count != 17){
        std::cout<<"FAIL: "<<count<<"\n";
        return 1;
    }

    map["eighteen"] = 18;
    if(map.contains("eighteen")){
        std::cout<<"OK\n";
        return 0;
    }
}