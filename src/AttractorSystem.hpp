#pragma once

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class AttractorSystem {
    private:
        DynamicPool<Attractor> attractors;

    public: 
        AttractorSystem(){
            attractors.setPoolSize(1000);  
        }

        void updateAttractors(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles){

            unsigned int maxAttractorCount = attractors.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxAttractorCount, &particles, &params](const int a, const int b){
                
                float timestep = params.timestep; 

                for(int i = 0; i < maxAttractorCount; i++){

                    if(!attractors.isInUse(i))
                        continue;          
               

                    if(attractors[i].falloff == Falloff::Constant){
                        for(int p = a; p < b; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor; 
                            if(dist > attractors[i].maxDist)
                                distFactor = 0;
                            else
                                distFactor = 1; 

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

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor; 
                            if(dist > attractors[i].maxDist)
                                distFactor = 0;
                            else if(dist < attractors[i].minDist)
                                distFactor = dist == 0 ? 0 : 1 / (attractors[i].minDist * attractors[i].minDist * dist);
                            else
                                distFactor = 1 / (dist2 * dist); 

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

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor; 
                            if(dist > attractors[i].maxDist)
                                distFactor = 0;
                            else if(dist < attractors[i].minDist)
                                distFactor = dist == 0 ? 0 : 1 / (attractors[i].minDist * dist);
                            else
                                distFactor = 1 / (dist2); 

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

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor = dist > attractors[i].maxDist || dist < attractors[i].minDist ? 0 : dist - attractors[i].minDist;

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

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff;
                            float maxDist2 = attractors[i].maxDist * attractors[i].maxDist; 
                            float minDist2 = attractors[i].minDist * attractors[i].minDist; 

                            float distFactor = dist2 > maxDist2 || dist2 < minDist2 ? 0 : dist2 - minDist2;

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