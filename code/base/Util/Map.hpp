//
// Created by Niels on 6/16/2019.
//

#ifndef IPASS_MAP_HPP
#define IPASS_MAP_HPP
#include <hwlib.hpp>

template<class K, class V, int capacity=16>
class Map {
    K KeyArray[capacity] = {};
    V ValueArray[capacity] = {};
    size_t currentSize;

public:
    Map() {}



    size_t size() {
        return currentSize;
    }

    bool put(K key, V val) {
        if (currentSize >= capacity) {
            return false;
        }
        KeyArray[currentSize] = key;
        ValueArray[currentSize] = val;
        currentSize++;
        return true;
    }

    V get(K key, V defaultVal) {
        for (size_t i = 0; i < currentSize; i++) {
            if (KeyArray[i] == key) {
                return ValueArray[i];
            }
        }
        return defaultVal;
    }

    K find(V val, K defaultKey) {
        for (size_t i = 0; i < currentSize; i++) {
            if (ValueArray[i] == val) {
                return KeyArray[i];
            }
        }
        return defaultKey;
    }

    V remove(K key, V defaultVal) {
        for (size_t i = 0; i < currentSize; i++) {
            if (KeyArray[i] == key) {
                V ret = ValueArray[i];
                remove(i);
                return ret;
            }
        }
        return defaultVal;
    }

    void remove(uint8_t index) {
        for(size_t i = index; i < currentSize-1; i++) {
            KeyArray[i] = KeyArray[i+1];
            ValueArray[i] = ValueArray[i+1];
        }
        currentSize--;
    }




};




#endif //IPASS_MAP_HPP
