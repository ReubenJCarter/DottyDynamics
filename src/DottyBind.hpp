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
    EXPORT void World_setSubsteps(World* instance, float substeps); 
    EXPORT void World_setGlobalDamping(World* instance, float globalDamping); 

    EXPORT int World_addParticle(World* instance, Vec3 initialPosition, Vec3 initialVelocity, float invMass); 
    EXPORT void World_destroyParticle(World* instance, int inx); 
    EXPORT Particle* World_getParticlesPtr(World* instance);
    EXPORT bool* World_getParticlesInUsePtr(World* instance);
    EXPORT int World_getParticlesPoolBound(World* instance);

    EXPORT int World_addAttrator(World* instance, Vec3 position, float strength, float minDist, float maxDist, Falloff falloff); 
    EXPORT void World_destroyAttractor(World* instance, int inx); 
    EXPORT void World_setAttractorPosition(World* instance, int inx, Vec3 position);
    EXPORT void World_setAttractorStrength(World* instance, int inx, float strength);

    EXPORT int World_addRod(World* instance, int a, int b, float length, float stiffness);
    EXPORT void World_destroyRod(World* instance, int inx);
    EXPORT void World_setRodStiffness(World* instance, int inx, float stiffness);
    EXPORT void World_setRodLength(World* instance, int inx, float length);

    EXPORT int World_addAnchorRod(World* instance, int a, Vec3 position, float length, float stiffness);
    EXPORT void World_destroyAnchorRod(World* instance, int inx);
    EXPORT void World_setAnchorRodStiffness(World* instance, int inx, float stiffness);
    EXPORT void World_setAnchorRodLength(World* instance, int inx, float length);
    EXPORT void World_setAnchorRodPosition(World* instance, int inx, Vec3 position);

    EXPORT int World_addNoiseField(World* instance, NoiseType noiseType, float strength, float noiseScale);
    EXPORT void World_destroyNoiseField(World* instance, int inx); 
}