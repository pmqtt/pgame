#include <cmath>
#include "phashmap.h"

auto find_next_prime(std::size_t n) -> std::size_t {
    std::size_t prime = n;
    bool found = false;
    while(!found){
        bool is_prime = true;
        for(std::size_t i = 2; i <= std::sqrt(prime)+1; ++i){
            if(prime % i == 0){
                is_prime = false;
                break;
            }
        }
        if(is_prime){
            found = true;
        }else{
            prime++;
        }
    }
    return prime;
}