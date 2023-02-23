#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Util.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class VortexSystem {
    private:
        DynamicPool<Vortex> vortices;

    public: 
        VortexSystem(){
            vortices.setPoolSize(1000); 
        }

        void updateVortices(BS::thread_pool& threadPool, float timestep, WorldParams& params, DynamicPool<Particle>& particles){

            unsigned int maxVorticesCount = vortices.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxVorticesCount, timestep, &particles](const int a, const int b){
                                
                for(int i = 0; i < maxVorticesCount; i++){

                    if(!vortices.isInUse(i))
                        continue; 

                    for(int p = a; p < b; p++){
                        float xDiff = vortices[i].position.x - particles[p].position.x; 
                        float yDiff = vortices[i].position.y - particles[p].position.y;
                        float zDiff = vortices[i].position.z - particles[p].position.z;

                        float distFactor; 
                        if(vortices[i].falloff == Falloff::Constant)
                            distFactor = falloffConstant(xDiff, yDiff, zDiff, vortices[i].maxDist); 
                        else if(vortices[i].falloff == Falloff::InvDist2) 
                            distFactor = falloffInvDist2(xDiff, yDiff, zDiff, vortices[i].minDist, vortices[i].maxDist); 
                        else if(vortices[i].falloff == Falloff::InvDist) 
                            distFactor = falloffInvDist(xDiff, yDiff, zDiff, vortices[i].minDist, vortices[i].maxDist); 
                        else if(vortices[i].falloff == Falloff::InvDist2Well) 
                            distFactor = falloffInvDist2Well(xDiff, yDiff, zDiff, vortices[i].minDist, vortices[i].maxDist); 
                        else if(vortices[i].falloff == Falloff::InvDistWell) 
                            distFactor = falloffInvDistWell(xDiff, yDiff, zDiff, vortices[i].minDist, vortices[i].maxDist); 

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

        void setVortexLayerEnabled(int inx, int layer, bool enabled){
            vortices[inx].layerMask = setBitVal(vortices[inx].layerMask, layer, enabled); 
        }

        void zeroVortexLayerMask(int inx){
            vortices[inx].layerMask = 0; 
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