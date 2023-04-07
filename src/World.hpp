#pragma once 

#include <limits>
#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Util.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"
#include "NoiseGenerator.hpp"

#include "GlobalForceSystem.hpp"
#include "AttractorSystem.hpp"
#include "StrangeAttractorSystem.hpp"
#include "VortexSystem.hpp"
#include "NoiseFieldSystem.hpp"
#include "BoxColliderSystem.hpp"
#include "SphereColliderSystem.hpp"
#include "DamperSystem.hpp"
#include "RodSystem.hpp"
#include "AngleConstraintSystem.hpp"

class World {
    private:
        WorldParams params; 

        BS::thread_pool threadPool;

        float deltaT;
        float invDeltaT;

        DynamicPool<Particle> particles;
        DynamicPool<Vec3> particleDeltas; 
        DynamicPool<int> particleDeltaCount; 
        DynamicPool<uint32_t> particleLayerMask; 

        void updateCollisionBounds(){
            
            float hasCollisionFloor = params.hasCollisionFloor; 
            float collisionFloorHeight = params.collisionFloorHeight; 

            if(hasCollisionFloor){

                unsigned int maxParticleCount = particles.getBound();
                
                for (int i = 0; i < maxParticleCount; i++){
                    float py = particles[i].positionNext.y; 
                    float pen = collisionFloorHeight - py; 
                    particles[i].positionNext.y += pen > 0 ? pen : 0; 
                }                
            }
        }

    public:  

        GlobalForceSystem globalForceSystem;
        AttractorSystem attractorSystem; 
        StrangeAttractorSystem strangeAttractorSystem; 
        VortexSystem vortexSystem; 
        NoiseFieldSystem noiseFieldSystem; 
        BoxColliderSystem boxColliderSystem;
        SphereColliderSystem sphereColliderSystem; 
        DamperSystem damperSystem; 
        RodSystem rodSystem; 
        AngleConstraintSystem angleConstraintSystem;

        World(){
            params.timestep = 0.016666; 
            params.substeps = 1; 
            params.globalDamping = 0.01f;
            params.gravity = 9.81; 

            params.collisionFloorStaticFriction = 0; 
            params.collisionFloorKineticFriction = 0; 
            params.collisionFloorHeight = 0; 
            params.hasCollisionFloor = false;

            deltaT = params.timestep / params.substeps; 
            invDeltaT = 1.0f / deltaT; 
            
            particles.setPoolSize(100000); 
            particleDeltas.setPoolSize(100000); 
            particleDeltaCount.setPoolSize(100000); 
            particleLayerMask.setPoolSize(100000); 
        }

        void clear(){
            params.timestep = 0.016666; 
            params.substeps = 1; 
            params.globalDamping = 0.01f;
            params.gravity = 9.81; 

            params.collisionFloorStaticFriction = 0; 
            params.collisionFloorKineticFriction = 0; 
            params.collisionFloorHeight = 0;  
            params.hasCollisionFloor = false;

            deltaT = params.timestep / params.substeps; 
            invDeltaT = 1.0f / deltaT; 

            globalForceSystem.clearGlobalForces(); 
            attractorSystem.clearAttractors(); 
            strangeAttractorSystem.clearStrangeAttractors(); 
            vortexSystem.clearVortices(); 
            noiseFieldSystem.clearNoiseFields(); 
            boxColliderSystem.clearBoxColliders(); 
            sphereColliderSystem.clearSphereColliders(); 
            damperSystem.clearDampers(); 
            rodSystem.clearRods();
            rodSystem.clearAnchorRods(); 
            angleConstraintSystem.clearAngleConstraints(); 

            particles.clear(); 
            particleDeltas.clear(); 
            particleDeltaCount.clear(); 
            particleLayerMask.clear(); 
        }

