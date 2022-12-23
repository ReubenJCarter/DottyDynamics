#pragma once 

#include <vector>
#include <iostream>
#include <chrono>

#include "thirdparty/VecMath/VecMath.hpp"
#include "thirdparty/FastNoiseLite/FastNoiseLite.h"

class Noise {
    private:
        FastNoiseLite internalnoise;

    public:
        Noise(){
            internalnoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        }

        float get(Vec3& s){
            return internalnoise.GetNoise(s.x, s.y, s.z);
        }

        float get(float x, float y, float z){
            return internalnoise.GetNoise(x, y, z);
        }

        Vec3& getCurl(Vec3& s){
            const float eps = 0.0001;

            float x = s.x; 
            float y = s.y; 
            float z = s.z;

            Vec3 curl;

            //Find rate of change in YZ plane
            float n1 = get(x, y + eps, z); 
            float n2 = get(x, y - eps, z); 
            //Average to find approximate derivative
            float a = (n1 - n2)/(2 * eps);
            n1 = get(x, y, z + eps); 
            n2 = get(x, y, z - eps); 
            //Average to find approximate derivative
            float b = (n1 - n2)/(2 * eps);
            curl.x = a - b;

            //Find rate of change in XZ plane
            n1 = get(x, y, z + eps); 
            n2 = get(x, y, z - eps); 
            a = (n1 - n2)/(2 * eps);
            n1 = get(x + eps, y, z); 
            n2 = get(x - eps, y, z); 
            b = (n1 - n2)/(2 * eps);
            curl.y = a - b;

            //Find rate of change in XY plane
            n1 = get(x + eps, y, z); 
            n2 = get(x - eps, y, z); 
            a = (n1 - n2)/(2 * eps);
            n1 = get(x, y + eps, z); 
            n2 = get(x, y - eps, z); 
            b = (n1 - n2)/(2 * eps);
            curl.z = a - b;

            return curl;
        }
}; 