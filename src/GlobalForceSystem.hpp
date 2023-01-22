#pragma once

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"
#include "IntersectionTesting.hpp"

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

                            float xDir = globalForces[i].direction.x; 
                            float yDir = globalForces[i].direction.y;
                            float zDir = globalForces[i].direction.z; 

                            particles[p].velocity.x += timestep * particles[p].invMass * xDir * globalForces[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDir * globalForces[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDir * globalForces[i].strength;
                        
                        }

                    }else if(globalForces[i].boundShape == BoundShapeType::Sphere){
                        
                        for(int p = a; p < b; p++){

                            float boundStrength = getSphereBoundStrength( 
                                particles[p].positionNext, 
                                globalForces[i].position, 
                                globalForces[i].boundSize, 
                                globalForces[i].boundShape, 
                                globalForces[i].boundThickness, 
                                globalForces[i].boundFalloff);
                            
                            float xDir = globalForces[i].direction.x; 
                            float yDir = globalForces[i].direction.y;
                            float zDir = globalForces[i].direction.z; 

                            particles[p].velocity.x += timestep * particles[p].invMass * xDir * globalForces[i].strength * boundStrength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDir * globalForces[i].strength * boundStrength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDir * globalForces[i].strength * boundStrength;
                        
                        }

                    }else if(globalForces[i].boundShape == BoundShapeType::Box){
                        
                        for(int p = a; p < b; p++){

                            float boundStrength = getBoxBoundStrength( 
                                particles[p].positionNext, 
                                globalForces[i].position, 
                                globalForces[i].boundSize, 
                                globalForces[i].boundShape, 
                                globalForces[i].boundThickness, 
                                globalForces[i].boundFalloff, 
                                globalForces[i].boundInvRotation); 

                            float xDir = globalForces[i].direction.x; 
                            float yDir = globalForces[i].direction.y;
                            float zDir = globalForces[i].direction.z; 

                            particles[p].velocity.x += timestep * particles[p].invMass * xDir * globalForces[i].strength * boundStrength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDir * globalForces[i].strength * boundStrength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDir * globalForces[i].strength * boundStrength;
                        }
                    }

                }

            }).wait();
        }

        int addGlobalForce(Vec3 position, Vec3 direction, float strength, Vec3 boundSize, 
                           BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation){
            GlobalForce g; 
            g.position = position; 
            g.direction = direction; 
            g.strength = strength;
            g.boundSize = boundSize; 
            g.boundShape = boundShape;
            g.boundThickness = boundThickness; 
            g.boundFalloff = boundFalloff; 
            g.boundInvRotation = boundInvRotation; 
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

        void setGlobalForceBoundFalloff(int inx, BoundFalloff boundFalloff){
            globalForces[inx].boundFalloff = boundFalloff; 
        }

        void setGlobalForceBoundInvRotation(int inx, Mat3 invRotation){
            globalForces[inx].boundInvRotation = invRotation; 
        }
}; 