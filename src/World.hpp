#pragma once 

#include <limits>
#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"
#include "NoiseGenerator.hpp"


class World {
    private:
        BS::thread_pool threadPool;

        float timestep; 
        int substeps; 
        float globalDamping; 
        float gravity;
        float collisionFloorStaticFriction; 
        float collisionFloorKineticFriction; 
        float collisionFloorHeight; 
        bool hasCollisionFloor;
        DynamicPool<Particle> particles;
        DynamicPool<Rod> rods;
        DynamicPool<AnchorRod> anchorRods;
        DynamicPool<Vec3> rodDeltas; 
        DynamicPool<int> rodDeltaCount; 
        DynamicPool<Attractor> attractors;
        DynamicPool<Vortex> vortices;
        DynamicPool<Vec3> globalForces;
        DynamicPool<NoiseField> noiseFields;   
        DynamicPool<NoiseGenerator> noiseGenerators;    
        DynamicPool<BoxCollider> boxColliders;    
        DynamicPool<SphereCollider> sphereColliders;    
         

        void updateGlobalForces(){

            unsigned int maxGlobalForceCount = globalForces.getBound(); 

            threadPool.parallelize_loop(maxGlobalForceCount, [this](const int a, const int b){
                
                unsigned int pcount = particles.getBound();
                
                for(int i = a; i < b; i++){

                    if(!globalForces.isInUse(i))
                        continue; 

                    float xDiff = globalForces[i].x; 
                    float yDiff = globalForces[i].y;
                    float zDiff = globalForces[i].z; 

                    for(int p = 0; p < pcount; p++){
                        particles[p].velocity.x += timestep * particles[p].invMass * xDiff;
                        particles[p].velocity.y += timestep * particles[p].invMass * yDiff;
                        particles[p].velocity.z += timestep * particles[p].invMass * zDiff;
                    }
                }

            }).wait();
        }

