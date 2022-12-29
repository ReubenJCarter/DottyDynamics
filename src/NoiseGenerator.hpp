#pragma once 

#include <vector>
#include <iostream>
#include <chrono>

#include "thirdparty/VecMath/VecMath.hpp"
#include "thirdparty/FastNoiseLite/FastNoiseLite.h"

#include "Primitives.hpp"

class NoiseGenerator {
    private:
        FastNoiseLite internalnoise[3];

    public:
        NoiseGenerator(){
            setSeed(0); 
            setType(NoiseType::Simplex);
             
        }

        void setSeed(int s){
            for(int i = 0; i < 3; i++){
                internalnoise[i].SetSeed(s*3 + i);
            } 
        }

        void setType(NoiseType t){
            for(int i = 0; i < 3; i++){
                if(t == NoiseType::Simplex || t == NoiseType::SimplexCurl)
                    internalnoise[i].SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
                else if(t == NoiseType::Perlin || t == NoiseType::PerlinCurl)
                    internalnoise[i].SetNoiseType(FastNoiseLite::NoiseType_Perlin);
                else if(t == NoiseType::Value || t == NoiseType::ValueCurl)
                    internalnoise[i].SetNoiseType(FastNoiseLite::NoiseType_Value);
            }
        }

        float get(Vec3& s){
            float n;
            n = internalnoise[0].GetNoise(s.x, s.y, s.z);
            return n;
        }

        float get(float x, float y, float z){
            float n;
            n = internalnoise[0].GetNoise(x, y, z);
            return n;
        }

        Vec3 get3(Vec3& s){
            Vec3 n;
            n.x = internalnoise[0].GetNoise(s.x, s.y, s.z);
            n.y = internalnoise[1].GetNoise(s.x, s.y, s.z);
            n.z = internalnoise[2].GetNoise(s.x, s.y, s.z);
            return n;
        }

        Vec3 get3(float x, float y, float z){
            Vec3 n; 
            n.x = internalnoise[0].GetNoise(x, y, z);
            n.y = internalnoise[1].GetNoise(x, y, z);
            n.z = internalnoise[2].GetNoise(x, y, z);
            return n;
        }

        Vec3 curl(Vec3& s){
            
            const float eps = 0.0001;

            float x = s.x; 
            float y = s.y; 
            float z = s.z;

            Vec3 curl;

            //Find rate of change in YZ plane
            float n1 = internalnoise[2].GetNoise(x, y + eps, z); 
            float n2 = internalnoise[2].GetNoise(x, y - eps, z); 
            //Average to find approximate derivative
            float a = (n1 - n2)/(2 * eps);
            n1 = internalnoise[1].GetNoise(x, y, z + eps); 
            n2 = internalnoise[1].GetNoise(x, y, z - eps); 
            //Average to find approximate derivative
            float b = (n1 - n2)/(2 * eps);
            curl.x = a - b;

            //Find rate of change in XZ plane
            n1 = internalnoise[0].GetNoise(x, y, z + eps); 
            n2 = internalnoise[0].GetNoise(x, y, z - eps); 
            a = (n1 - n2)/(2 * eps);
            n1 = internalnoise[2].GetNoise(x + eps, y, z); 
            n2 = internalnoise[2].GetNoise(x - eps, y, z); 
            b = (n1 - n2)/(2 * eps);
            curl.y = a - b;

            //Find rate of change in XY plane
            n1 = internalnoise[1].GetNoise(x + eps, y, z); 
            n2 = internalnoise[1].GetNoise(x - eps, y, z); 
            a = (n1 - n2)/(2 * eps);
            n1 = internalnoise[0].GetNoise(x, y + eps, z); 
            n2 = internalnoise[0].GetNoise(x, y - eps, z); 
            b = (n1 - n2)/(2 * eps);
            curl.z = a - b;

            return curl;

        }
}; 