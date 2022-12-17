#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class World {
    private:
        BS::thread_pool threadPool;

        float timestep; 
        float invTimestep; 
        float globalDamping; 
        float gravity;
        DynamicPool<Particle> particles;
        DynamicPool<Rod> rods;
        DynamicPool<Vec3> rodDeltas; 
        DynamicPool<int> rodDeltaCount; 
        DynamicPool<Attractor> attractors;
        DynamicPool<Vortex> vortices;
        DynamicPool<Vec3> globalForces;

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

                    for(int p = 0; p < pcount; p++){
                        float xDiff = attractors[i].position.x - particles[p].position.x; 
                        float yDiff = attractors[i].position.y - particles[p].position.y;
                        float zDiff = attractors[i].position.z - particles[p].position.z;

                        float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                        float dist = sqrt(dist2); 
                        float dist2Factor = 1 / (dist2 * dist) ; 

                        particles[p].velocity.x += timestep * particles[p].invMass * xDiff * dist2Factor * attractors[i].strength;
                        particles[p].velocity.y += timestep * particles[p].invMass * yDiff * dist2Factor * attractors[i].strength;
                        particles[p].velocity.z += timestep * particles[p].invMass * zDiff * dist2Factor * attractors[i].strength;
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
                        float xDiff = vortices[i].position.x - particles[p].position.x; 
                        float yDiff = vortices[i].position.y - particles[p].position.y;
                        float zDiff = vortices[i].position.z - particles[p].position.z;

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

    public:  
        

        World(){
            timestep = 0.016666; 
            invTimestep = 1.0f/timestep;
            globalDamping = 1;
            gravity = 9.81; 
            
            particles.setPoolSize(100000); 
            rodDeltas.setPoolSize(100000); 
            rodDeltaCount.setPoolSize(100000); 

            rods.setPoolSize(200000);

            attractors.setPoolSize(1000);  
            vortices.setPoolSize(1000); 
            globalForces.setPoolSize(1000); 

        }
        
        

        void update(){

            unsigned int maxParticleCount = particles.getBound();
            
            //Update forces 
            updateGlobalForces(); 
            updateAttractors(); 
            updateVortices(); 

            //Apply velocities and gravity 
            for(int i = 0; i < maxParticleCount; i++){
                particles[i].velocity.y -= timestep * gravity; 
                
                particles[i].velocity.x *= globalDamping; 
                particles[i].velocity.y *= globalDamping; 
                particles[i].velocity.z *= globalDamping;

                particles[i].positionNext.x = particles[i].position.x + timestep * particles[i].velocity.x; 
                particles[i].positionNext.y = particles[i].position.y + timestep * particles[i].velocity.y;
                particles[i].positionNext.z = particles[i].position.z + timestep * particles[i].velocity.z;
            }     

            //update rods 
            updateRods(); 

            //Apply New Position
            for(int i = 0; i < maxParticleCount; i++){

                particles[i].velocity.x = (particles[i].positionNext.x - particles[i].position.x) * invTimestep;
                particles[i].velocity.y = (particles[i].positionNext.y - particles[i].position.y) * invTimestep;
                particles[i].velocity.z = (particles[i].positionNext.z - particles[i].position.z) * invTimestep;

                particles[i].position.x = particles[i].positionNext.x;
                particles[i].position.y = particles[i].positionNext.y;
                particles[i].position.z = particles[i].positionNext.z;
            }   
        }

        void setGravity(float grav){
            gravity = grav; 
        }

        void setTimestep(float t){
            timestep = t; 
            invTimestep = 1.0f/timestep;
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

            return particles.add(p); 
        }

        void destroyParticle(int inx){
            particles.remove(inx); 
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


        int addAttrator(Vec3 position, float strength){
            Attractor a; 
            a.position = position; 
            a.strength = strength;
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


        int addRod(int a, int b, float length, float strength){
            Rod r;
            r.a = a; 
            r.b = b; 
            r.length = length; 
            r.strength = strength; 
            return rods.add(r); 
        }

        void destroyRod(int inx){
            rods.remove(inx); 
        }

        void setRodStrength(int inx, float strength){
            rods[inx].strength = strength;
        }

        void setRodLength(int inx, float length){
            rods[inx].length = length;
        }
}; 