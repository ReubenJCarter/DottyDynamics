#pragma once 

#include <stdint.h>
#include <string>

template <typename T>
inline T setBitVal(T num, int inx, bool val){
    return val ? num | (1 << inx) : num & ~(1 << inx);
}

typedef void(*FuncCallBack)(const char* message, int size);

inline void log(FuncCallBack cb, std::string message){
    const char* tmsg = message.c_str();
    if (cb != nullptr)
        cb(tmsg, (int)strlen(tmsg));
}