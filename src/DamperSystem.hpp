#pragma once

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Util.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"
#include "IntersectionTesting.hpp"

class DamperSystem {
    private:
        DynamicPool<Damper> dampers;

    public:
        DamperSystem(){
            dampers.setPoolSize(1000); 
        }

        void updateDampers(BS::thread_pool& threadPool, float timestep, WorldParams& params, DynamicPool<Particle>& particles, DynamicPool<uint32_t>& particleLayerMask){

            unsigned int maxDampersCount = dampers.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxDampersCount, timestep, &particles, &particleLayerMask](const int a, const int b){
                
                for(int i = 0; i < maxDampersCount; i++){

                    if(!dampers.isInUse(i))
                        continue; 

                    uint32_t M = dampers[i].layerMask;

                    if(dampers[i].boundShape == BoundShapeType::Infinite){
                    
                        for(int p = a; p < b; p++){

                            float strength = M & particleLayerMask[p] ? dampers[i].strength : 0; 

                            particles[p].velocity.x -= particles[p].velocity.x * timestep * particles[p].invMass * dampers[i].strength;
                            particles[p].velocity.y -= particles[p].velocity.y * timestep * particles[p].invMass * dampers[i].strength;
                            particles[p].velocity.z -= particles[p].velocity.z * timestep * particles[p].invMass * dampers[i].strength;
                        }

                    }else if(dampers[i].boundShape == BoundShapeType::Sphere){
                        
                        for(int p = a; p < b; p++){

                            float boundStrength = getSphereBoundStrength( 
                                particles[p].positionNext, 
                                dampers[i].position, 
                                dampers[i].boundSize, 
                                dampers[i].boundShape, 
                                dampers[i].boundThickness, 
                                dampers[i].boundFalloff);

                            boundStrength = M & particleLayerMask[p] ? boundStrength : 0; 

                            particles[p].velocity.x -= particles[p].velocity.x * timestep * particles[p].invMass * dampers[i].strength * boundStrength;
                            particles[p].velocity.y -= particles[p].velocity.y * timestep * particles[p].invMass * dampers[i].strength * boundStrength;
                            particles[p].velocity.z -= particles[p].velocity.z * timestep * particles[p].invMass * dampers[i].strength * boundStrength;
                        }

                    }else if(dampers[i].boundShape == BoundShapeType::Box){
                        
                        for(int p = a; p < b; p++){

                            float boundStrength = getBoxBoundStrength( 
                                particles[p].positionNext, 
                                dampers[i].position, 
                                dampers[i].boundSize, 
                                dampers[i].boundShape, 
                                dampers[i].boundThickness, 
                                dampers[i].boundFalloff, 
                                dampers[i].boundInvRotation); 

                            boundStrength = M & particleLayerMask[p] ? boundStrength : 0;

                            particles[p].velocity.x -= particles[p].velocity.x * timestep * particles[p].invMass * dampers[i].strength * boundStrength;
                            particles[p].velocity.y -= particles[p].velocity.y * timestep * particles[p].invMass * dampers[i].strength * boundStrength;
                            particles[p].velocity.z -= particles[p].velocity.z * timestep * particles[p].invMass * dampers[i].strength * boundStrength;
                        }
                    }

                }

            }).wait();
        }

        int addDamper(Vec3 position, float strength, Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation){
            Damper d; 
            d.layerMask = 0xFFFFFFFF; 
            d.position = position; 
            d.strength = strength;
            d.boundSize = boundSize; 
            d.boundShape = boundShape;
            d.boundThickness = boundThickness; 
            d.boundFalloff = boundFalloff; 
            d.boundInvRotation = boundInvRotation; 
            return dampers.add(d);  
        }

        Damper* getDamperPtr(int inx){
            return &(dampers[inx]); 
        }

        void destroyDamper(int inx){
            dampers.remove(inx); 
        }

        void clearDampers(){
            dampers.clear(); 
        }

        void setDamperLayerEnabled(int inx, int layer, bool enabled){
            dampers[inx].layerMask = setBitVal(dampers[inx].layerMask, layer, enabled); 
        }

        void zeroDamperLayerMask(int inx){
            dampers[inx].layerMask = 0; 
        }

        void setDamperPosition(int inx, Vec3 position){
            dampers[inx].position = position; 
        }

        void setDamperStrength(int inx, float strength){
            dampers[inx].strength = strength; 
        }

        void setDamperBoundSize(int inx, Vec3 boundSize){
            dampers[inx].boundSize = boundSize; 
        }

        void setDamperBoundShape(int inx, BoundShapeType boundShape){
            dampers[inx].boundShape = boundShape; 
        }

        void setDamperBoundThickness(int inx, float boundThickness){
            dampers[inx].boundThickness = boundThickness; 
        }

        void setDamperBoundFalloff(int inx, BoundFalloff boundFalloff){
            dampers[inx].boundFalloff = boundFalloff; 
        }

        void setDamperBoundInvRotation(int inx, Mat3 invRotation){
            dampers[inx].boundInvRotation = invRotation; 
        }
}; 