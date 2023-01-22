#pragma once 

#include "thirdparty/VecMath/VecMath.hpp"

enum Falloff{Constant, InvDist, InvDist2, InvDistWell, InvDist2Well }; 
enum NoiseType{Simplex, SimplexCurl, Perlin, PerlinCurl, Value, ValueCurl}; 
enum FieldMode{Force, CorrectionForce}; 
enum BoundShapeType{Box, Sphere, Infinite}; 
enum BoundFalloff{Linear, Squared, Cubed}; 
enum StrangeAttractorType{Aizawa, Arneodo, Dadras, Dequan, Lorenz, LorenzMod2, Thomas }; 

struct WorldParams {
    float timestep; 
    int substeps; 
    float globalDamping; 
    float gravity;
    float collisionFloorStaticFriction; 
    float collisionFloorKineticFriction; 
    float collisionFloorHeight; 
    bool hasCollisionFloor;
};

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

struct StrangeAttractor {
    Vec3 position; 
    Vec3 scale; 
    StrangeAttractorType type; 
    float strength;
    float minDist; 
    float maxDist;
    Falloff falloff;  
    float param[10]; 
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
    float noiseScale; 
    float strength;
    int resolution; 
    NoiseType noiseType;  
    FieldMode mode; 
    Vec3 boundSize; 
    BoundShapeType boundShape;
    float boundThickness; 
    BoundFalloff boundFalloff; 
    Mat3 boundInvRotation; 
}; 

struct GlobalForce {
    Vec3 position; 
    Vec3 direction;
    float strength; 
    Vec3 boundSize; 
    BoundShapeType boundShape;
    float boundThickness; 
    BoundFalloff boundFalloff; 
    Mat3 boundInvRotation; 
};

struct BoxCollider {
    Vec3 position; 
    Mat3 invRotation; 
    Vec3 size;
    float staticFriction; 
    float kineticFriction; 
    bool inverse; 
}; 

struct SphereCollider {
    Vec3 position; 
    float radius;
    float staticFriction; 
    float kineticFriction; 
    bool inverse;
}; 