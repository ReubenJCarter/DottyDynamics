#define PCOUNT 50000
#define ATTRACTOR_COUNT 200
#define ROD_COUNT 50000

#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <math.h>
#include <thread> 

#include "../src/PerfTimer.hpp"
#include "../src/thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "../src/thirdparty/VecMath/VecMath.hpp"
#include "../src/DynamicPool.hpp"


typedef std::chrono::high_resolution_clock Clock;


float randf(){
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r; 
}


float damping = 0.1; 
float timestep = 0.01; 
float gravity = 9.81; 






struct Particle {
    float x, y, z, xNext, yNext, zNext,  vx, vy, vz, invMass; 
};

struct Attractor {
    float x; 
    float y; 
    float z; 
    float strength; 
}; 

Attractor* attractors; 

struct Rod {
    int a; 
    int b;
    float length;
    float strength;  
}; 

class ParticlesAOS {
public:
    int particleCount; 
    Particle* particles;
    Rod* rods; 
    Vec3* rodDeltas; 
    int* rodDeltaCount; 

    ParticlesAOS(){
        particleCount = PCOUNT; 
        particles = new Particle[particleCount]; 
        for(int i = 0; i < particleCount; i++){
            particles[i].x = randf(); 
            particles[i].y = randf(); 
            particles[i].z = randf();  
            particles[i].vx = randf(); 
            particles[i].vy = randf(); 
            particles[i].vz = randf();
            particles[i].invMass = 1.0f/randf();  
        }

        attractors = new Attractor[ATTRACTOR_COUNT]; 
        for(int i = 0; i < ATTRACTOR_COUNT; i++){
            attractors[i].x = randf() * 10; 
            attractors[i].y = randf() * 10; 
            attractors[i].z = randf() * 10;  
            attractors[i].strength = randf(); 
        }

        rods = new Rod[ROD_COUNT]; 
        for(int i = 0; i < ROD_COUNT; i++){
            rods[i].a = randf() * (particleCount-1); 
            rods[i].b = randf() * (particleCount-1); 
            rods[i].length = randf() * 10; 
            rods[i].strength = randf(); 
        }

        rodDeltas = new Vec3[particleCount];
        rodDeltaCount = new int[particleCount]; 
        for(int i = 0; i < particleCount; i++){
            rodDeltas[i].x = 0; 
            rodDeltas[i].y = 0; 
            rodDeltas[i].z = 0;  
            rodDeltaCount[i] = 0; 
        }
    }

    void update(){

        //
        //Update forces 
        for(int i = 0; i < ATTRACTOR_COUNT; i++){
            for(int p = 0; p < particleCount; p++){
                float xDiff = attractors[i].x - particles[p].x; 
                float yDiff = attractors[i].y - particles[p].y;
                float zDiff = attractors[i].z - particles[p].z;

                float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                float dist = sqrt(dist2); 
                float dist2Factor = 1 / (dist2 * dist) ; 

                particles[p].vx += timestep * particles[p].invMass * xDiff * dist2Factor;
                particles[p].vy += timestep * particles[p].invMass * yDiff * dist2Factor;
                particles[p].vz += timestep * particles[p].invMass * zDiff * dist2Factor;
            }
        }

        //
        //Apply velocities and gravity 
        for(int i = 0; i < particleCount; i++){

            particles[i].vy -= gravity; 

            particles[i].vx *= damping; 
            particles[i].vy *= damping; 
            particles[i].vz *= damping; 
    
            particles[i].xNext = particles[i].x + timestep * particles[i].vx; 
            particles[i].yNext = particles[i].y + timestep * particles[i].vy;
            particles[i].zNext = particles[i].z + timestep * particles[i].vz;
        }

        //
        //Generate collision constraints

        //
        //Project constraits

        //Update Rods
        for(int i = 0; i < ROD_COUNT; i++){
            float invMassA = particles[rods[i].a].invMass;
            float invMassB = particles[rods[i].b].invMass;
            float dx = (particles[rods[i].a].xNext - particles[rods[i].b].xNext);
            float dy = (particles[rods[i].a].yNext - particles[rods[i].b].yNext);
            float dz = (particles[rods[i].a].zNext - particles[rods[i].b].zNext);

            float diffLen2 = dx * dx + dy * dy + dz * dz;
            float diffLen = diffLen2 == 0 ? 1 : sqrt(diffLen2);
            float rodDiffLen = diffLen - rods[i].length;

            float factor = (rods[i].strength * rodDiffLen) / (diffLen * (invMassA + invMassB));

            dx *= factor;
            dy *= factor;
            dz *= factor;

            rodDeltas[rods[i].a].x -= invMassA * dx;
            rodDeltas[rods[i].a].y -= invMassA * dy;
            rodDeltas[rods[i].a].z -= invMassA * dz;
            rodDeltas[rods[i].b].x += invMassB * dx;
            rodDeltas[rods[i].b].y += invMassB * dy;
            rodDeltas[rods[i].b].z += invMassB * dz;

            rodDeltaCount[rods[i].a]++;
            rodDeltaCount[rods[i].b]++;
        
        }

        for (int i = 0; i < particleCount; i++){
            
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

                particles[i].xNext += 1.5f * deltaX;
                particles[i].yNext += 1.5f * deltaY;
                particles[i].zNext += 1.5f * deltaZ;
            }
            
        }

