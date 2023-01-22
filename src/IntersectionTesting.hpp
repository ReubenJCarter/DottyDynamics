#pragma once 

#include "Primitives.hpp"

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

inline float getBoxBoundStrength(Vec3 testPoint, Vec3 position, Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation){
    
    //Transform point into correct space for test 
    Vec3 p = testPoint; 
    p.sub(position); 
    p.multm(boundInvRotation); 
    
    //Test against all sides of box 
    float halfsx = boundSize.x/2;
    float halfsy = boundSize.y/2;
    float halfsz = boundSize.z/2;
    if(p.x > halfsx || p.x < -halfsx || p.y > halfsy || p.y < -halfsy || p.z > halfsz || p.z < -halfsz){
        return 0.0f; 
    }

    //test if inside main region not within the thickness falloff region
    float halfsxSubF = halfsx - boundThickness; 
    float halfsySubF = halfsy - boundThickness; 
    float halfszSubF = halfsz - boundThickness; 
    if(p.x < halfsxSubF && p.x > -halfsxSubF && p.y < halfsySubF && p.y > -halfsySubF && p.z < halfszSubF && p.z > -halfszSubF){
        return 1.0f; 
    }

    //Get dist from each outer wall and find the shortest 
    float dxp = halfsx - p.x; 
    float dxn = p.x + halfsx;
    float shortestDX = MIN(dxp, dxn); 

    float dyp = halfsy - p.y; 
    float dyn = p.y + halfsy;
    float shortestDY = MIN(dyp, dyn); 

    float dzp = halfsz - p.z; 
    float dzn = p.z + halfsz;
    float shortestDZ = MIN(dzp, dzn); 

    float temp = MIN(shortestDX, shortestDY);
    float shortestD = MIN(shortestDZ, temp ); 

    //use the shortest dist from the outer wall as the input to an ease curve 
    float s = shortestD / boundThickness;

    float f = boundFalloff == BoundFalloff::Linear ? s : 
              boundFalloff == BoundFalloff::Squared ? s * s : 
              s * s * s;  
    
    return f; 
}

inline float getSphereBoundStrength(Vec3 testPoint, Vec3 position, Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff){
    
    //Transform point into correct space for test 
    Vec3 p = testPoint; 
    p.sub(position); 

    float d2 = p.len2(); 
    float r2 =  boundSize.x * boundSize.x;

    //Test against sphere
    if(d2 > r2){
        return 0.0f; 
    }

    float innerR = boundSize.x - boundThickness; 
    float innerR2 = innerR * innerR; 

    if(d2 < innerR2){
        return 1.0f; 
    }


    //use distance from the outer wall to the point in the falloff zone as the input to falloff  
    float s = (boundSize.x - sqrt(d2)) / (boundThickness); 

    float f = boundFalloff == BoundFalloff::Linear ? s : 
              boundFalloff == BoundFalloff::Squared ? s * s : 
              s * s * s; 
    
    return f; 
}