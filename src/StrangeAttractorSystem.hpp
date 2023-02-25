#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Util.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"
#include "FalloffFunctions.hpp"

class StrangeAttractorSystem {
    private:
        DynamicPool<StrangeAttractor> strangeAttractors; 


        float lerp(float v0, float v1, float t) {
            return v0 + t * (v1 - v0);
        }

        inline Vec3 computeStrangeAttractor(float x, float y, float z, StrangeAttractorType type, float a, float b, float c, float d, float e, float f){
            if(type == StrangeAttractorType::Aizawa)
                return computeAizawa(x, y, z, a, b, c, d, e, f); 
            else if(type == StrangeAttractorType::Arneodo)
                return computeArneodo(x, y, z, a, b, c, d, e, f); 
            else if(type == StrangeAttractorType::Dadras)
                return computeDadras(x, y, z, a, b, c, d, e, f); 
            else if(type == StrangeAttractorType::Dequan)
                return computeDequan(x, y, z, a, b, c, d, e, f); 
            else if(type == StrangeAttractorType::Lorenz)
                return computeLorenz(x, y, z, a, b, c, d, e, f); 
            else if(type == StrangeAttractorType::LorenzMod2)
                return computeLorenzMod2(x, y, z, a, b, c, d, e, f); 
            else if(type == StrangeAttractorType::Thomas)
                return computeThomas(x, y, z, a, b, c, d, e, f); 

            else 
                return Vec3(0, 0, 0); 
        }

        inline Vec3 computeAizawa(float x, float y, float z, float a, float b, float c, float d, float e, float f){
            a = 0.95;
            b = 0.7;
            c = 0.6;
            d = 3.5;
            e = 0.25;
            f = 0.1;
        
            Vec3 v; 
            v.x = ((z-b)*x - d*y); 
            v.y = (d * x + (z-b) * y); 
            v.z = c + a*z - ((z*z*z) /3) - (x*x) + f * z * (x*x*x);  
            return v; 
        }

        inline Vec3 computeArneodo(float x, float y, float z, float a, float b, float c, float d, float e, float f){
            a = -5.5;
            b = 3.5;
            d = -1;

            Vec3 v; 
            v.x = y; 
            v.y = z; 
            v.z = (-a*x -b*y -z + (d* (pow(x, 3))));  
            return v; 
        }

        inline Vec3 computeDadras(float x, float y, float z, float a, float b, float c, float d, float e, float f){
            a = 3;
            b = 2.7;
            c = 1.7;
            d = 2;
            e = 9;
            
            Vec3 v; 
            v.x = (y- a*x +b*y*z); 
            v.y = (c*y -x*z +z); 
            v.z = (d*x*y - e*z);  
            return v; 
        }

        inline Vec3 computeDequan(float x, float y, float z, float a, float b, float c, float d, float e, float f){
            a = 40.0;
            b = 1.833;
            c = 0.16;
            d = 0.65;
            e = 55.0;
            f = 20.0;

            Vec3 v; 
            v.x = ( a*(y-x) + c*x*z); 
            v.y = (e*x + f*y - x*z); 
            v.z = (b*z + x*y - d*x*x);  
            return v; 
        }

        inline Vec3 computeLorenz(float x, float y, float z, float a, float b, float c, float d, float e, float f){
            a = 10.0;
            b = 28.0;
            c = 2.6666666667;

            Vec3 v; 
            v.x = (a * (y - x)); 
            v.y = (x * (b-z) - y); 
            v.z = (x*y - c*z);  
            return v; 
        }

        inline Vec3 computeLorenzMod2(float x, float y, float z, float a, float b, float c, float d, float e, float f){
            a = 0.9;
            b = 5.0;
            c = 9.9;
            d = 1.0;

            Vec3 v; 
            v.x = (-a*x+ y*y - z*z + a *c); 
            v.y = (x*(y-b*z)+d); 
            v.z = (-z + x*(b*y +z));  
            return v; 
        }

