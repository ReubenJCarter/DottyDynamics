#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class BoxColliderSystem {
    private:
        DynamicPool<BoxCollider> boxColliders;

    public: 
        BoxColliderSystem(){
            boxColliders.setPoolSize(1000); 
        }

        void updateBoxColliders(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles){
            unsigned int maxColliderCount = boxColliders.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxColliderCount, &particles, &params](const int a, const int b){
                
                for(int i = 0; i < maxColliderCount; i++){

                    if(!boxColliders.isInUse(i))
                            continue; 

                    for(int p = a; p < b; p++){

                        //Transform point into correct space for test 
                        Vec3 ppos = particles[p].position;
                        ppos.sub(boxColliders[i].position); 
                        ppos.multm(boxColliders[i].invRotation); 

                        //Test if outside to box  
                        float halfsx = boxColliders[i].size.x/2;
                        float halfsy = boxColliders[i].size.y/2;
                        float halfsz = boxColliders[i].size.z/2;

                        if(ppos.x > halfsx || ppos.x < -halfsx || ppos.y > halfsy || ppos.y < -halfsy || ppos.z > halfsz || ppos.z < -halfsz){
                            continue; 
                        }

                        //if the particle is in the box, find the side it is closest to 

                    }
                }

            }).wait();
        }

        int addBoxCollider(Vec3 position, Mat3 invRotation, Vec3 size, float kineticFriction, float staticFriction, bool inverse){
            BoxCollider c; 
            c.position = position; 
            c.invRotation = invRotation; 
            c.size = size;
            c.kineticFriction = kineticFriction;
            c.staticFriction = staticFriction; 
            c.inverse = inverse;
            return boxColliders.add(c);  
        }

        BoxCollider* getBoxColliderPtr(int inx){
            return &(boxColliders[inx]); 
        }

        void destroyBoxCollider(int inx){
            boxColliders.remove(inx); 
        }

        void clearBoxColliders(){
            boxColliders.clear(); 
        }

        void setBoxColliderPosition(int inx, Vec3 position){
            boxColliders[inx].position = position; 
        }

        void setBoxColliderInvRotation(int inx, Mat3 invRotation){
            boxColliders[inx].invRotation = invRotation; 
        }

        void setBoxColliderSize(int inx, Vec3 size){
            boxColliders[inx].size = size; 
        }

        void setBoxColliderKineticFriction(int inx, float kineticFriction){
            boxColliders[inx].kineticFriction = kineticFriction; 
        }

        void setBoxColliderStaticFriction(int inx, float staticFriction){
            boxColliders[inx].staticFriction = staticFriction; 
        }

        void setBoxColliderInverse(int inx, bool inverse){
            boxColliders[inx].inverse = inverse; 
        }
};