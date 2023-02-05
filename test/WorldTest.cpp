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

    /*
    for(int i = 0; i < attractorCount; i++){
        world.attractorSystem.addAttrator(Vec3(randf(),  randf(),  randf()), 1, 0.01, 100, Falloff::InvDist2); 
    }*/

    world.noiseFieldSystem.addNoiseField(Vec3(0,0,0), NoiseType::SimplexCurl, 2, 1, 50, FieldMode::CorrectionForce, Vec3(0, 0, 0), 
                                        BoundShapeType::Box, 0.1f, BoundFalloff::Linear, Mat3(), 128, true); 


    for(int i = 0; i < 1000; i++){
        world.update(); 
    }

    return 0; 
}