#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class World {
    private:
        BS::thread_pool threadPool;

    public:  
        float timestep; 
        float gloablDamping; 
        float gravity;
        DynamicPool<Particle> particles;
        DynamicPool<Rod> rods;
        DynamicPool<Vec3> rodDeltas; 
        DynamicPool<int> rodDeltaCount; 
        DynamicPool<Attractor> attractors;
        DynamicPool<Vec3> globalForces;

        World(){
            timestep = 0.016666; 
            gloablDamping = 1;
            gravity = 9.81; 
        }
        
        void updateGlobalForces(){

            unsigned int maxGlobalForceCount = globalForces.getBound(); 

            threadPool.parallelize_loop(maxGlobalForceCount, [this](const int a, const int b){
                
                unsigned int pcount = particles.getBound();
                
                for(int i = a; i < b; i++){

                    if(globalForces.isInUse(i))
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

                    if(attractors.isInUse(i))
                        continue; 

                    for(int p = 0; p < pcount; p++){
                        float xDiff = attractors[i].position.x - particles[p].position.x; 
                        float yDiff = attractors[i].position.y - particles[p].position.y;
                        float zDiff = attractors[i].position.z - particles[p].position.z;

                        float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                        float dist = sqrt(dist2); 
                        float dist2Factor = 1 / (dist2 * dist) ; 

                        particles[p].velocity.x += timestep * particles[p].invMass * xDiff * dist2Factor;
                        particles[p].velocity.y += timestep * particles[p].invMass * yDiff * dist2Factor;
                        particles[p].velocity.z += timestep * particles[p].invMass * zDiff * dist2Factor;
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

                float factor = (rods[i].strength * rodDiffLen) / (diffLen * (invMassA + invMassB));

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

            unsigned int maxParticleCount = particles.getBound();
            
            for (int i = 0; i < maxParticleCount; i++){

                int deltaCount = rodDeltaCount[i];
                float deltaX = rodDeltas[i].x;
                float deltaY = rodDeltas[i].y;
                float deltaZ = rodDeltas[i].z;

                rodDeltaCount[i] = 0;
                rodDeltas[i].x = 0;
                rodDeltas[i].y = 0;
                rodDeltas[i].z = 0;
                
                if (deltaCount > 0){
                    
                    deltaX /= deltaCount;
                    deltaY /= deltaCount;
                    deltaZ /= deltaCount;

                    particles[i].positionNext.x += 1.5f * deltaX;
                    particles[i].positionNext.y += 1.5f * deltaY;
                    particles[i].positionNext.z += 1.5f * deltaZ;
                }
                
            }
        }

        void update(){

            unsigned int maxParticleCount = particles.getBound();
            
            //Update forces 
            updateGlobalForces(); 
            updateAttractors(); 

            //Apply velocities and gravity 
            for(int i = 0; i < maxParticleCount; i++){
                particles[i].velocity.y -= gravity; 
                
                particles[i].velocity.x *= gloablDamping; 
                particles[i].velocity.y *= gloablDamping; 
                particles[i].velocity.z *= gloablDamping;

                particles[i].positionNext.x = particles[i].position.x + timestep * particles[i].velocity.x; 
                particles[i].positionNext.y = particles[i].position.y + timestep * particles[i].velocity.y;
                particles[i].positionNext.z = particles[i].position.z + timestep * particles[i].velocity.z;
            }     

            //update rods 
            updateRods(); 

            //Apply New Position
            for(int i = 0; i < maxParticleCount; i++){

                particles[i].velocity.x = (particles[i].positionNext.x - particles[i].position.x) * timestep;
                particles[i].velocity.y = (particles[i].positionNext.y - particles[i].position.y) * timestep;
                particles[i].velocity.z = (particles[i].positionNext.z - particles[i].position.z) * timestep;

                particles[i].position.x = particles[i].positionNext.x;
                particles[i].position.y = particles[i].positionNext.y;
                particles[i].position.z = particles[i].positionNext.z;
            }   
        }

        

}; 