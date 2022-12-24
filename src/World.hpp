#pragma once 

#include <limits>
#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"
#include "Noise.hpp"

class World {
    private:
        BS::thread_pool threadPool;

        float timestep; 
        int substeps; 
        float globalDamping; 
        float gravity;
        DynamicPool<Particle> particles;
        DynamicPool<Rod> rods;
        DynamicPool<AnchorRod> anchorRods;
        DynamicPool<Vec3> rodDeltas; 
        DynamicPool<int> rodDeltaCount; 
        DynamicPool<Attractor> attractors;
        DynamicPool<Vortex> vortices;
        DynamicPool<Vec3> globalForces;
        DynamicPool<NoiseField> noiseFields; 

        Noise simplexNoise; 

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

                    if(attractors[i].falloff == Falloff::Squared){
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
                    else if(attractors[i].falloff == Falloff::Linear){
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
                    else if(attractors[i].falloff == Falloff::LinearWell){
                        for(int p = 0; p < pcount; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                            float dist = sqrt(dist2); 

                            float distFactor = dist > attractors[i].maxDist || dist < attractors[i].minDist ? 0 : dist;

                            particles[p].velocity.x += timestep * particles[p].invMass * xDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.y += timestep * particles[p].invMass * yDiff * distFactor * attractors[i].strength;
                            particles[p].velocity.z += timestep * particles[p].invMass * zDiff * distFactor * attractors[i].strength;
                        }
                    }
                    else if(attractors[i].falloff == Falloff::SquaredWell){
                        for(int p = 0; p < pcount; p++){
                            float xDiff = attractors[i].position.x - particles[p].position.x; 
                            float yDiff = attractors[i].position.y - particles[p].position.y;
                            float zDiff = attractors[i].position.z - particles[p].position.z;

                            float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff;
                            float maxDist2 = attractors[i].maxDist * attractors[i].maxDist; 
                            float minDist2 = attractors[i].minDist * attractors[i].minDist; 

                            float distFactor = dist2 > maxDist2 || dist2 < minDist2 ? 0 : dist2;

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

                    for(int p = 0; p < pcount; p++){
                        
                    }
                }

            }).wait();

        }

        void updateNoiseFields(){
            unsigned int noiseFieldsCount = noiseFields.getBound(); 

            threadPool.parallelize_loop(noiseFieldsCount, [this](const int a, const int b){
                
                unsigned int pcount = particles.getBound();
                
                for(int i = a; i < b; i++){

                    if(!noiseFields.isInUse(i))
                        continue; 

                    for(int p = 0; p < pcount; p++){
                        
                        Vec3 curl = simplexNoise.getCurl(particles[p].position); 
                        
                        particles[p].velocity.x += timestep * particles[p].invMass * curl.x * noiseFields[i].strength;
                        particles[p].velocity.y += timestep * particles[p].invMass * curl.y * noiseFields[i].strength;
                        particles[p].velocity.z += timestep * particles[p].invMass * curl.z * noiseFields[i].strength;
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


    public:  
        

        World(){
            timestep = 0.016666; 
            substeps = 4; 
            globalDamping = 1;
            gravity = 9.81; 
            
            particles.setPoolSize(100000); 
            rodDeltas.setPoolSize(100000); 
            rodDeltaCount.setPoolSize(100000); 

            rods.setPoolSize(200000);
            anchorRods.setPoolSize(200000);

            attractors.setPoolSize(1000);  
            vortices.setPoolSize(1000); 
            globalForces.setPoolSize(1000); 

            noiseFields.setPoolSize(100); 

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
                updateNoiseFields(); 

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


        int addParticle(Vec3 initialPosition, Vec3 initialVelocity, float invMass){
            Particle p; 
            p.position = initialPosition; 
            p.positionNext = initialPosition; 
            p.velocity = initialVelocity; 
            p.invMass = invMass; 
            p.rodCount = 0; 
            p.rodDelta = Vec3(0, 0, 0); 

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

        void destroyAttractor(int inx){
            attractors.remove(inx); 
        }

        void setAttractorPosition(int inx, Vec3 position){
            attractors[inx].position = position; 
        }

        void setAttractorStrength(int inx, float strength){
            attractors[inx].strength = strength; 
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


        int addNoiseField(NoiseType noiseType, float strength, float noiseScale){
            NoiseField n; 
            
            n.position = Vec3(0, 0, 0); 
            n.boundShape = BoundShape::Sphere; 
            n.boundSize = Vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()); 
            n.falloff = Falloff::Linear; 
            n.falloffRatio = 0; 

            n.noiseType = noiseType; 
            n.strength = strength; 
            n.noiseScale = noiseScale;
            return noiseFields.add(n);  
        }

        void destroyNoiseField(int inx){
            noiseFields.remove(inx); 
        }
}; 