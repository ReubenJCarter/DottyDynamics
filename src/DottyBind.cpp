#include "DottyBind.hpp"

EXPORT char* nativeTest(){
    return "Hello World\n"; 
}


EXPORT World* createWorld(){
    return new World();
}

EXPORT void freeWorld(World* instance){
    delete instance;
}


EXPORT void World_update(World* instance){
    instance->update(); 
}


EXPORT void World_setGravity(World* instance, float gravity){
    instance->setGravity(gravity); 
}

EXPORT void World_setTimestep(World* instance, float timestep){
    instance->setTimestep(timestep); 
}

EXPORT void World_setSubsteps(World* instance, float substeps){
    instance->setSubsteps(substeps);
}

EXPORT void World_setGlobalDamping(World* instance, float globalDamping){
    instance->setGlobalDamping(globalDamping); 
}

EXPORT void World_setHasCollisionFloor(World* instance, bool hasCollisionFloor){
    instance->setHasCollisionFloor(hasCollisionFloor); 
}

EXPORT void World_setCollisionFloor(World* instance, float height, float staticFriction, float kineticFriction){
    instance->setCollisionFloor(height, staticFriction, kineticFriction); 
}



EXPORT int World_addParticle(World* instance, Vec3 initialPosition, Vec3 initialVelocity, float invMass){
    return instance->addParticle(initialPosition, initialVelocity, invMass); 
}

EXPORT void World_destroyParticle(World* instance, int inx){
    instance->destroyParticle(inx);
}

EXPORT Particle* World_getParticlesPtr(World* instance){
    return instance->getParticlesPtr(); 
}

EXPORT bool* World_getParticlesInUsePtr(World* instance){
    return instance->getParticlesInUsePtr();   
}

EXPORT int World_getParticlesPoolBound(World* instance){
    return instance->getParticlesPoolBounds(); 
}



EXPORT int World_addAttrator(World* instance, Vec3 position, float strength, float minDist, float maxDist, Falloff falloff){
    return instance->addAttrator(position, strength, minDist, maxDist, falloff); 
}

EXPORT Attractor* World_getAttractorPtr(World* instance, int inx){
    return instance->getAttractorPtr(inx);
}

EXPORT void World_destroyAttractor(World* instance, int inx){
    instance->destroyAttractor(inx);
}

EXPORT void World_clearAttractors(World* instance){
    instance->clearAttractors(); 
}

EXPORT void World_setAttractorPosition(World* instance, int inx, Vec3 position){
    instance->setAttractorPosition(inx, position);
}

EXPORT void World_setAttractorStrength(World* instance, int inx, float strength){
    instance->setAttractorStrength(inx, strength);
}

EXPORT void World_setAttractorMinDist(World* instance, int inx, float minDist){
    instance->setAttractorMinDist(inx, minDist); 
}

EXPORT void World_setAttractorMaxDist(World* instance, int inx, float maxDist){
    instance->setAttractorMaxDist(inx, maxDist); 
}

EXPORT void World_setAttractorFalloff(World* instance, int inx, Falloff falloff){
    instance->setAttractorFalloff(inx, falloff); 
}



EXPORT int World_addVortex(World* instance, Vec3 position, Vec3 normal, float strength, float minDist, float maxDist, Falloff falloff){
    return instance->addVortex(position, normal, strength, minDist, maxDist, falloff);
}

EXPORT Vortex* World_getVortexPtr(World* instance, int inx){
    return instance->getVortexPtr(inx);
}

EXPORT void World_destroyVortex(World* instance, int inx){
    instance->destroyVortex(inx); 
}

EXPORT void World_clearVortices(World* instance){
    instance->clearVortices(); 
}

EXPORT void World_setVortexPosition(World* instance, int inx, Vec3 position){
    instance->setVortexPosition(inx, position); 
}

EXPORT void World_setVortexNormal(World* instance, int inx, Vec3 normal){
    instance->setVortexNormal(inx, normal); 
}

EXPORT void World_setVortexStrength(World* instance, int inx, float strength){
    instance->setVortexStrength(inx, strength); 
}

EXPORT void World_setVortexMinDist(World* instance, int inx, float minDist){
    instance->setVortexMinDist(inx, minDist); 
}

EXPORT void World_setVortexMaxDist(World* instance, int inx, float maxDist){
    instance->setVortexMaxDist(inx, maxDist); 
}

EXPORT void World_setVortexFalloff(World* instance, int inx, Falloff falloff){
    instance->setVortexFalloff(inx, falloff); 
}




EXPORT int World_addRod(World* instance, int a, int b, float length, float stiffness){
    return instance->addRod(a, b, length, stiffness); 
}

EXPORT void World_destroyRod(World* instance, int inx){
    instance->destroyRod(inx);
}

EXPORT void World_setRodStiffness(World* instance, int inx, float stiffness){
    instance->setRodStiffness(inx, stiffness); 
}

EXPORT void World_setRodLength(World* instance, int inx, float length){
    instance->setRodLength(inx, length); 
}



EXPORT int World_addAnchorRod(World* instance, int a, Vec3 position, float length, float stiffness){
    return instance->addAnchorRod(a, position, length, stiffness); 
}

EXPORT void World_destroyAnchorRod(World* instance, int inx){
    instance->destroyAnchorRod(inx);
}

EXPORT void World_setAnchorRodStiffness(World* instance, int inx, float stiffness){
    instance->setAnchorRodStiffness(inx, stiffness); 
}

EXPORT void World_setAnchorRodLength(World* instance, int inx, float length){
    instance->setAnchorRodLength(inx, length); 
}

EXPORT void World_setAnchorRodPosition(World* instance, int inx, Vec3 position){
    instance->setAnchorRodPosition(inx, position);
}



EXPORT int World_addNoiseField(World* instance, NoiseType noiseType, float strength, float noiseScale, bool isVelocity){
    return instance->addNoiseField(noiseType, strength, noiseScale, isVelocity); 
}

EXPORT void World_setNoiseFieldViscosity(World* instance, int inx, float viscosity){
    instance->setNoiseFieldViscosity(inx, viscosity); 
}

EXPORT void World_destroyNoiseField(World* instance, int inx){
    instance->destroyNoiseField(inx); 
}

EXPORT void World_clearNoiseFields(World* instance){
    instance->clearNoiseFields(); 
}