#pragma once

#include <math.h>

float falloffConstant(float xDiff, float yDiff, float zDiff, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    float distFactor; 
    if(dist > maxDist)
        distFactor = 0;
    else
        distFactor = 1; 

    return distFactor; 
}

float falloffInvDist2(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    float distFactor; 
    if(dist > maxDist)
        distFactor = 0;
    else if(dist < minDist)
        distFactor = dist == 0 ? 0 : 1 / (minDist * minDist * dist);
    else
        distFactor = 1 / (dist2 * dist);

    return distFactor; 
}

float falloffInvDist(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    float distFactor; 
    if(dist > maxDist)
        distFactor = 0;
    else if(dist < minDist)
        distFactor = dist == 0 ? 0 : 1 / (minDist * dist);
    else
        distFactor = 1 / (dist2); 

    return distFactor; 
}

float falloffInvDist2Well(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){

    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff;
    float maxDist2 = maxDist * maxDist; 
    float minDist2 = minDist * minDist; 

    float distFactor = dist2 > maxDist2 || dist2 < minDist2 ? 0 : dist2 - minDist2;

    return distFactor; 
}

float falloffInvDistWell(float xDiff, float yDiff, float zDiff, float minDist, float maxDist){
    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
    float dist = sqrt(dist2); 

    float distFactor = dist > maxDist || dist < minDist ? 0 : dist - minDist;

    return distFactor; 
}