        //
        //Apply New Position
        for(int i = 0; i < particleCount; i++){
            particles[i].vx = (particles[i].xNext - particles[i].x) * timestep;
            particles[i].vy = (particles[i].yNext - particles[i].y) * timestep;
            particles[i].vz = (particles[i].zNext - particles[i].z) * timestep;

            particles[i].x = particles[i].xNext;
            particles[i].y = particles[i].yNext;
            particles[i].z = particles[i].zNext;
        }
    }
}; 


class ParticlesAOSMultiThreadded {
public:
    BS::thread_pool pool;
    int particleCount; 
    Particle* particles;
    Rod* rods; 
    Vec3* rodDeltas; 
    int* rodDeltaCount; 
    

    ParticlesAOSMultiThreadded(){
        particleCount = PCOUNT; 
        particles = new Particle[particleCount]; 
        for(int i = 0; i < particleCount; i++){
            particles[i].x = randf(); 
            particles[i].y = randf(); 
            particles[i].z = randf();  
            particles[i].vx = randf(); 
            particles[i].vy = randf(); 
            particles[i].vz = randf();
            particles[i].invMass = 1.0f/randf();  
        }

        rods = new Rod[ROD_COUNT]; 
        for(int i = 0; i < ROD_COUNT; i++){
            rods[i].a = randf() * (particleCount-1); 
            rods[i].b = randf() * (particleCount-1); 
            rods[i].length = randf() * 10; 
            rods[i].strength = randf(); 
        }

        rodDeltas = new Vec3[particleCount];
        rodDeltaCount = new int[particleCount]; 
        for(int i = 0; i < particleCount; i++){
            rodDeltas[i].x = 0; 
            rodDeltas[i].y = 0; 
            rodDeltas[i].z = 0;  
            rodDeltaCount[i] = 0; 
        }

    }

    void update(){

        //
        //Update forces 
        pool.parallelize_loop(ATTRACTOR_COUNT, [this](const int a, const int b)
        {
            for(int i = a; i < b; i++){
                for(int p = 0; p < particleCount; p++){
                    float xDiff = attractors[i].x - particles[p].x; 
                    float yDiff = attractors[i].y - particles[p].y;
                    float zDiff = attractors[i].z - particles[p].z;

                    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                    float dist = sqrt(dist2); 
                    float dist2Factor = 1 / (dist2 * dist) ; 

                    particles[p].vx += timestep * particles[p].invMass * xDiff * dist2Factor;
                    particles[p].vy += timestep * particles[p].invMass * yDiff * dist2Factor;
                    particles[p].vz += timestep * particles[p].invMass * zDiff * dist2Factor;
                }
            }

        })
        .wait();
        

        //
        //Apply velocities and gravity 
        //parallel processing is no faster here as the memory bandwidth is limited here.
        for(int i = 0; i < particleCount; i++){

            particles[i].vy -= gravity; 

            particles[i].vx *= damping; 
            particles[i].vy *= damping; 
            particles[i].vz *= damping; 
    
            particles[i].xNext = particles[i].x + timestep * particles[i].vx; 
            particles[i].yNext = particles[i].y + timestep * particles[i].vy;
            particles[i].zNext = particles[i].z + timestep * particles[i].vz;
        }
            
        //
        //Generate collision constraints

        //
        //Project constraits

        //Update Rods
        for(int i = 0; i < ROD_COUNT; i++){
            float invMassA = particles[rods[i].a].invMass;
            float invMassB = particles[rods[i].b].invMass;
            float dx = (particles[rods[i].a].xNext - particles[rods[i].b].xNext);
            float dy = (particles[rods[i].a].yNext - particles[rods[i].b].yNext);
            float dz = (particles[rods[i].a].zNext - particles[rods[i].b].zNext);

            float diffLen2 = dx * dx + dy * dy + dz * dz;
            float diffLen = diffLen2 == 0 ? 1 : sqrt(diffLen2);
            float rodDiffLen = diffLen - rods[i].length;

            float factor = (rods[i].strength * rodDiffLen) / (diffLen * (invMassA + invMassB));

            dx *= factor;
            dy *= factor;
            dz *= factor;

            rodDeltas[rods[i].a].x -= invMassA * dx;
            rodDeltas[rods[i].a].y -= invMassA * dy;
            rodDeltas[rods[i].a].z -= invMassA * dz;
            rodDeltas[rods[i].b].x += invMassB * dx;
            rodDeltas[rods[i].b].y += invMassB * dy;
            rodDeltas[rods[i].b].z += invMassB * dz;

            rodDeltaCount[rods[i].a]++;
            rodDeltaCount[rods[i].b]++;
        
        }

        for (int i = 0; i < particleCount; i++){
            
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

                particles[i].xNext += 1.5f * deltaX;
                particles[i].yNext += 1.5f * deltaY;
                particles[i].zNext += 1.5f * deltaZ;
            }
            
        }