        void updateAttractors(){

            unsigned int maxAttractorCount = attractors.getBound(); 

            threadPool.parallelize_loop(maxAttractorCount, [this](const int a, const int b){
                
                unsigned int pcount = particles.getBound();
            
                for(int i = a; i < b; i++){

                    if(!attractors.isInUse(i))
                      continue; 

                    if(attractors[i].falloff == Falloff::InvDist2){
                        for(int p = 0; p < pcount; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor; 
                            if(dist > attractors[i].maxDist)
                                distFactor = 0;
                            else if(dist < attractors[i].minDist)
                                distFactor = dist == 0 ? 0 : 1 / (attractors[i].minDist * attractors[i].minDist * dist);
                            else
                                distFactor = 1 / (dist2 * dist); 

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::InvDist){
                        for(int p = 0; p < pcount; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor; 
                            if(dist > attractors[i].maxDist)
                                distFactor = 0;
                            else if(dist < attractors[i].minDist)
                                distFactor = dist == 0 ? 0 : 1 / (attractors[i].minDist * dist);
                            else
                                distFactor = 1 / (dist2); 

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::InvDistWell){
                        for(int p = 0; p < pcount; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor = dist > attractors[i].maxDist || dist < attractors[i].minDist ? 0 : dist - attractors[i].minDist;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::InvDist2Well){
                        for(int p = 0; p < pcount; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff;
                            float maxDist2 = attractors[i].maxDist * attractors[i].maxDist; 
                            float minDist2 = attractors[i].minDist * attractors[i].minDist; 

                            float distFactor = dist2 > maxDist2 || dist2 < minDist2 ? 0 : dist2 - minDist2;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                }

            }).wait();

        }

        void updateVortices(){

            unsigned int maxVorticesCount = vortices.getBound(); 

            threadPool.parallelize_loop(maxVorticesCount, [this](const int a, const int b){
                
                unsigned int pcount = particles.getBound();
                
                for(int i = a; i < b; i++){

                    if(!vortices.isInUse(i))
                        continue; 

                    if(vortices[i].falloff == Falloff::InvDist2){
                        for(int p = 0; p < pcount; p++){
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
                    }
                }

            }).wait();

        }

        void updateNoiseFieldsForces(){
            unsigned int noiseFieldsCount = noiseFields.getBound(); 

            threadPool.parallelize_loop(noiseFieldsCount, [this](const int a, const int b){
                
                unsigned int pcount = particles.getBound();
                
                for(int i = a; i < b; i++){

                    if(!noiseFields.isInUse(i))
                        continue; 

                    if(!noiseFields[i].isVelocity){
                        if(noiseFields[i].noiseType == NoiseType::Simplex || noiseFields[i].noiseType == NoiseType::Perlin || noiseFields[i].noiseType == NoiseType::Value){
                            for(int p = 0; p < pcount; p++){

                                Vec3 coord = particles[p].position; 
                                coord.mults(noiseFields[i].noiseScale); 
                                
                                Vec3 n = noiseGenerators[i].get3(coord);

                                float fX = n.x * noiseFields[i].strength; 
                                float fY = n.y * noiseFields[i].strength; 
                                float fZ = n.z * noiseFields[i].strength; 
                                
                                particles[p].velocity.x += timestep * particles[p].invMass * fX;
                                particles[p].velocity.y += timestep * particles[p].invMass * fY;
                                particles[p].velocity.z += timestep * particles[p].invMass * fZ;
                            }
                        }
                        else if(noiseFields[i].noiseType == NoiseType::SimplexCurl || noiseFields[i].noiseType == NoiseType::PerlinCurl || noiseFields[i].noiseType == NoiseType::ValueCurl){
                            for(int p = 0; p < pcount; p++){

                                Vec3 coord = particles[p].position; 
                                coord.mults(noiseFields[i].noiseScale); 
                                
                                Vec3 n = noiseGenerators[i].curl(coord); 

                                float fX = n.x * noiseFields[i].strength; 
                                float fY = n.y * noiseFields[i].strength; 
                                float fZ = n.z * noiseFields[i].strength; 
                                
                                particles[p].velocity.x += timestep * particles[p].invMass * fX;
                                particles[p].velocity.y += timestep * particles[p].invMass * fY;
                                particles[p].velocity.z += timestep * particles[p].invMass * fZ;
                            }
                        }
                    }
                    else{
                        if(noiseFields[i].noiseType == NoiseType::Simplex || noiseFields[i].noiseType == NoiseType::Perlin || noiseFields[i].noiseType == NoiseType::Value){
                            for(int p = 0; p < pcount; p++){

                                Vec3 coord = particles[p].position; 
                                coord.mults(noiseFields[i].noiseScale); 
                                
                                Vec3 n = noiseGenerators[i].get3(coord);
                                
                                float targetVelocityX = n.x * noiseFields[i].strength; 
                                float targetVelocityY = n.y * noiseFields[i].strength; 
                                float targetVelocityZ = n.z * noiseFields[i].strength; 
                                float verrX = targetVelocityX - particles[p].velocity.x; 
                                float verrY = targetVelocityY - particles[p].velocity.y; 
                                float verrZ = targetVelocityZ - particles[p].velocity.z;
                                float kp =  noiseFields[i].viscosity;
                                float correctingForceX = verrX * kp; 
                                float correctingForceY = verrY * kp;
                                float correctingForceZ = verrZ * kp;

                                particles[p].velocity.x += timestep * particles[p].invMass * correctingForceX;
                                particles[p].velocity.y += timestep * particles[p].invMass * correctingForceY;
                                particles[p].velocity.z += timestep * particles[p].invMass * correctingForceZ;
                            }
                        }
                        else if(noiseFields[i].noiseType == NoiseType::SimplexCurl || noiseFields[i].noiseType == NoiseType::PerlinCurl || noiseFields[i].noiseType == NoiseType::ValueCurl){
                            for(int p = 0; p < pcount; p++){

                                Vec3 coord = particles[p].position; 
                                coord.mults(noiseFields[i].noiseScale); 
                                
                                Vec3 n = noiseGenerators[i].curl(coord); 
                                
                                float targetVelocityX = n.x * noiseFields[i].strength; 
                                float targetVelocityY = n.y * noiseFields[i].strength; 
                                float targetVelocityZ = n.z * noiseFields[i].strength; 
                                float verrX = targetVelocityX - particles[p].velocity.x; 
                                float verrY = targetVelocityY - particles[p].velocity.y; 
                                float verrZ = targetVelocityZ - particles[p].velocity.z;
                                float kp =  noiseFields[i].viscosity;
                                float correctingForceX = verrX * kp; 
                                float correctingForceY = verrY * kp;
                                float correctingForceZ = verrZ * kp;

                                particles[p].velocity.x += timestep * particles[p].invMass * correctingForceX;
                                particles[p].velocity.y += timestep * particles[p].invMass * correctingForceY;
                                particles[p].velocity.z += timestep * particles[p].invMass * correctingForceZ;
                            }
                        }
                    }
                  
                }

            }).wait();
        }

        void updateRods(){
            
            unsigned int maxRodCount = rods.getBound(); 

            for(int i = 0; i < maxRodCount; i++){
                
                float invMassA = particles[rods[i].a].invMass;
                float invMassB = particles[rods[i].b].invMass;
                float dx = (particles[rods[i].a].positionNext.x - particles[rods[i].b].positionNext.x);
                float dy = (particles[rods[i].a].positionNext.y - particles[rods[i].b].positionNext.y);
                float dz = (particles[rods[i].a].positionNext.z - particles[rods[i].b].positionNext.z);

                float diffLen2 = dx * dx + dy * dy + dz * dz;
                float diffLen = diffLen2 == 0 ? 1 : sqrt(diffLen2);
                float rodDiffLen = diffLen - rods[i].length;

                float factor = (rods[i].stiffness * rodDiffLen) / (diffLen * (invMassA + invMassB));

                dx *= factor;
                dy *= factor;
                dz *= factor;
                
                if(rods.isInUse(i)){
                    rodDeltas[rods[i].a].x -= invMassA * dx;
                    rodDeltas[rods[i].a].y -= invMassA * dy;
                    rodDeltas[rods[i].a].z -= invMassA * dz;
                    
                    rodDeltas[rods[i].b].x += invMassB * dx;
                    rodDeltas[rods[i].b].y += invMassB * dy;
                    rodDeltas[rods[i].b].z += invMassB * dz;

                    rodDeltaCount[rods[i].a]++;
                    rodDeltaCount[rods[i].b]++;
                }
            }


            unsigned int maxAnchorRodCount = anchorRods.getBound(); 

            for(int i = 0; i < maxAnchorRodCount; i++){
                
                float invMassA = particles[anchorRods[i].a].invMass;
                float dx = (particles[anchorRods[i].a].positionNext.x - anchorRods[i].position.x);
                float dy = (particles[anchorRods[i].a].positionNext.y - anchorRods[i].position.y);
                float dz = (particles[anchorRods[i].a].positionNext.z - anchorRods[i].position.z);

                float diffLen2 = dx * dx + dy * dy + dz * dz;
                float diffLen = diffLen2 == 0 ? 1 : sqrt(diffLen2);
                float rodDiffLen = diffLen - anchorRods[i].length;

                float factor = (anchorRods[i].stiffness * rodDiffLen) / (diffLen * (invMassA));

                dx *= factor;
                dy *= factor;
                dz *= factor;
                
                if(anchorRods.isInUse(i)){
                    rodDeltas[anchorRods[i].a].x -= invMassA * dx;
                    rodDeltas[anchorRods[i].a].y -= invMassA * dy;
                    rodDeltas[anchorRods[i].a].z -= invMassA * dz;

                    rodDeltaCount[anchorRods[i].a]++;
                }
            }


            unsigned int maxParticleCount = particles.getBound();
            
            for (int i = 0; i < maxParticleCount; i++){

                int deltaCount = rodDeltaCount[i];
                float deltaX = rodDeltas[i].x;
                float deltaY = rodDeltas[i].y;
                float deltaZ = rodDeltas[i].z;

                if (deltaCount > 0){
                    
                    deltaX /= deltaCount;
                    deltaY /= deltaCount;
                    deltaZ /= deltaCount;

                    particles[i].positionNext.x += 1.5f * deltaX;
                    particles[i].positionNext.y += 1.5f * deltaY;
                    particles[i].positionNext.z += 1.5f * deltaZ;
                }

                rodDeltaCount[i] = 0;
                rodDeltas[i].x = 0;
                rodDeltas[i].y = 0;
                rodDeltas[i].z = 0;
                
            }
        }

        void updateCollisionBounds(){
            if(hasCollisionFloor){

                unsigned int maxParticleCount = particles.getBound();
                
                for (int i = 0; i < maxParticleCount; i++){
                    float py = particles[i].positionNext.y; 
                    float disp = collisionFloorHeight - py; 
                    particles[i].positionNext.y += disp > 0 ? disp : 0; 
                }                
            }
        }

        void updateBoxColliders(){
            unsigned int maxColliderCount = boxColliders.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxColliderCount](const int a, const int b){
                
                for(int i = 0; i < maxColliderCount; i++){

                    if(!boxColliders.isInUse(i))
                            continue; 

                    for(int p = a; p < b; p++){
                        
                        float halfsx = boxColliders[i].size.x/2;
                        float halfsy = boxColliders[i].size.y/2;
                        float halfsz = boxColliders[i].size.z/2;

                        Vec3 ppos = particles[p].position; 
                        ppos.multm(boxColliders[i].invRotation); 

                        if(ppos.x > halfsx || ppos.x < -halfsx || ppos.y > halfsy || ppos.y < -halfsy || ppos.z > halfsz || ppos.z < -halfsz){
                            continue; 
                        }


                    }
                }

            }).wait();
        }

        void updateSphereColliders(){
            
            unsigned int maxColliderCount = sphereColliders.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxColliderCount](const int a, const int b){
                
                for(int i = 0; i < maxColliderCount; i++){

                    if(!sphereColliders.isInUse(i))
                            continue; 

                    for(int p = a; p < b; p++){
                    
                        float xDiff = sphereColliders[i].position.x - particles[p].position.x; 
                        float yDiff = sphereColliders[i].position.y - particles[p].position.y;
                        float zDiff = sphereColliders[i].position.z - particles[p].position.z;

                        float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                        float dist = sqrt(dist2); 
                        float pen = sphereColliders[i].radius - dist; 

                        float xDiffNorm = xDiff / dist; 
                        float yDiffNorm = yDiff / dist; 
                        float zDiffNorm = zDiff / dist; 

                        float dispX = xDiffNorm * pen;
                        float dispY = yDiffNorm * pen; 
                        float dispZ = zDiffNorm * pen; 

                        particles[p].positionNext.x -= pen > 0 ? dispX : 0; 
                        particles[p].positionNext.y -= pen > 0 ? dispY : 0; 
                        particles[p].positionNext.z -= pen > 0 ? dispZ : 0; 
                    }
                }

            }).wait();
        }


    public:  
        

        World(){
            timestep = 0.016666; 
            substeps = 4; 
            globalDamping = 1;
            gravity = 9.81; 

            collisionFloorStaticFriction = 0; 
            collisionFloorKineticFriction = 0; 
            collisionFloorHeight = 0; 
            hasCollisionFloor = false;
            
            particles.setPoolSize(100000); 
            rodDeltas.setPoolSize(100000); 
            rodDeltaCount.setPoolSize(100000); 

            rods.setPoolSize(200000);
            anchorRods.setPoolSize(200000);

            attractors.setPoolSize(1000);  
            vortices.setPoolSize(1000); 
            globalForces.setPoolSize(1000); 

            noiseFields.setPoolSize(50); 
            noiseGenerators.setPoolSize(50); 

            boxColliders.setPoolSize(1000); 
            sphereColliders.setPoolSize(1000);
        }
        
        

        void update(){

            float deltaT = timestep / substeps; 
            float invDeltaT = 1.0f / deltaT; 

            for(int s = 0; s < substeps; s++){
            
                unsigned int maxParticleCount = particles.getBound();
                
                //Update forces 
                updateGlobalForces(); 
                updateAttractors(); 
                updateVortices(); 
                updateNoiseFieldsForces(); 

                //Apply velocities and gravity 
                for(int i = 0; i < maxParticleCount; i++){
                    particles[i].velocity.y -= deltaT * gravity; 
                    
                    particles[i].velocity.x *= globalDamping; 
                    particles[i].velocity.y *= globalDamping; 
                    particles[i].velocity.z *= globalDamping;

                    particles[i].positionNext.x = particles[i].position.x + deltaT * particles[i].velocity.x; 
                    particles[i].positionNext.y = particles[i].position.y + deltaT * particles[i].velocity.y;
                    particles[i].positionNext.z = particles[i].position.z + deltaT * particles[i].velocity.z;
                } 
 
                //update colliders
                updateCollisionBounds(); 
                updateBoxColliders(); 
                updateSphereColliders(); 

                //update rods 
                updateRods(); 

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

        void setGravity(float grav){
            gravity = grav; 
        }

        void setTimestep(float t){
            timestep = t; 
        }

        void setSubsteps(float s){
            substeps = s; 
        }

        void setGlobalDamping(float d){
            globalDamping = d;
        }

        void setHasCollisionFloor(bool h){
            hasCollisionFloor = h; 
        }

        void setCollisionFloor(float height, float staticFriction, float kineticFriction){
            collisionFloorStaticFriction = staticFriction; 
            collisionFloorKineticFriction = kineticFriction; 
            collisionFloorHeight = height; 
        }


        int addParticle(Vec3 initialPosition, Vec3 initialVelocity, float invMass){
            Particle p; 
            p.position = initialPosition; 
            p.positionNext = initialPosition; 
            p.velocity = initialVelocity; 
            p.invMass = invMass; 

            Vec3 zero = Vec3(0, 0, 0); 
            rodDeltas.add(zero);

            int zeroInt = 0; 
            rodDeltaCount.add(zeroInt);

            return particles.add(p); 
        }

        void destroyParticle(int inx){
            particles.remove(inx); 
            rodDeltas.remove(inx);
            rodDeltaCount.remove(inx);
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


        int addAttrator(Vec3 position, float strength, float minDist, float maxDist, Falloff falloff){
            Attractor a; 
            a.position = position; 
            a.strength = strength;
            a.minDist = minDist;
            a.maxDist = maxDist; 
            a.falloff = falloff;
            return attractors.add(a);  
        }

        Attractor* getAttractorPtr(int inx){
            return &(attractors[inx]); 
        }

        void destroyAttractor(int inx){
            attractors.remove(inx); 
        }

        void clearAttractors(){
            attractors.clear(); 
        }

        void setAttractorPosition(int inx, Vec3 position){
            attractors[inx].position = position; 
        }

        void setAttractorStrength(int inx, float strength){
            attractors[inx].strength = strength; 
        }

        void setAttractorMinDist(int inx, float minDist){
            attractors[inx].minDist = minDist; 
        }

        void setAttractorMaxDist(int inx, float maxDist){
            attractors[inx].maxDist = maxDist; 
        }

        void setAttractorFalloff(int inx, Falloff falloff){
            attractors[inx].falloff = falloff;
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


        int addRod(int a, int b, float length, float stiffness){
            Rod r;
            r.a = a; 
            r.b = b; 
            r.length = length; 
            r.stiffness = stiffness; 
            return rods.add(r); 
        }

        void destroyRod(int inx){
            rods.remove(inx); 
        }

        void setRodStiffness(int inx, float stiffness){
            rods[inx].stiffness = stiffness;
        }

        void setRodLength(int inx, float length){
            rods[inx].length = length;
        }


        int addAnchorRod(int a, Vec3 position, float length, float stiffness){
            AnchorRod r;
            r.a = a; 
            r.position = position; 
            r.length = length; 
            r.stiffness = stiffness; 
            return anchorRods.add(r); 
        }

        void destroyAnchorRod(int inx){
            anchorRods.remove(inx); 
        }

        void setAnchorRodStiffness(int inx, float stiffness){
            anchorRods[inx].stiffness = stiffness;
        }

        void setAnchorRodLength(int inx, float length){
            anchorRods[inx].length = length;
        }

        void setAnchorRodPosition(int inx, Vec3 position){
            anchorRods[inx].position = position;
        }


        int addNoiseField(NoiseType noiseType, float strength, float noiseScale, bool isVelocity){
            NoiseField n; 

            n.position = Vec3(0, 0, 0); 
            n.boundShape = ShapeType::Sphere; 
            n.boundSize = Vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()); 
            n.falloff = Falloff::InvDist; 
            n.falloffRatio = 0; 
            n.viscosity = 1.0f;

            n.noiseType = noiseType; 
            n.strength = strength; 
            n.noiseScale = noiseScale;
            n.isVelocity = isVelocity; 
            noiseFields.add(n);

            NoiseGenerator gen; 
            gen.setType(noiseType); 

            return noiseGenerators.add(gen);   
        }

        void destroyNoiseField(int inx){
            noiseFields.remove(inx); 
            noiseGenerators.remove(inx);
        }

        void clearNoiseFields(){
            noiseFields.clear(); 
            noiseGenerators.clear(); 
        }

        void setNoiseFieldViscosity(int inx, float viscosity){
            noiseFields[inx].viscosity = viscosity; 
        }



        int addSphereCollider(Vec3 position, float radius, float kineticFriction, float staticFriction, bool inverse){
            SphereCollider c; 
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