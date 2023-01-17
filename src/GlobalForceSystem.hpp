#pragma once

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class GlobalForceSystem {
    private:
        DynamicPool<GlobalForce> globalForces;

    public:
        GlobalForceSystem(){
            globalForces.setPoolSize(1000); 
        }

        void updateGlobalForces(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles){

            unsigned int maxGlobalForceCount = globalForces.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxGlobalForceCount, &particles, &params](const int a, const int b){

                float timestep = params.timestep; 
                
                for(int i = 0; i < maxGlobalForceCount; i++){

                    if(!globalForces.isInUse(i))
                        continue; 

                    if(globalForces[i].boundShape == BoundShapeType::Infinite){
                    
                        for(int p = a; p < b; p++){

                            float xDiff = globalForces[i].direction.x; 
                            float yDiff = globalForces[i].direction.y;
                            float zDiff = globalForces[i].direction.z; 

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * globalForces[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * globalForces[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * globalForces[i].strength;
                        
                        }

                    }else if(globalForces[i].boundShape == BoundShapeType::Sphere){
                        

                    }

                }

            }).wait();
        }

        int addGlobalForce(Vec3 position, Vec3 direction, float strength, Vec3 boundSize, 
                           BoundShapeType boundShape, float boundThickness, Falloff boundFalloff){
            GlobalForce g; 
            g.position = position; 
            g.direction = direction; 
            g.strength = strength;
            g.boundSize = boundSize; 
            g.boundShape = boundShape;
            g.boundThickness = boundThickness; 
            g.boundFalloff = boundFalloff; 
            return globalForces.add(g);  
        }

        GlobalForce* getGlobalForcePtr(int inx){
            return &(globalForces[inx]); 
        }

        void destroyGlobalForce(int inx){
            globalForces.remove(inx); 
        }

        void clearGlobalForces(){
            globalForces.clear(); 
        }

        void setGlobalForcePosition(int inx, Vec3 position){
            globalForces[inx].position = position; 
        }

        void setGlobalForceDirection(int inx, Vec3 direction){
            globalForces[inx].direction = direction; 
        }

        void setGlobalForceStrength(int inx, float strength){
            globalForces[inx].strength = strength; 
        }

        void setGlobalForceBoundSize(int inx, Vec3 boundSize){
            globalForces[inx].boundSize = boundSize; 
        }

        void setGlobalForceBoundShape(int inx, BoundShapeType boundShape){
            globalForces[inx].boundShape = boundShape; 
        }

        void setGlobalForceBoundThickness(int inx, float boundThickness){
            globalForces[inx].boundThickness = boundThickness; 
        }

        void setGlobalForceBoundFalloff(int inx, Falloff boundFalloff){
            globalForces[inx].boundFalloff = boundFalloff; 
        }

        void setGlobalForceBoundInvRotation(int inx, Mat3 invRotation){
            globalForces[inx].boundInvRotation = invRotation; 
        }
}; 