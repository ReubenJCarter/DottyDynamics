#include "../src/World.hpp"
#include <stdlib.h>

float randf(){
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r; 
}

int main(){

    World world; 

    int particleCount = 50000; 
    int attractorCount = 100; 
    
    for(int i = 0; i < particleCount; i++){
        world.addParticle(Vec3(randf(),  randf(),  randf()), Vec3(0, 0, 0), 1); 
    }

    for(int i = 0; i < attractorCount; i++){
        world.addAttrator(Vec3(randf(),  randf(),  randf()), 1, 0.01, 100, Falloff::InvDist2); 
    }


    for(int i = 0; i < 100; i++){
        world.update(); 
    }

    return 0; 
}