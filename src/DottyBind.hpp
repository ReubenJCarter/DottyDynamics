#pragma once

#define DLLPROJECT_EXPORTS 

#ifdef DLLPROJECT_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#include "World.hpp"

extern "C" {
    EXPORT char* nativeTest();
    
    EXPORT World* createWorld();
    EXPORT void freeWorld(World* instance);
    
    EXPORT void World_update(World* instance);

    EXPORT void World_setGravity(World* instance, float gravity); 
    EXPORT void World_setTimestep(World* instance, float timestep); 
    EXPORT void World_setGlobalDamping(World* instance, float globalDamping); 

    EXPORT int World_addParticle(World* instance, Vec3 initialPosition, Vec3 initialVelocity, float invMass); 
    EXPORT void World_destroyParticle(World* instance, int inx); 
    EXPORT Particle* World_getParticlesPtr(World* instance);
    EXPORT bool* World_getParticlesInUsePtr(World* instance);
    EXPORT int World_getParticlesPoolBound(World* instance);

    EXPORT int World_addAttrator(World* instance, Vec3 position, float strength); 
    EXPORT void World_destroyAttractor(World* instance, int inx); 
    EXPORT void World_setAttractorPosition(World* instance, int inx, Vec3 position);
    EXPORT void World_setAttractorStrength(World* instance, int inx, float strength);

    EXPORT int World_addRod(World* instance, int a, int b, float length, float strength);
    EXPORT void World_destroyRod(World* instance, int inx);
    EXPORT void World_setRodStrength(World* instance, int inx, float strength);
    EXPORT void World_setRodLength(World* instance, int inx, float length);
}