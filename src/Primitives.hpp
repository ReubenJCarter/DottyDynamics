#pragma once 

#include "thirdparty/VecMath/VecMath.hpp"

enum Falloff{Constant, Linear, Squared, LinearWell, SquaredWell}; 
enum NoiseType{Simplex, SimplexCurl, Perlin, PerlinCurl, Value, ValueCurl}; 
enum ShapeType{Box, Sphere, Ellipsoid};


struct Particle {
    Vec3 position;
    Vec3 positionNext; 
    Vec3 velocity; 
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

struct NoiseField {
    Vec3 position; 
    Vec3 boundSize; 
    ShapeType boundShape;
    float falloffRatio; 
    Falloff falloff; 
    float noiseScale; 
    float strength;
    int resolution; 
    NoiseType noiseType; 
    float viscosity; 
    bool isVelocity; 
}; 

struct BoxCollider {
    Vec3 position; 
    Vec3 size;
    bool inverse; 
}; 

struct SphereCollider {
    Vec3 position; 
    float size;
    bool inverse;
}; 