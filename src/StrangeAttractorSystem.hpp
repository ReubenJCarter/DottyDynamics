#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class StrangeAttractorSystem {
    private:
        DynamicPool<StrangeAttractor> strangeAttractors; 

    public: 
        StrangeAttractorSystem(){
            strangeAttractors.setPoolSize(1000); 
        }

        void updateStrangeAttractors(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles){

            unsigned int maxAttractorCount = strangeAttractors.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxAttractorCount, &particles, &params](const int a, const int b){

                float timestep = params.timestep; 

                for(int i = 0; i < maxAttractorCount; i++){

                    if(!strangeAttractors.isInUse(i))
                        continue;          
               

                    if(strangeAttractors[i].falloff == Falloff::Constant){
                        for(int p = a; p < b; p++){
                            float xDiff = strangeAttractors[i].position.x - particles[p].position.x; 
                            float yDiff = strangeAttractors[i].position.y - particles[p].position.y;
                            float zDiff = strangeAttractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 
                            float distFactor; 
                            if(dist > strangeAttractors[i].maxDist)
                                distFactor = 0;
                            else
                                distFactor = 1; 

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * strangeAttractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * strangeAttractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * strangeAttractors[i].strength;
                        }
                    }
                    else if(strangeAttractors[i].falloff == Falloff::InvDist2){
                        for(int p = a; p < b; p++){
                            float xDiff = strangeAttractors[i].position.x - particles[p].position.x; 
                            float yDiff = strangeAttractors[i].position.y - particles[p].position.y;
                            float zDiff = strangeAttractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 
                            float distFactor; 
                            if(dist > strangeAttractors[i].maxDist)
                                distFactor = 0;
                            else if(dist < strangeAttractors[i].minDist)
                                distFactor = dist == 0 ? 0 : 1 / (strangeAttractors[i].minDist * strangeAttractors[i].minDist * dist);
                            else
                                distFactor = 1 / (dist2 * dist); 

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * strangeAttractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * strangeAttractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * strangeAttractors[i].strength;
                        }
                    }
                    else if(strangeAttractors[i].falloff == Falloff::InvDist){
                        for(int p = a; p < b; p++){
                            float xDiff = strangeAttractors[i].position.x - particles[p].position.x; 
                            float yDiff = strangeAttractors[i].position.y - particles[p].position.y;
                            float zDiff = strangeAttractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor; 
                            if(dist > strangeAttractors[i].maxDist)
                                distFactor = 0;
                            else if(dist < strangeAttractors[i].minDist)
                                distFactor = dist == 0 ? 0 : 1 / (strangeAttractors[i].minDist * dist);
                            else
                                distFactor = 1 / (dist2); 

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * strangeAttractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * strangeAttractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * strangeAttractors[i].strength;
                        }
                    }
                    else if(strangeAttractors[i].falloff == Falloff::InvDistWell){
                        for(int p = a; p < b; p++){
                            float xDiff = strangeAttractors[i].position.x - particles[p].position.x; 
                            float yDiff = strangeAttractors[i].position.y - particles[p].position.y;
                            float zDiff = strangeAttractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor = dist > strangeAttractors[i].maxDist || dist < strangeAttractors[i].minDist ? 0 : dist - strangeAttractors[i].minDist;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * strangeAttractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * strangeAttractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * strangeAttractors[i].strength;
                        }
                    }
                    else if(strangeAttractors[i].falloff == Falloff::InvDist2Well){
                        for(int p = a; p < b; p++){
                            float xDiff = strangeAttractors[i].position.x - particles[p].position.x; 
                            float yDiff = strangeAttractors[i].position.y - particles[p].position.y;
                            float zDiff = strangeAttractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff;
                            float maxDist2 = strangeAttractors[i].maxDist * strangeAttractors[i].maxDist; 
                            float minDist2 = strangeAttractors[i].minDist * strangeAttractors[i].minDist; 

                            float distFactor = dist2 > maxDist2 || dist2 < minDist2 ? 0 : dist2 - minDist2;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * strangeAttractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * strangeAttractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * strangeAttractors[i].strength;
                        }
                    }
                
                }
            }).wait();

        }

        int addStrangeAttrator(Vec3 position, Vec3 scale, StrangeAttractorType type, float strength, float minDist, float maxDist, Falloff falloff){
            StrangeAttractor a; 
            a.position = position;
            a.scale = scale;  
            a.type = type; 
            a.strength = strength;
            a.minDist = minDist;
            a.maxDist = maxDist; 
            a.falloff = falloff;
            return strangeAttractors.add(a);  
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

        void setStrangeAttractorPosition(int inx, Vec3 position){
            strangeAttractors[inx].position = position; 
        }

        void setStrangeAttractorScale(int inx, Vec3 scale){
            strangeAttractors[inx].scale = scale; 
        }

        void setStrangeAttractorType(int inx, StrangeAttractorType type){
            strangeAttractors[inx].type = type; 
        }

        void setStrangeAttractorStrength(int inx, float strength){
            strangeAttractors[inx].strength = strength; 
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
};