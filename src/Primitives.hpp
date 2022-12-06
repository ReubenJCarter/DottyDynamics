#pragma once 

#include "thirdparty/VecMath/VecMath.hpp"

class Particle {
    public:
        Vec3 position;
        Vec3 positionNext; 
        Vec3 velocity; 
        Vec3 rodDelta; 
        int rodCount; 
        float invMass; 
}; 

class Rod {
    public:
        int a; 
        int b; 
        float length; 
        float strength;
}; 

class Attractor {
    public:
        Vec3 position; 
        float strength;
}; 

class Vortex {
    public:
        Vec3 position; 
        Vec3 normal; 
        float strength;
}; 