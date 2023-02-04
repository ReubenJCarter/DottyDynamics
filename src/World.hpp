#pragma once 

#include <limits>
#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
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

class World {
    private:
        WorldParams params; 

        BS::thread_pool threadPool;

        DynamicPool<Particle> particles;
        DynamicPool<Vec3> particleDaltas; 
        DynamicPool<int> particleDeltaCount; 

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

        World(){
            params.timestep = 0.016666; 
            params.substeps = 1; 
            params.globalDamping = 0.01f;
            params.gravity = 9.81; 

            params.collisionFloorStaticFriction = 0; 
            params.collisionFloorKineticFriction = 0; 
            params.collisionFloorHeight = 0; 
            params.hasCollisionFloor = false;
            
            particles.setPoolSize(100000); 
            particleDaltas.setPoolSize(100000); 
            particleDeltaCount.setPoolSize(100000); 
        }

        void update(){
            float timestep = params.timestep; 
            float substeps = params.substeps; 
            float gravity = params.gravity; 
            float globalDamping = params.globalDamping; 

            float deltaT = timestep / substeps; 
            float invDeltaT = 1.0f / deltaT; 

            for(int s = 0; s < substeps; s++){
            
                unsigned int maxParticleCount = particles.getBound();
                
                //update gravity
                for(int i = 0; i < maxParticleCount; i++)
                    particles[i].velocity.y -= deltaT * gravity; 

                //Update forces 
                globalForceSystem.updateGlobalForces(threadPool, params, particles); 
                attractorSystem.updateAttractors(threadPool, params, particles); 
                vortexSystem.updateVortices(threadPool, params, particles); 

                //Update fields
                strangeAttractorSystem.updateStrangeAttractors(threadPool, params, particles); 
                noiseFieldSystem.updateNoiseFieldsForces(threadPool, params, particles); 

                //update dampers
                damperSystem.updateDampers(threadPool, params, particles); 

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
                boxColliderSystem.updateBoxColliders(threadPool, params, particles); 
                sphereColliderSystem.updateSphereColliders(threadPool, params, particles); 

                //update rods 
                rodSystem.updateRods(threadPool, params, particles, particleDaltas, particleDeltaCount); 

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
        }

        void setSubsteps(float s){
            params.substeps = s; 
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
            particleDaltas.add(zero);

            int zeroInt = 0; 
            particleDeltaCount.add(zeroInt);

            return particles.add(p); 
        }

        void destroyParticle(int inx){
            particles.remove(inx); 
            particleDaltas.remove(inx);
            particleDeltaCount.remove(inx);
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
      
}; 