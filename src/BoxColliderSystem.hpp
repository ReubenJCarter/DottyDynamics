#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class BoxColliderSystem {
    private:
        enum BoxSide{xp, xn, yp, yn, zp, zn};
        DynamicPool<BoxCollider> boxColliders;

    public: 
        BoxColliderSystem(){
            boxColliders.setPoolSize(1000); 
        }

        void updateBoxColliders(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles){
            unsigned int maxColliderCount = boxColliders.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxColliderCount, &particles](const int a, const int b){
                
                for(int i = 0; i < maxColliderCount; i++){

                    if(!boxColliders.isInUse(i))
                            continue; 

                    //get normal matrix from invRotation
                    Mat3 normalMatrix = boxColliders[i].invRotation; 
                    normalMatrix.inv(); 

                    //compute normals for each side 
                    Vec3 sideNormals[6]; 
                    sideNormals[xp] = Vec3(1, 0, 0).multm(normalMatrix); 
                    sideNormals[xn] = Vec3(-1, 0, 0).multm(normalMatrix); 
                    sideNormals[yp] = Vec3(0, 1, 0).multm(normalMatrix); 
                    sideNormals[yn] = Vec3(0, -1, 0).multm(normalMatrix); 
                    sideNormals[zp] = Vec3(0, 0, 1).multm(normalMatrix); 
                    sideNormals[zn] = Vec3(0, 0, -1).multm(normalMatrix); 

                    for(int p = a; p < b; p++){

                        //Transform point into correct space for test 
                        Vec3 ppos = particles[p].positionNext;
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
                        float dxp = halfsx - ppos.x; 
                        float dxn = ppos.x + halfsx;
                        float dyp = halfsy - ppos.y; 
                        float dyn = ppos.y + halfsy;
                        float dzp = halfsz - ppos.z; 
                        float dzn = ppos.z + halfsz;

                        float mind = dxp; 
                        int minSide = BoxSide::xp; 
                        if(dxn < mind){
                            mind = dxn; 
                            minSide = xn; 
                        }
                        if(dyp < mind){
                            mind = dyp; 
                            minSide = yp; 
                        }
                        if(dyn < mind){
                            mind = dyn; 
                            minSide = yn; 
                        }
                        if(dzp < mind){
                            mind = dzp; 
                            minSide = zp; 
                        }
                        if(dzn < mind){
                            mind = dzn; 
                            minSide = zn; 
                        }

                        //project the particle onto the closest side 
                        Vec3 norm = sideNormals[minSide]; 
                        particles[p].positionNext.x += norm.x * mind;
                        particles[p].positionNext.y += norm.y * mind;
                        particles[p].positionNext.z += norm.z * mind;
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