        //
        //Apply New Position
        for(int i = 0; i < particleCount; i++){
            particles[i].vx = (particles[i].xNext - particles[i].x) * timestep;
            particles[i].vy = (particles[i].yNext - particles[i].y) * timestep;
            particles[i].vz = (particles[i].zNext - particles[i].z) * timestep;

            particles[i].x = particles[i].xNext;
            particles[i].y = particles[i].yNext;
            particles[i].z = particles[i].zNext;
        }   
    }
}; 



class Particle2{
public:
    Vec3 position;
    Vec3 positionNext; 
    Vec3 velocity; 
    //Vec3 rodDelta; 
    //int rodCount; 
    float invMass; 
}; 

struct Attractor2 {
    Vec3 position;
    float strength; 
}; 

Attractor2* attractors2; 

class ParticlesAOSMultiThreaddedV2 {
public:
    BS::thread_pool threadPool;
    int particleCount; 
    DynamicPool<Particle2> particles;
    DynamicPool<Rod> rods;
    DynamicPool<Vec3> rodDeltas; 
    DynamicPool<int> rodDeltaCount; 
    

    ParticlesAOSMultiThreaddedV2(){
        particleCount = PCOUNT; 
        particles.setPoolSize(particleCount*2); 
        for(int i = 0; i < particleCount; i++){
            Particle2 p; 
            p.position.x = randf(); 
            p.position.y = randf(); 
            p.position.z = randf(); 
            p.velocity.x = randf(); 
            p.velocity.y = randf(); 
            p.velocity.z = randf(); 
            p.invMass = 1.0f/randf();

            particles.add(p); 
        }

        attractors2 = new Attractor2[ATTRACTOR_COUNT]; 
        for(int i = 0; i < ATTRACTOR_COUNT; i++){
            attractors2[i].position.x = randf() * 10;
            attractors2[i].position.y =  randf() * 10;
            attractors2[i].position.z =  randf() * 10;
            attractors2[i].strength = randf(); 
        }

        rods.setPoolSize(ROD_COUNT*2);
        for(int i = 0; i < ROD_COUNT; i++){
            Rod r; 
            r.a = randf() * (particleCount-1); 
            r.b = randf() * (particleCount-1); 
            r.length = randf() * 10; 
            r.strength = randf(); 
            rods.add(r); 
        }

        rodDeltas.setPoolSize(particleCount * 2);
        rodDeltaCount.setPoolSize(particleCount * 2); 
        for(int i = 0; i < particleCount; i++){
            Vec3 delta(0, 0, 0); 
            rodDeltas.add(delta);
            int deltaCount = 0;  
            rodDeltaCount.add(deltaCount); 
        }

    }