        void update(){
            float timestep = params.timestep; 
            float substeps = params.substeps; 
            float gravity = params.gravity; 
            float globalDamping = params.globalDamping; 

            deltaT = params.timestep / params.substeps; 
            invDeltaT = 1.0f / deltaT;

            for(int s = 0; s < substeps; s++){
            
                unsigned int maxParticleCount = particles.getBound();
                
                //update gravity
                for(int i = 0; i < maxParticleCount; i++)
                    particles[i].velocity.y -= deltaT * gravity; 

                //Update forces 
                globalForceSystem.updateGlobalForces(threadPool, deltaT, params, particles, particleLayerMask); 
                attractorSystem.updateAttractors(threadPool, deltaT, params, particles, particleLayerMask); 
                vortexSystem.updateVortices(threadPool, deltaT, params, particles, particleLayerMask); 

                //Update fields
                strangeAttractorSystem.updateStrangeAttractors(threadPool, deltaT, params, particles, particleLayerMask); 
                noiseFieldSystem.updateNoiseFieldsForces(threadPool, deltaT, params, particles, particleLayerMask); 

                //update dampers
                damperSystem.updateDampers(threadPool, deltaT, params, particles, particleLayerMask); 

                //Apply velocities  
                for(int i = 0; i < maxParticleCount; i++){
                    particles[i].velocity.x -= particles[i].velocity.x * globalDamping * deltaT; 
                    particles[i].velocity.y -= particles[i].velocity.y * globalDamping * deltaT; 
                    particles[i].velocity.z -= particles[i].velocity.z * globalDamping * deltaT;

                    particles[i].positionNext.x = particles[i].position.x + deltaT * particles[i].velocity.x; 
                    particles[i].positionNext.y = particles[i].position.y + deltaT * particles[i].velocity.y;
                    particles[i].positionNext.z = particles[i].position.z + deltaT * particles[i].velocity.z;

                } 

                //Apply pre stabalization for collision constraints (any unsatisfied constraints from the previous timestep dont propogate through )
 
                //update colliders
                updateCollisionBounds(); 
                boxColliderSystem.updateBoxColliders(threadPool, params, particles, particleLayerMask); 
                sphereColliderSystem.updateSphereColliders(threadPool, params, particles, particleLayerMask); 

                //update rods 
                rodSystem.updateRods(threadPool, params, particles, particleDeltas, particleDeltaCount); 

                //update angle constraints
                //angleConstraintSystem.updateAngleConstraints(threadPool, params, particles, particleDeltas, particleDeltaCount); 

                //apply deltas   
                /*              
                for (int i = 0; i < maxParticleCount; i++){

                    int deltaCount = particleDeltaCount[i];
                    float deltaX = particleDeltas[i].x;
                    float deltaY = particleDeltas[i].y;
                    float deltaZ = particleDeltas[i].z;

                    if (deltaCount > 0){
                        
                        deltaX /= deltaCount;
                        deltaY /= deltaCount;
                        deltaZ /= deltaCount;

                        particles[i].positionNext.x += 1.5f * deltaX;
                        particles[i].positionNext.y += 1.5f * deltaY;
                        particles[i].positionNext.z += 1.5f * deltaZ;
                    }

                    particleDeltaCount[i] = 0;
                    particleDeltas[i].x = 0;
                    particleDeltas[i].y = 0;
                    particleDeltas[i].z = 0;
                }
                */

                //Apply New Position
                for(int i = 0; i < maxParticleCount; i++){

                    particles[i].velocity.x = (particles[i].positionNext.x - particles[i].position.x) * invDeltaT;
                    particles[i].velocity.y = (particles[i].positionNext.y - particles[i].position.y) * invDeltaT;
                    particles[i].velocity.z = (particles[i].positionNext.z - particles[i].position.z) * invDeltaT;

                    particles[i].position.x = particles[i].positionNext.x;
                    particles[i].position.y = particles[i].positionNext.y;
                    particles[i].position.z = particles[i].positionNext.z;

                } 
            }  
        }

        WorldParams* getWorldParamsPtr(){
            return &params; 
        }

        void setGravity(float grav){
            params.gravity = grav; 
        }

        void setTimestep(float t){
            params.timestep = t; 
            deltaT = params.timestep / params.substeps; 
            invDeltaT = 1.0f / deltaT; 
        }

        void setSubsteps(float s){
            params.substeps = s; 
            deltaT = params.timestep / params.substeps; 
            invDeltaT = 1.0f / deltaT; 
        }

        void setGlobalDamping(float d){
            params.globalDamping = d;
        }

        void setHasCollisionFloor(bool h){
            params.hasCollisionFloor = h; 
        }

        void setCollisionFloor(float height, float staticFriction, float kineticFriction){
            params.collisionFloorStaticFriction = staticFriction; 
            params.collisionFloorKineticFriction = kineticFriction; 
            params.collisionFloorHeight = height; 
        }


        int addParticle(Vec3 initialPosition, Vec3 initialVelocity, float invMass){
            Particle p; 
            p.position = initialPosition; 
            p.positionNext = initialPosition; 
            p.velocity = initialVelocity; 
            p.invMass = invMass; 

            Vec3 zero = Vec3(0, 0, 0); 
            particleDeltas.add(zero);

            int zeroInt = 0; 
            particleDeltaCount.add(zeroInt);

            uint32_t allOnes = 0xFFFFFFFF; 
            particleLayerMask.add(allOnes); 

            return particles.add(p); 
        }

        void destroyParticle(int inx){
            particles.remove(inx); 
            particleDeltas.remove(inx);
            particleDeltaCount.remove(inx);
            particleLayerMask.remove(inx); 
        }

        Particle* getParticlesPtr(){
            return particles.getDataPtr();
        }

        bool* getParticlesInUsePtr(){
            return particles.getInUsePtr(); 
        }
        
        int getParticlesPoolBounds(){
            return particles.getBound(); 
        }

        void resetParticlePosition(int inx, Vec3 position){
            particles[inx].position = position; 
            particles[inx].positionNext = position; 
        }

        void addParticleForce(int inx, Vec3 force){
            float t = params.timestep / params.substeps; 
            particles[inx].velocity.x += force.x * particles[inx].invMass * t; 
            particles[inx].velocity.y += force.y * particles[inx].invMass * t; 
            particles[inx].velocity.z += force.z * particles[inx].invMass * t; 
        }

        void addParticleImpulse(int inx, Vec3 impulse){
            particles[inx].velocity.x += impulse.x * particles[inx].invMass ; 
            particles[inx].velocity.y += impulse.y * particles[inx].invMass ; 
            particles[inx].velocity.z += impulse.z * particles[inx].invMass ; 
        }

        void addParticlePositionDelta(int inx, Vec3 delta){
            particleDeltas[inx].x += delta.x; 
            particleDeltas[inx].y += delta.y; 
            particleDeltas[inx].z += delta.z; 
        }

        void setParticleLayerEnabled(int inx, int layer, bool enabled){
            particleLayerMask[inx] = setBitVal<uint32_t>(particleLayerMask[inx], layer, enabled);  
        }

        void zeroParticleLayerMask(int inx){
            particleLayerMask[inx] = 0;
        }
}; 