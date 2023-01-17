#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class VortexSystem {
    private:
        DynamicPool<Vortex> vortices;

    public: 
        VortexSystem(){
            vortices.setPoolSize(1000); 
        }

        void updateVortices(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles){

            unsigned int maxVorticesCount = vortices.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxVorticesCount, &particles, &params](const int a, const int b){
                
                float timestep = params.timestep; 
                
                for(int i = 0; i < maxVorticesCount; i++){

                    if(!vortices.isInUse(i))
                        continue; 

                    //if(vortices[i].falloff == Falloff::InvDist2){
                        for(int p = a; p < b; p++){
                            float xDiff = vortices[i].position.x - particles[p].position.x; 
                            float yDiff = vortices[i].position.y - particles[p].position.y;
                            float zDiff = vortices[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor; 
                            if(dist > vortices[i].maxDist)
                                distFactor = 0;
                            else if(dist < vortices[i].minDist)
                                distFactor = dist == 0 ? 0 : 1 / (vortices[i].minDist * vortices[i].minDist);
                            else
                                distFactor = 1 / (dist2); 

                            float vx = vortices[i].normal.x * vortices[i].strength;  
                            float vy = vortices[i].normal.y * vortices[i].strength;
                            float vz = vortices[i].normal.z * vortices[i].strength;

                            float cx = vy * zDiff - vz * yDiff; 
	                        float cy = vz * xDiff - vx * zDiff; 
	                        float cz = vx * yDiff - vy * xDiff; 

                            particles[p].velocity.x += timestep * particles[p].invMass * cx * distFactor;
                            particles[p].velocity.y += timestep * particles[p].invMass * cy * distFactor;
                            particles[p].velocity.z += timestep * particles[p].invMass * cz * distFactor;
                        }
                    //}
                }

            }).wait();

        }

        int addVortex(Vec3 position, Vec3 normal, float strength, float minDist, float maxDist, Falloff falloff){
            Vortex v; 
            v.position = position; 
            v.normal = normal;
            v.strength = strength;
            v.minDist = minDist;
            v.maxDist = maxDist; 
            v.falloff = falloff;
            return vortices.add(v);  
        }

        Vortex* getVortexPtr(int inx){
            return &(vortices[inx]);
        }

        void destroyVortex(int inx){
            vortices.remove(inx); 
        }

        void clearVortices(){
            vortices.clear(); 
        }

        void setVortexPosition(int inx, Vec3 position){
            vortices[inx].position = position; 
        }

        void setVortexNormal(int inx, Vec3 normal){
            vortices[inx].normal = normal; 
        }

        void setVortexStrength(int inx, float strength){
            vortices[inx].strength = strength; 
        }

        void setVortexMinDist(int inx, float minDist){
            vortices[inx].minDist = minDist; 
        }

        void setVortexMaxDist(int inx, float maxDist){
            vortices[inx].maxDist = maxDist; 
        }

        void setVortexFalloff(int inx, Falloff falloff){
            vortices[inx].falloff = falloff; 
        }
};