    void update(){

        //
        //Update forces 
        threadPool.parallelize_loop(ATTRACTOR_COUNT, [this](const int a, const int b){
            for(int i = a; i < b; i++){
                unsigned int pcount = particles.getBound();
                for(int p = 0; p < pcount; p++){
                    
                    
                    float xDiff = attractors2[i].position.x - particles[p].position.x; 
                    float yDiff = attractors2[i].position.y - particles[p].position.y;
                    float zDiff = attractors2[i].position.z - particles[p].position.z;

                    float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                    float dist = sqrt(dist2); 
                    float dist2Factor = 1 / (dist2 * dist) ; 

                    particles[p].velocity.x += timestep * particles[p].invMass * xDiff * dist2Factor;
                    particles[p].velocity.y += timestep * particles[p].invMass * yDiff * dist2Factor;
                    particles[p].velocity.z += timestep * particles[p].invMass * zDiff * dist2Factor;
                    
                    /*
                    Vec3 diff(attractors2[i].position);
                    diff.sub(particles[p].position);  

                    float dist2 = diff.len2(); 
                    float dist = sqrt(dist2); 
                    float dist2Factor = 1 / (dist2 * dist) ; 

                    diff.mults(timestep * particles[p].invMass * dist2Factor);

                    particles[p].velocity.add(diff); 
                    */
                }
            }

        })
        .wait();


        unsigned int maxParticleCount = particles.getBound();
        

        //
        //Apply velocities and gravity 
        //parallel processing is no faster here as the memory bandwidth is limited here.
        for(int i = 0; i < maxParticleCount; i++){


            particles[i].velocity.y -= gravity; 
            
            particles[i].velocity.x *= damping; 
            particles[i].velocity.y *= damping; 
            particles[i].velocity.z *= damping;
            
            particles[i].positionNext.x = particles[i].position.x + timestep * particles[i].velocity.x; 
            particles[i].positionNext.y = particles[i].position.y + timestep * particles[i].velocity.y;
            particles[i].positionNext.z = particles[i].position.z + timestep * particles[i].velocity.z;
            
            /*
            particles[i].velocity.mults(damping);

            particles[i].positionNext.set( particles[i].position );
            particles[i].positionNext.add( Vec3(particles[i].velocity).mults(timestep) ); 
            */
        }
            
        //
        //Generate collision constraints

        //
        //Project constraits

        //Update Rods
        for(int i = 0; i < ROD_COUNT; i++){
            
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
            
            rodDeltas[rods[i].a].x -= invMassA * dx;
            rodDeltas[rods[i].a].y -= invMassA * dy;
            rodDeltas[rods[i].a].z -= invMassA * dz;
            rodDeltas[rods[i].b].x += invMassB * dx;
            rodDeltas[rods[i].b].y += invMassB * dy;
            rodDeltas[rods[i].b].z += invMassB * dz;

            rodDeltaCount[rods[i].a]++;
            rodDeltaCount[rods[i].b]++;

            /*
            particles[rods[i].a].rodDelta.x -= invMassA * dx;
            particles[rods[i].a].rodDelta.y -= invMassA * dy;
            particles[rods[i].a].rodDelta.z -= invMassA * dz;
            particles[rods[i].b].rodDelta.x += invMassB * dx;
            particles[rods[i].b].rodDelta.y += invMassB * dy;
            particles[rods[i].b].rodDelta.z += invMassB * dz;

            particles[rods[i].a].rodCount++;
            particles[rods[i].b].rodCount++;
            */
            /*
            float invMassA = particles[rods[i].a].invMass;
            float invMassB = particles[rods[i].b].invMass;

            Vec3 d(particles[rods[i].a].positionNext); 
            d.sub(particles[rods[i].b].positionNext); 

            float diffLen2 = d.len2();
            float diffLen = diffLen2 == 0 ? 1 : sqrt(diffLen2);
            float rodDiffLen = diffLen - rods[i].length;

            float factor = (rods[i].strength * rodDiffLen) / (diffLen * (invMassA + invMassB));

            d.mults(factor); 

            rodDeltas[rods[i].a].sub(Vec3(d).mults(invMassA));
            rodDeltas[rods[i].b].add(Vec3(d).mults(invMassB));
            rodDeltaCount[rods[i].a]++;
            rodDeltaCount[rods[i].b]++;
            */
        }

        
        for (int i = 0; i < maxParticleCount; i++){
            
            
            int deltaCount = rodDeltaCount[i];
            float deltaX = rodDeltas[i].x;
            float deltaY = rodDeltas[i].y;
            float deltaZ = rodDeltas[i].z;

            rodDeltaCount[i] = 0;
            rodDeltas[i].x = 0;
            rodDeltas[i].y = 0;
            rodDeltas[i].z = 0;
            
            /*
            int deltaCount = particles[i].rodCount;
            float deltaX = particles[i].rodDelta.x;
            float deltaY = particles[i].rodDelta.y;
            float deltaZ = particles[i].rodDelta.z;

            particles[i].rodCount = 0;
            particles[i].rodDelta.x = 0;
            particles[i].rodDelta.y = 0;
            particles[i].rodDelta.z = 0;
            */
            if (deltaCount > 0){
                
                deltaX /= deltaCount;
                deltaY /= deltaCount;
                deltaZ /= deltaCount;

                particles[i].positionNext.x += 1.5f * deltaX;
                particles[i].positionNext.y += 1.5f * deltaY;
                particles[i].positionNext.z += 1.5f * deltaZ;
            }
            
            /*
            int deltaCount = rodDeltaCount[i];
            Vec3 delta(rodDeltas[i]);
            rodDeltaCount[i] = 0;
            rodDeltas[i].set(0, 0, 0);

            if (deltaCount > 0){
                
                delta.divs(deltaCount); 
                particles[i].positionNext.add(delta.mults(1.5f)); 

            }
            */
            
        }


        //
        //Apply New Position
        for(int i = 0; i < maxParticleCount; i++){

            particles[i].velocity.x = (particles[i].positionNext.x - particles[i].position.x) * timestep;
            particles[i].velocity.y = (particles[i].positionNext.y - particles[i].position.y) * timestep;
            particles[i].velocity.z = (particles[i].positionNext.z - particles[i].position.z) * timestep;

            particles[i].position.x = particles[i].positionNext.x;
            particles[i].position.y = particles[i].positionNext.y;
            particles[i].position.z = particles[i].positionNext.z;
            
            /*
            particles[i].velocity.set(particles[i].positionNext).sub(particles[i].position).mults(timestep); 
            particles[i].position.set(particles[i].positionNext); 
            */
        }   
    }
}; 


