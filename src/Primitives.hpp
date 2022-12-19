#pragma once 

#include "thirdparty/VecMath/VecMath.hpp"

enum Falloff{Constant, Linear, Squared, LinearWell, SquaredWell}; 

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
    float stiffness;
}; 

struct AnchorRod {
    Vec3 position; 
    int a; 
    float length; 
    float stiffness;
}; 

struct Attractor {
    Vec3 position; 
    float strength;
    float minDist; 
    float maxDist; 
    Falloff falloff; 
}; 

struct Vortex {
    Vec3 position; 
    Vec3 normal; 
    float strength;
    float minDist; 
    float maxDist; 
    Falloff falloff; 
}; 

struct CurlNoise {
    Vec3 position; 
    Vec3 boundSize; 
    float strength;
    Falloff falloff; 
    float noiseScale; 
    int resolution; 
}; 