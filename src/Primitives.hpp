#pragma once 

#include "thirdparty/VecMath/VecMath.hpp"

struct Particle {
    Vec3 position;
    Vec3 positionNext; 
    Vec3 velocity; 
    Vec3 rodDelta; 
    int rodCount; 
    float invMass; 
}; 

struct Rod {
    int a; 
    int b; 
    float length; 
    float strength;
}; 

struct Attractor {
    Vec3 position; 
    float strength;
}; 

struct Vortex {
    Vec3 position; 
    Vec3 normal; 
    float strength;
}; 