class ParticlesSOA {
public:
    float* x;  
    float* y; 
    float* z; 
    float* xNext; 
    float* yNext;  
    float* zNext; 
    float* vx; 
    float* vy;  
    float* vz; 
    float* invMass;  

    int* roda;
    int* rodb;
    float* rodStrength;
    float* rodLength;

    float* rodDeltax; 
    float* rodDeltay; 
    float* rodDeltaz; 
    int* rodDeltaCount; 

    ParticlesSOA(){
        x = new float[PCOUNT];  
        y = new float[PCOUNT]; 
        z = new float[PCOUNT]; 
        xNext = new float[PCOUNT]; 
        yNext = new float[PCOUNT];  
        zNext = new float[PCOUNT];
        vx = new float[PCOUNT];  
        vy = new float[PCOUNT]; 
        vz = new float[PCOUNT];
        invMass = new float[PCOUNT]; 

        for(int i = 0; i < PCOUNT; i++){
            x[i] = randf(); 
            y[i] = randf(); 
            z[i] = randf(); 
            vx[i] = randf(); 
            vy[i] = randf(); 
            vz[i] = randf(); 
            invMass[i] = 1.0f/randf(); 
        }

        roda = new int[ROD_COUNT]; 
        rodb = new int[ROD_COUNT];
        rodStrength = new float[ROD_COUNT];
        rodLength = new float[ROD_COUNT];
        for(int i = 0; i < ROD_COUNT; i++){
            roda[i] = randf() * (PCOUNT-1); 
            rodb[i] = randf() * (PCOUNT-1); 
            rodLength[i] = randf() * 10; 
            rodStrength[i] = randf(); 
        }

        rodDeltax = new float[PCOUNT];
        rodDeltay = new float[PCOUNT];
        rodDeltaz = new float[PCOUNT];
        rodDeltaCount = new int[PCOUNT]; 
        for(int i = 0; i < PCOUNT; i++){
            rodDeltax[i] = 0; 
            rodDeltay[i] = 0; 
            rodDeltaz[i] = 0;  
            rodDeltaCount[i] = 0; 
        }
    }

