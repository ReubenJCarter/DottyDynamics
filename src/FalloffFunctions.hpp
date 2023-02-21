#pragma once

#include <math.h>

inline float falloffConstant(float xDiff, float yDiff, float zDiff, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    float distFactor; 
    if(dist > maxDist)
        distFactor = 0;
    else
        distFactor = 1; 

    return distFactor; 
}

inline float falloffInvDist2(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    float distFactor; 
    if(dist > maxDist)
        distFactor = 0;
    else if(dist <= minDist)
        distFactor = dist == 0 ? 0 : 1 / (minDist * minDist * dist);
    else
        distFactor = 1 / (dist2 * dist);

    return distFactor; 
}

inline float falloffInvDist(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    float distFactor; 
    if(dist > maxDist)
        distFactor = 0;
    else if(dist <= minDist)
        distFactor = dist == 0 ? 0 : 1 / (minDist * dist);
    else
        distFactor = 1 / (dist2); 

    return distFactor; 
}

inline float falloffInvDist2Well(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){

    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff;
    float maxDist2 = maxDist * maxDist; 
    float minDist2 = minDist * minDist; 

    float distFactor = dist2 > maxDist2 || dist2 < minDist2 ? 0 : dist2 - minDist2;

    return distFactor; 
}

inline float falloffInvDistWell(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    float distFactor = dist > maxDist || dist < minDist ? 0 : dist - minDist;

    return distFactor; 
}

inline float falloffLinearRange(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    if(dist <= minDist)
        return 1.0f; 
    else if (dist > maxDist)
        return 0.0f; 

    float distFactor = (maxDist - dist) / (maxDist - minDist);

    return distFactor; 
}

inline float falloffSquaredRange(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    if(dist <= minDist)
        return 1.0f; 
    else if (dist > maxDist)
        return 0.0f; 

    float distFactor = (maxDist - dist) / (maxDist - minDist);

    return distFactor * distFactor; 
}

inline float falloffCubedRange(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    if(dist <= minDist)
        return 1.0f; 
    else if (dist > maxDist)
        return 0.0f; 

    float distFactor = (maxDist - dist) / (maxDist - minDist);

    return distFactor * distFactor * distFactor; 
}