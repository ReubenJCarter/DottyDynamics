#include "../src/World.hpp"
#include <stdlib.h>

#include "../src/PerfTimer.hpp"

float randf(){
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r; 
}

int main(){

    World world; 

    int particleCount = 50000; 
    int attractorCount = 100; 
    int rodCount = 10000; 

    world.particles.setPoolSize(particleCount*2); 
    for(int i = 0; i < particleCount; i++){
        Particle p; 
        p.position.x = randf(); 
        p.position.y = randf(); 
        p.position.z = randf(); 
        p.velocity.x = randf(); 
        p.velocity.y = randf(); 
        p.velocity.z = randf(); 
        p.invMass = 1.0f/randf();

        world.particles.add(p); 
    }

    world.attractors.setPoolSize(attractorCount); 
    for(int i = 0; i < attractorCount; i++){
        Attractor a; 
        a.position = Vec3(randf() * 10, randf() * 10, randf() * 10);
        a.strength = randf(); 
        world.attractors.add(a); 
    }

    world.rods.setPoolSize(rodCount*2);
    for(int i = 0; i < rodCount; i++){
        Rod r; 
        r.a = randf() * (particleCount-1); 
        r.b = randf() * (particleCount-1); 
        r.length = randf() * 10; 
        r.strength = randf(); 
        world.rods.add(r); 
    }

    world.rodDeltas.setPoolSize(particleCount * 2);
    world.rodDeltaCount.setPoolSize(particleCount * 2); 
    for(int i = 0; i < particleCount; i++){
        Vec3 delta(0, 0, 0); 
        world.rodDeltas.add(delta);
        int deltaCount = 0;  
        world.rodDeltaCount.add(deltaCount); 
    }


    PerfTimer perfTimer; 
    perfTimer.reset(); 

    for(int i = 0; i < 100; i++){
        world.update(); 
    }

    perfTimer.lap(); 


    return 0; 
}