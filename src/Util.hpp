#pragma once 

#include <stdint.h>

template <typename T>
inline T setBitVal(T num, int inx, bool val){
    return val ? num | (1 << inx) : num & ~(1 << inx);
}