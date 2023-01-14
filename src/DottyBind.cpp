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


EXPORT WorldParams* World_getWorldParamsPtr(World* instance){
    return instance->getWorldParamsPtr(); 
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



EXPORT int World_addGlobalForce(World* instance, Vec3 position, Vec3 direction, float strength, Vec3 boundSize, BoundShapeType boundShape, float boundThickness, Falloff boundFalloff){
    return instance->addGlobalForce(position, direction, strength, boundSize, boundShape, boundThickness, boundFalloff); 
}

EXPORT GlobalForce* World_getGlobalForcePtr(World* instance, int inx){
    return instance->getGlobalForcePtr(inx); 
}

EXPORT void World_destroyGlobalForce(World* instance, int inx){
    instance->destroyGlobalForce(inx); 
}

EXPORT void World_clearGlobalForces(World* instance){
    instance->clearGlobalForces(); 
}

EXPORT void World_setGlobalForcePosition(World* instance, int inx, Vec3 position){
    instance->setGlobalForcePosition(inx, position); 
}

EXPORT void World_setGlobalForceDirection(World* instance, int inx, Vec3 direction){
    instance->setGlobalForceDirection(inx, direction); 
}

EXPORT void World_setGlobalForceStrength(World* instance, int inx, float strength){
    instance->setGlobalForceStrength(inx, strength); 
}

EXPORT void World_setGlobalForceBoundSize(World* instance, int inx, Vec3 boundSize){
    instance->setGlobalForceBoundSize(inx, boundSize); 
}

EXPORT void World_setGlobalForceBoundShape(World* instance, int inx, BoundShapeType boundShape){
    instance->setGlobalForceBoundShape(inx, boundShape); 
}

EXPORT void World_setGlobalForceBoundThickness(World* instance, int inx, float boundThickness){
    instance->setGlobalForceBoundThickness(inx, boundThickness);
}

EXPORT void World_setGlobalForceBoundFalloff(World* instance, int inx, Falloff boundFalloff){
    instance->setGlobalForceBoundFalloff(inx, boundFalloff);
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



EXPORT int World_addStrangeAttrator(World* instance, Vec3 position, Vec3 scale, StrangeAttractorType type, float strength, float minDist, float maxDist, Falloff falloff){
    return instance->addStrangeAttrator(position, scale, type, strength, minDist, maxDist, falloff); 
}

EXPORT StrangeAttractor* World_getStrangeAttractorPtr(World* instance, int inx){
    return instance->getStrangeAttractorPtr(inx);
}

EXPORT void World_destroyStrangeAttractor(World* instance, int inx){
    instance->destroyStrangeAttractor(inx);
}

EXPORT void World_clearStrangeAttractors(World* instance){
    instance->clearStrangeAttractors(); 
}

EXPORT void World_setStrangeAttractorPosition(World* instance, int inx, Vec3 position){
    instance->setStrangeAttractorPosition(inx, position);
}

EXPORT void World_setStrangeAttractorScale(World* instance, int inx, Vec3 scale){
    instance->setStrangeAttractorScale(inx, scale);
}

EXPORT void World_setStrangeAttractorType(World* instance, int inx, StrangeAttractorType type){
    instance->setStrangeAttractorType(inx, type);
}

EXPORT void World_setStrangeAttractorStrength(World* instance, int inx, float strength){
    instance->setStrangeAttractorStrength(inx, strength);
}

EXPORT void World_setStrangeAttractorMinDist(World* instance, int inx, float minDist){
    instance->setStrangeAttractorMinDist(inx, minDist); 
}

EXPORT void World_setStrangeAttractorMaxDist(World* instance, int inx, float maxDist){
    instance->setStrangeAttractorMaxDist(inx, maxDist); 
}

EXPORT void World_setStrangeAttractorFalloff(World* instance, int inx, Falloff falloff){
    instance->setStrangeAttractorFalloff(inx, falloff); 
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



EXPORT int World_addNoiseField(World* instance, Vec3 position, NoiseType noiseType, float strength, float noiseScale, FieldMode mode, 
                          BoundShapeType boundShape, Vec3 boundSize, Falloff boundFalloff, float boundThickness){
    return instance->addNoiseField(position, noiseType, strength, noiseScale, mode, boundShape, boundSize, boundFalloff, boundThickness); 
}

EXPORT void World_destroyNoiseField(World* instance, int inx){
    instance->destroyNoiseField(inx); 
}

EXPORT void World_clearNoiseFields(World* instance){
    instance->clearNoiseFields(); 
}

EXPORT void World_setNoiseFieldPosition(World* instance, int inx, Vec3 position){
    instance->setNoiseFieldPosition(inx, position); 
}

EXPORT void World_setNoiseFieldNoiseType(World* instance, int inx, NoiseType noiseType){
    instance->setNoiseFieldNoiseType(inx, noiseType);
}

EXPORT void World_setNoiseFieldStrength(World* instance, int inx, float strength){
    instance->setNoiseFieldStrength(inx, strength); 
}

EXPORT void World_setNoiseFieldNoiseScale(World* instance, int inx, float noiseScale){
    instance->setNoiseFieldNoiseScale(inx, noiseScale); 
}

EXPORT void World_setNoiseMode(World* instance, int inx, FieldMode mode){
    instance->setNoiseFieldMode(inx, mode); 
}

EXPORT void World_setNoiseFieldBoundShape(World* instance, int inx, BoundShapeType boundShape){
    instance->setNoiseFieldBoundShape(inx, boundShape); 
}

EXPORT void World_setNoiseFieldBoundSize(World* instance, int inx, Vec3 boundSize){
    instance->setNoiseFieldBoundSize(inx, boundSize); 
}

EXPORT void World_setNoiseFieldBounFalloff(World* instance, int inx, Falloff boundFalloff){
    instance->setNoiseFieldBoundFalloff(inx, boundFalloff); 
}

EXPORT void World_setNoiseFieldBounThickness(World* instance, int inx, float thickness){
    instance->setNoiseFieldBoundThickness(inx, thickness); 
}



EXPORT int World_addSphereCollider(World* instance, Vec3 position, float radius, float kineticFriction, float staticFriction, bool inverse){
    return instance->addSphereCollider(position, radius, kineticFriction, staticFriction, inverse); 
}

EXPORT SphereCollider* World_getSphereColliderPtr(World* instance, int inx){
    return instance->getSphereColliderPtr(inx); 
}

EXPORT void World_destroySphereCollider(World* instance, int inx){
    instance->destroySphereCollider(inx);
}

EXPORT void World_clearSphereColliders(World* instance){
    instance->clearSphereColliders();
}

EXPORT void World_setSphereColliderPosition(World* instance, int inx, Vec3 position){
    instance->setSphereColliderPosition(inx, position); 
}

EXPORT void World_setSphereColliderRadius(World* instance, int inx, float radius){
    instance->setSphereColliderRadius(inx, radius); 
}

EXPORT void World_setSphereColliderKineticFriction(World* instance, int inx, float kineticFriction){
    instance->setSphereColliderKineticFriction(inx, kineticFriction); 
}

EXPORT void World_setSphereColliderStaticFriction(World* instance, int inx, float staticFriction){
    instance->setSphereColliderStaticFriction(inx, staticFriction); 
}

EXPORT void World_setSphereColliderInverse(World* instance, int inx, bool inverse){
    instance->setSphereColliderInverse(inx, inverse);
}



EXPORT int World_addBoxCollider(World* instance, Vec3 position, Mat3 invRotation, Vec3 size, float kineticFriction, float staticFriction, bool inverse){
    return instance->addBoxCollider(position, invRotation, size, kineticFriction, staticFriction, inverse); 
}

EXPORT BoxCollider* World_getBoxColliderPtr(World* instance, int inx){
    return instance->getBoxColliderPtr(inx); 
}

EXPORT void World_destroyBoxCollider(World* instance, int inx){
    instance->destroyBoxCollider(inx); 
}

EXPORT void World_clearBoxColliders(World* instance){
    instance->clearBoxColliders(); 
}

EXPORT void World_setBoxColliderPosition(World* instance, int inx, Vec3 position){
    instance->setBoxColliderPosition(inx, position); 
}

EXPORT void World_setBoxColliderInvRotation(World* instance, int inx, Mat3 invRotation){
    instance->setBoxColliderInvRotation(inx, invRotation); 
}

EXPORT void World_setBoxColliderSize(World* instance, int inx, Vec3 size){
    instance->setBoxColliderSize(inx, size); 
}

EXPORT void World_setBoxColliderKineticFriction(World* instance, int inx, float kineticFriction){
    instance->setBoxColliderKineticFriction(inx, kineticFriction); 
}

EXPORT void World_setBoxColliderStaticFriction(World* instance, int inx, float staticFriction){
    instance->setBoxColliderStaticFriction(inx, staticFriction); 
}

EXPORT void World_setBoxColliderInverse(World* instance, int inx, bool inverse){
    instance->setBoxColliderInverse(inx, inverse); 
}