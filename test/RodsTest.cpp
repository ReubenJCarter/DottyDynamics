#include "../src/World.hpp"
#include <stdlib.h>

#include "../src/PerfTimer.hpp"

float randf(){
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return r; 
}

int main(){

    World world; 

    PerfTimer perfTimer; 

    int particleCount = 100; 

    world.setGravity(0); 
    
    for(int i = 0; i < particleCount; i++){
        world.addParticle(Vec3(i, i,  i ), Vec3(0, 0, 0), 1); 
    }

    world.attractorSystem.addAttrator(Vec3(0, 0, 0), 100, 0, 1000, Falloff::InvDist2); 

    for(int i = 0; i < particleCount-1; i++){
        world.rodSystem.addRod(i, i+1, 1.73, 1); 

        if(i < particleCount-2){
            world.rodSystem.addRod(i, i+2, 1.73*2, 1); 
        }

        if(i < particleCount-4){
            world.rodSystem.addRod(i, i+4, 1.73*4, 1); 
        }
    }

    perfTimer.reset(); 
    int N = 1000; 
    for(int i = 0; i < N; i++){
        world.update(); 
    }
    perfTimer.lap();

    Particle* p = world.getParticlesPtr(); 
    for(int i = 0; i < particleCount; i++){
        std::cout << "posx:" << p[i].position.x << ", " << "posy:" << p[i].position.y << ", " << "posz:" << p[i].position.z << "\n"; 
    }
    double time = perfTimer.times[0]; 
    std::cout << "av update time: " << time/N << "\n"; 

    return 0; 
}