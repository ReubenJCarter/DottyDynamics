#pragma once

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Util.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"
#include "FalloffFunctions.hpp"

class AttractorSystem {
    private:
        DynamicPool<Attractor> attractors;

    public: 
        AttractorSystem(){
            attractors.setPoolSize(1000);  
        }

        void updateAttractors(BS::thread_pool& threadPool, float timestep, WorldParams& params, DynamicPool<Particle>& particles, DynamicPool<uint32_t>& particleLayerMask){

            unsigned int maxAttractorCount = attractors.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxAttractorCount, timestep, &particles, &particleLayerMask](const int a, const int b){
                

                for(int i = 0; i < maxAttractorCount; i++){

                    if(!attractors.isInUse(i))
                        continue;          

                    uint32_t M = attractors[i].layerMask;

                    if(attractors[i].falloff == Falloff::Constant){
                        for(int p = a; p < b; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float distFactor = falloffConstant(xDiff, yDiff, zDiff, attractors[i].maxDist); 

                            distFactor = M & particleLayerMask[p] ? distFactor : 0;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::InvDist2){
                        for(int p = a; p < b; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float distFactor = falloffInvDist2(xDiff, yDiff, zDiff, attractors[i].minDist, attractors[i].maxDist); 

                            distFactor = M & particleLayerMask[p] ? distFactor : 0;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::InvDist){
                        for(int p = a; p < b; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float distFactor = falloffInvDist(xDiff, yDiff, zDiff, attractors[i].minDist, attractors[i].maxDist); 

                            distFactor = M & particleLayerMask[p] ? distFactor : 0;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::InvDistWell){
                        for(int p = a; p < b; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float distFactor = falloffInvDistWell(xDiff, yDiff, zDiff, attractors[i].minDist, attractors[i].maxDist); 

                            distFactor = M & particleLayerMask[p] ? distFactor : 0;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::InvDist2Well){
                        for(int p = a; p < b; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float distFactor = falloffInvDist2Well(xDiff, yDiff, zDiff, attractors[i].minDist, attractors[i].maxDist); 

                            distFactor = M & particleLayerMask[p] ? distFactor : 0;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::LinearRange){
                        for(int p = a; p < b; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float distFactor = falloffLinearRange(xDiff, yDiff, zDiff, attractors[i].minDist, attractors[i].maxDist); 

                            distFactor = M & particleLayerMask[p] ? distFactor : 0;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::SquaredRange){
                        for(int p = a; p < b; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float distFactor = falloffSquaredRange(xDiff, yDiff, zDiff, attractors[i].minDist, attractors[i].maxDist); 

                            distFactor = M & particleLayerMask[p] ? distFactor : 0;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::CubedRange){
                        for(int p = a; p < b; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float distFactor = falloffCubedRange(xDiff, yDiff, zDiff, attractors[i].minDist, attractors[i].maxDist); 

                            distFactor = M & particleLayerMask[p] ? distFactor : 0;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                
                }
            }).wait();

        }

        int addAttrator(Vec3 position, float strength, float minDist, float maxDist, Falloff falloff){
            Attractor a; 
            a.layerMask = 0xFFFFFFFF; 
            a.position = position; 
            a.strength = strength;
            a.minDist = minDist;
            a.maxDist = maxDist; 
            a.falloff = falloff;
            return attractors.add(a);  
        }

        Attractor* getAttractorPtr(int inx){
            return &(attractors[inx]); 
        }

        void destroyAttractor(int inx){
            attractors.remove(inx); 
        }

        void clearAttractors(){
            attractors.clear(); 
        }

        void setAttractorLayerEnabled(int inx, int layer, bool enabled){
            attractors[inx].layerMask = setBitVal(attractors[inx].layerMask, layer, enabled); 
        }

        void zeroAttractorLayerMask(int inx){
            attractors[inx].layerMask = 0; 
        }

        void setAttractorPosition(int inx, Vec3 position){
            attractors[inx].position = position; 
        }

        void setAttractorStrength(int inx, float strength){
            attractors[inx].strength = strength; 
        }

        void setAttractorMinDist(int inx, float minDist){
            attractors[inx].minDist = minDist; 
        }

        void setAttractorMaxDist(int inx, float maxDist){
            attractors[inx].maxDist = maxDist; 
        }

        void setAttractorFalloff(int inx, Falloff falloff){
            attractors[inx].falloff = falloff;
        }
}; 