#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Util.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class SphereColliderSystem {
    private:
        DynamicPool<SphereCollider> sphereColliders;  

    public: 
        SphereColliderSystem(){
            sphereColliders.setPoolSize(1000);
        }

        void updateSphereColliders(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles, DynamicPool<uint32_t>& particleLayerMask){
            
            unsigned int maxColliderCount = sphereColliders.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxColliderCount, &particles, &params, &particleLayerMask](const int a, const int b){
                
                for(int i = 0; i < maxColliderCount; i++){

                    if(!sphereColliders.isInUse(i))
                            continue; 

                    uint32_t M = sphereColliders[i].layerMask;

                    for(int p = a; p < b; p++){

                        if(!(M & particleLayerMask[p]))
                            continue;
                    
                        float xDiff = sphereColliders[i].position.x - particles[p].positionNext.x; 
                        float yDiff = sphereColliders[i].position.y - particles[p].positionNext.y;
                        float zDiff = sphereColliders[i].position.z - particles[p].positionNext.z;

                        float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                        float dist = sqrt(dist2); 
                        float pen = sphereColliders[i].radius - dist; 

                        float xDiffNorm = xDiff / dist; 
                        float yDiffNorm = yDiff / dist; 
                        float zDiffNorm = zDiff / dist; 

                        float dispX = xDiffNorm * pen;
                        float dispY = yDiffNorm * pen; 
                        float dispZ = zDiffNorm * pen; 

                        if(pen > 0){
                            particles[p].positionNext.x -= dispX;
                            particles[p].positionNext.y -= dispY;
                            particles[p].positionNext.z -= dispZ;
                        }
                    }
                }

            }).wait();
        }

        int addSphereCollider(Vec3 position, float radius, float kineticFriction, float staticFriction, bool inverse){
            SphereCollider c; 
            c.layerMask = 0xFFFFFFFF; 
            c.position = position; 
            c.radius = radius;
            c.kineticFriction = kineticFriction;
            c.staticFriction = staticFriction; 
            c.inverse = inverse;
            return sphereColliders.add(c);  
        }

        SphereCollider* getSphereColliderPtr(int inx){
            return &(sphereColliders[inx]); 
        }

        void destroySphereCollider(int inx){
            sphereColliders.remove(inx); 
        }

        void clearSphereColliders(){
            sphereColliders.clear(); 
        }

        void setSphereColliderLayerEnabled(int inx, int layer, bool enabled){
            sphereColliders[inx].layerMask = setBitVal(sphereColliders[inx].layerMask, layer, enabled); 
        }

        void zeroSphereColliderLayerMask(int inx){
            sphereColliders[inx].layerMask = 0; 
        }

        void setSphereColliderPosition(int inx, Vec3 position){
            sphereColliders[inx].position = position; 
        }

        void setSphereColliderRadius(int inx, float radius){
            sphereColliders[inx].radius = radius; 
        }

        void setSphereColliderKineticFriction(int inx, float kineticFriction){
            sphereColliders[inx].kineticFriction = kineticFriction; 
        }

        void setSphereColliderStaticFriction(int inx, float staticFriction){
            sphereColliders[inx].staticFriction = staticFriction; 
        }

        void setSphereColliderInverse(int inx, bool inverse){
            sphereColliders[inx].inverse = inverse; 
        }
};