    void update(){
        //Update forces 
        for(int i = 0; i < ATTRACTOR_COUNT; i++){
            for(int p = 0; p < PCOUNT; p++){
                float xDiff = attractors[i].x - x[p]; 
                float yDiff = attractors[i].y - y[p]; 
                float zDiff = attractors[i].z - z[p];

                float dist2 = xDiff * xDiff + yDiff * yDiff + zDiff * zDiff; 
                float dist = sqrt(dist2); 
                float dist2Factor = 1 / (dist2 * dist) ; 

                vx[p] += timestep * invMass[p] * xDiff * dist2Factor;
                vy[p] += timestep * invMass[p] * yDiff * dist2Factor;
                vz[p] += timestep * invMass[p] * zDiff * dist2Factor;
            }
        }

        //Apply velocities and gravity 
        for(int i = 0; i < PCOUNT; i++){
            vx[i] *= damping; 
            xNext[i] = x[i] + timestep * vx[i]; 
        }
        for(int i = 0; i < PCOUNT; i++){
            vy[i] -= gravity; 
            vy[i] *= damping; 
            yNext[i] = y[i] + timestep * vy[i];
        }
        for(int i = 0; i < PCOUNT; i++){
            vz[i] *= damping; 
            zNext[i] = z[i] + timestep * vz[i];
        }
        
        //Generate collision constraints

        //Project constraits

        //Update Rods
        for(int i = 0; i < ROD_COUNT; i++){
            float invMassA = invMass[roda[i]];
            float invMassB = invMass[rodb[i]];
            float dx = (xNext[roda[i]] - xNext[rodb[i]]);
            float dy = (yNext[roda[i]] - yNext[rodb[i]]);
            float dz = (zNext[roda[i]] - zNext[rodb[i]]);

            float diffLen2 = dx * dx + dy * dy + dz * dz;
            float diffLen = diffLen2 == 0 ? 1 : sqrt(diffLen2);
            float rodDiffLen = diffLen - rodStrength[i];

            float factor = (rodStrength[i] * 1.0f * rodDiffLen) / (diffLen * (invMassA + invMassB));

            dx *= factor;
            dy *= factor;
            dz *= factor;

            rodDeltax[roda[i]] -= invMassA * dx;
            rodDeltay[roda[i]] -= invMassA * dy;
            rodDeltaz[roda[i]] -= invMassA * dz;
            rodDeltax[rodb[i]] += invMassB * dx;
            rodDeltay[rodb[i]] += invMassB * dy;
            rodDeltaz[rodb[i]] += invMassB * dz;

            rodDeltaCount[roda[i]]++;
            rodDeltaCount[rodb[i]]++;
        
        }

        for (int i = 0; i < PCOUNT; i++){
            
            int deltaCount = rodDeltaCount[i];
            float deltaX = rodDeltax[i];
            float deltaY = rodDeltay[i];
            float deltaZ = rodDeltaz[i];

            rodDeltaCount[i] = 0;
            rodDeltax[i] = 0;
            rodDeltay[i] = 0;
            rodDeltaz[i] = 0;

            if (deltaCount > 0){
                
                deltaX /= deltaCount;
                deltaY /= deltaCount;
                deltaZ /= deltaCount;

                xNext[i] += 1.5f * deltaX;
                yNext[i] += 1.5f * deltaY;
                zNext[i] += 1.5f * deltaZ;
            }
            
        }

        //Apply New Position
        for(int i = 0; i < PCOUNT; i++){
            vx[i] = (xNext[i] - x[i]) * timestep;
            x[i] = xNext[i];
        }
        for(int i = 0; i < PCOUNT; i++){
            vy[i] = (yNext[i] - y[i]) * timestep;
            y[i] = yNext[i];
        }
        for(int i = 0; i < PCOUNT; i++){
            vz[i] = (zNext[i] - z[i]) * timestep;
            z[i] = zNext[i];
        }
    }
}; 

int main(){

    PerfTimer perfTimer;  
    //AOS TEST 
    std::cout << "AOS TEST: \n"; 
    ParticlesAOS pAOS; 

    perfTimer.Reset(); 
    for(int i = 0; i < 100; i++){
        pAOS.update(); 
    }
    perfTimer.Lap();


    //AOS multithreadded V2 TEST 
    std::cout << "AOS multi TEST 2: \n"; 
    ParticlesAOSMultiThreaddedV2 pAOSMultiv2; 

    perfTimer.Reset(); 
    for(int i = 0; i < 100; i++){
        pAOSMultiv2.update(); 
    }
    perfTimer.Lap();



    //AOS multithreadded TEST 
    std::cout << "AOS multi TEST: \n"; 
    ParticlesAOSMultiThreadded pAOSMulti; 

    perfTimer.Reset(); 
    for(int i = 0; i < 100; i++){
        pAOSMulti.update(); 
    }
    perfTimer.Lap();


    //SOA TEST 
    std::cout << "SOA TEST: \n"; 
    ParticlesSOA pSOA; 

    perfTimer.Reset(); 
    for(int i = 0; i < 100; i++){
        pSOA.update(); 
    }
    perfTimer.Lap(); 

    

    return 0; 
}