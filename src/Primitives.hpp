#pragma once 

#include <stdint.h>

#include "thirdparty/VecMath/VecMath.hpp"

enum Falloff{Constant, InvDist, InvDist2, InvDistWell, InvDist2Well, LinearRange, SquaredRange, CubedRange }; 
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

struct AngleConstraint {
    int a; 
    int b; 
    int c; 
    float angle;  
    float stiffness;
}; 

struct Attractor {
    uint32_t layerMask; 
    Vec3 position; 
    float strength;
    float minDist; 
    float maxDist; 
    Falloff falloff; 
}; 

struct StrangeAttractor {
    uint32_t layerMask; 
    Vec3 position; 
    float scale; 
    StrangeAttractorType type; 
    float strength;
    float targetSpeed; 
    FieldMode fieldMode; 
    float minDist; 
    float maxDist;
    Falloff falloff;  
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
}; 

struct Vortex {
    uint32_t layerMask; 
    Vec3 position; 
    Vec3 normal; 
    float strength;
    float minDist; 
    float maxDist; 
    Falloff falloff; 
}; 

struct NoiseField {
    uint32_t layerMask; 
    Vec3 position; 
    float noiseScale; 
    float strength;
    float targetSpeed;  
    NoiseType noiseType;  
    FieldMode mode; 
    Vec3 boundSize; 
    BoundShapeType boundShape;
    float boundThickness; 
    BoundFalloff boundFalloff; 
    Mat3 boundInvRotation; 
    int bakeResolution;
    bool useBake;
}; 

struct GlobalForce {
    uint32_t layerMask; 
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
    uint32_t layerMask; 
    Vec3 position; 
    Mat3 invRotation; 
    Vec3 size;
    float staticFriction; 
    float kineticFriction; 
    bool inverse; 
}; 

struct SphereCollider {
    uint32_t layerMask; 
    Vec3 position; 
    float radius;
    float staticFriction; 
    float kineticFriction; 
    bool inverse;
}; 

struct Damper {
    uint32_t layerMask; 
    Vec3 position; 
    float strength; 
    Vec3 boundSize; 
    BoundShapeType boundShape;
    float boundThickness; 
    BoundFalloff boundFalloff; 
    Mat3 boundInvRotation; 
    bool useBake;
};