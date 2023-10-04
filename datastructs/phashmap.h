#ifndef PHASHMAP_H
#define PHASHMAP_H
#include <algorithm>
#include <vector>
#include <cmath>
#include <list>
#include "../core/pdebug.h"


auto find_next_prime(std::size_t n) -> std::size_t;

template<class KEY,class VALUE>
struct Bucket{
    Bucket() : _key{},_value{},_valid(false) {}
    
    Bucket(const KEY & k, const VALUE &v):
        _key(k),_value(v),_valid(true){}

    auto valid() const -> bool {
        return _valid;
    }

    auto key()const -> const KEY &{
        return _key;
    } 

    auto value()const -> const VALUE & {
        return _value;
    }

    auto value()-> VALUE & {
        return _value;
    }

    auto hash()const -> std::size_t {
        return _hash;
    }

    void hash(std::size_t h){
        _hash = h;
    }

private:
    KEY _key;
    VALUE _value;
    bool _valid;
    std::size_t _hash;
};


template<class KEY,class VALUE>
class PHashMap{
public:
    PHashMap(){
        _count = 0;
        _container1_count = 0;
        _container2_count = 0;
        _hash1 = 5107;
        _hash2 = 6037;
        _container1.resize(_hash1);
        _container2.resize(_hash2);
    }

    auto contains(const KEY & k) const -> bool{
        std::size_t h = _hash_f(k);
        std::size_t pos1 = h % _hash1;
        std::size_t pos2 = h % _hash2;
        if(_container1[pos1].valid() && _container1[pos1].key() == k){
            return true;
        }
        if(_container2[pos2].valid() && _container2[pos2].key() == k){
            return true;
        }
        return false;
    }

    auto operator[](const KEY & k) -> VALUE & {
        std::size_t h = _hash_f(k);
        std::size_t pos1 = h % _hash1;
        std::size_t pos2 = h % _hash2;
        if(_container1[pos1].valid() && _container1[pos1].key() == k){
            return _container1[pos1].value();
        }
        if(_container2[pos2].valid() && _container2[pos2].key() == k){
            return _container2[pos2].value();
        }
        insert(k,VALUE{});
        return operator[](k);
    }

    void insert(const KEY & key, const VALUE & value){
        Bucket<KEY,VALUE> bucket(key,value);
        std::size_t h = _hash_f(key);
        bucket.hash(h);
        std::size_t pos1 = h % _hash1;
        
        if(!_container1[pos1].valid() || _container1[pos1].key() == key){
            _container1[pos1] = bucket;
            return;
        }

        std::size_t pos2 = h % _hash2;
        if(!_container2[pos2].valid() || _container2[pos2].key() == key){
            _container2[pos2] = bucket;
            return;
        }
        insert(bucket);
    }

private:
    void insert(const Bucket<KEY,VALUE> & bucket){
        bool run = true;
        Bucket<KEY,VALUE> iter = bucket;
        std::size_t loop_count = 0;
        while(run){
            std::size_t pos1 = iter.hash() % _hash1;
            
            if(_container1[pos1].valid() && _container1[pos1].key() != iter.key()){
                auto tmp = _container1[pos1];
                _container1[pos1] = iter;
                iter = tmp;
            }else{
                _container1[pos1] = iter;
                return;
            }
            std::size_t pos2 = iter.hash() % _hash2;
            if(_container2[pos2].valid() && _container2[pos2].key() != iter.key()){
                auto tmp = _container2[pos2];
                _container2[pos2] = iter;
                iter = tmp;
            }else{
                _container2[pos2] = iter;
                return;
            }
            loop_count++;
            if(loop_count > (_hash1 + _hash2)/2){
                resize(iter);
                return;
            }
        }
    }

    void resize(const Bucket<KEY,VALUE> & bucket){
        std::size_t old_hash1 = _hash1;
        std::size_t old_hash2 = _hash2;
        _hash1 = find_next_prime( old_hash1 * 2);
        _hash2 = find_next_prime( old_hash2 * 2);
        std::list<Bucket<KEY,VALUE>> tmp_container1;
        std::list<Bucket<KEY,VALUE>> tmp_container2;
        for(auto & x : _container1){
            if(x.valid()){
                tmp_container1.push_back(x);
            }
        }
        for(auto & x : _container2){
            if(x.valid()){
                tmp_container2.push_back(x);
            }
        }
        _container1.clear();
        _container2.clear();
        _container1.resize(_hash1);
        _container2.resize(_hash2);
        for(auto & x : tmp_container1){
            insert(x);
        }
        for(auto & x : tmp_container2){
            insert(x);
        }
        insert(bucket);
    }

private:
    std::size_t _count;
    std::size_t _container1_count;
    std::size_t _container2_count;
    std::size_t _hash1;
    std::size_t _hash2;
    std::hash<KEY> _hash_f;
    std::vector<Bucket<KEY,VALUE>> _container1;
    std::vector<Bucket<KEY,VALUE>> _container2;
};

#endif