        inline Vec3 computeThomas(float x, float y, float z, float a, float b, float c, float d, float e, float f){
            b = 0.19;

            Vec3 v; 
            v.x = (-b*x + sin(y)); 
            v.y = (-b*y + sin(z)); 
            v.z = (-b*z + sin(x));  
            return v; 
        }

    public: 
        StrangeAttractorSystem(){
            strangeAttractors.setPoolSize(1000); 
        }

        void updateStrangeAttractors(BS::thread_pool& threadPool, float timestep, WorldParams& params, DynamicPool<Particle>& particles){

            unsigned int maxAttractorCount = strangeAttractors.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxAttractorCount, timestep, &particles](const int a, const int b){
                
                for(int i = 0; i < maxAttractorCount; i++){

                    if(!strangeAttractors.isInUse(i))
                        continue;          

                    float ka = strangeAttractors[i].a; 
                    float kb = strangeAttractors[i].b; 
                    float kc = strangeAttractors[i].c; 
                    float kd = strangeAttractors[i].d; 
                    float ke = strangeAttractors[i].e; 
                    float kf = strangeAttractors[i].f;

                    float ascale = strangeAttractors[i].scale; 
                    float astrength = strangeAttractors[i].strength; 
                    float atargetspeed = strangeAttractors[i].targetSpeed;
                    FieldMode fieldMode = strangeAttractors[i].fieldMode;
                    Falloff falloff = strangeAttractors[i].falloff;
                    

                    for(int p = a; p < b; p++){

                        float xDiff = particles[p].position.x - strangeAttractors[i].position.x; 
                        float yDiff = particles[p].position.y - strangeAttractors[i].position.y;
                        float zDiff = particles[p].position.z - strangeAttractors[i].position.z;

                        float distFactor; 

                        if(falloff == Falloff::Constant)
                            distFactor = falloffConstant(xDiff, yDiff, zDiff, strangeAttractors[i].maxDist); 
                        else if(falloff == Falloff::InvDist2)
                            distFactor = falloffInvDist2(xDiff, yDiff, zDiff, strangeAttractors[i].minDist, strangeAttractors[i].maxDist); 
                        else if(falloff == Falloff::InvDist)
                            distFactor = falloffInvDist(xDiff, yDiff, zDiff, strangeAttractors[i].minDist, strangeAttractors[i].maxDist); 
                        else if(falloff == Falloff::InvDistWell)
                            distFactor = falloffInvDistWell(xDiff, yDiff, zDiff, strangeAttractors[i].minDist, strangeAttractors[i].maxDist); 
                        else if(falloff == Falloff::InvDist2Well)
                            distFactor = falloffInvDist2Well(xDiff, yDiff, zDiff, strangeAttractors[i].minDist, strangeAttractors[i].maxDist); 
                        else if(falloff == Falloff::LinearRange)
                            distFactor = falloffLinearRange(xDiff, yDiff, zDiff, strangeAttractors[i].minDist, strangeAttractors[i].maxDist); 
                        else if(falloff == Falloff::SquaredRange)
                            distFactor = falloffSquaredRange(xDiff, yDiff, zDiff, strangeAttractors[i].minDist, strangeAttractors[i].maxDist); 
                        else if(falloff == Falloff::CubedRange)
                            distFactor = falloffCubedRange(xDiff, yDiff, zDiff, strangeAttractors[i].minDist, strangeAttractors[i].maxDist); 
                        else
                            distFactor = 1;
                        
                        Vec3 att = computeStrangeAttractor(xDiff*ascale, yDiff*ascale, zDiff*ascale, strangeAttractors[i].type, ka, kb, kc, kd, ke, kf); 

                        if(strangeAttractors[i].fieldMode == FieldMode::Force){
                            float fX = att.x * strangeAttractors[i].strength; 
                            float fY = att.y * strangeAttractors[i].strength; 
                            float fZ = att.z * strangeAttractors[i].strength; 
                            
                            particles[p].velocity.x += timestep * particles[p].invMass * fX * distFactor;
                            particles[p].velocity.y += timestep * particles[p].invMass * fY * distFactor;
                            particles[p].velocity.z += timestep * particles[p].invMass * fZ * distFactor;
                        }
                        else if (strangeAttractors[i].fieldMode == FieldMode::CorrectionForce){
                            float targetVelocityX = att.x * strangeAttractors[i].targetSpeed;
                            float targetVelocityY = att.y * strangeAttractors[i].targetSpeed; 
                            float targetVelocityZ = att.z * strangeAttractors[i].targetSpeed; 
                            float verrX = targetVelocityX - particles[p].velocity.x; 
                            float verrY = targetVelocityY - particles[p].velocity.y; 
                            float verrZ = targetVelocityZ - particles[p].velocity.z;
                            float kp =  strangeAttractors[i].strength;
                            float correctingForceX = verrX * kp; 
                            float correctingForceY = verrY * kp;
                            float correctingForceZ = verrZ * kp;

                            particles[p].velocity.x += timestep * particles[p].invMass * correctingForceX * distFactor;
                            particles[p].velocity.y += timestep * particles[p].invMass * correctingForceY * distFactor;
                            particles[p].velocity.z += timestep * particles[p].invMass * correctingForceZ * distFactor;
                        }
                    }
                }
            }).wait();

        }

        int addStrangeAttractor(Vec3 position, float scale, StrangeAttractorType type, float strength, float targetSpeed, FieldMode fieldMode, float minDist, float maxDist, Falloff falloff, 
                               float a, float b, float c, float d, float e, float f){
            StrangeAttractor sa; 
            sa.layerMask = 0xFFFFFFFF; 
            sa.position = position;
            sa.scale = scale;  
            sa.type = type; 
            sa.strength = strength;
            sa.targetSpeed = targetSpeed; 
            sa.fieldMode = fieldMode; 
            sa.minDist = minDist;
            sa.maxDist = maxDist; 
            sa.falloff = falloff;
            sa.a = a; sa.b = b; sa.c = c; sa.d = d; sa.e = e; sa.f = f;
            return strangeAttractors.add(sa);  
        }

        StrangeAttractor* getStrangeAttractorPtr(int inx){
            return &(strangeAttractors[inx]); 
        }

        void destroyStrangeAttractor(int inx){
            strangeAttractors.remove(inx); 
        }

        void clearStrangeAttractors(){
            strangeAttractors.clear(); 
        }

        void setStrangeAttractorLayerEnabled(int inx, int layer, bool enabled){
            strangeAttractors[inx].layerMask = setBitVal(strangeAttractors[inx].layerMask, layer, enabled); 
        }

        void zeroStrangeAttractorLayerMask(int inx){
            strangeAttractors[inx].layerMask = 0; 
        }

        void setStrangeAttractorPosition(int inx, Vec3 position){
            strangeAttractors[inx].position = position; 
        }

        void setStrangeAttractorScale(int inx, float scale){
            strangeAttractors[inx].scale = scale; 
        }

        void setStrangeAttractorType(int inx, StrangeAttractorType type){
            strangeAttractors[inx].type = type; 
        }

        void setStrangeAttractorStrength(int inx, float strength){
            strangeAttractors[inx].strength = strength; 
        }

        void setStrangeAttractorTargetSpeed(int inx, float targetSpeed){
            strangeAttractors[inx].targetSpeed = targetSpeed; 
        }

        void setStrangeAttractorFieldMode(int inx, FieldMode fieldMode){
            strangeAttractors[inx].fieldMode = fieldMode; 
        }

        void setStrangeAttractorMinDist(int inx, float minDist){
            strangeAttractors[inx].minDist = minDist; 
        }

        void setStrangeAttractorMaxDist(int inx, float maxDist){
            strangeAttractors[inx].maxDist = maxDist; 
        }

        void setStrangeAttractorFalloff(int inx, Falloff falloff){
            strangeAttractors[inx].falloff = falloff;
        }

        void setStrangeAttractorCoeffs(int inx, float a, float b, float c, float d, float e, float f){
            strangeAttractors[inx].a = a; 
            strangeAttractors[inx].b = b; 
            strangeAttractors[inx].c = c; 
            strangeAttractors[inx].d = d; 
            strangeAttractors[inx].e = e; 
            strangeAttractors[inx].f = f; 
        }
};