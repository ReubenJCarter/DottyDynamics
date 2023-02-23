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

EXPORT void World_resetParticlePosition(World* instance, int inx, Vec3 position){
    instance->resetParticlePosition(inx, position); 
}

EXPORT void World_addParticleForce(World* instance, int inx, Vec3 force){
    instance->addParticleForce(inx, force);
}

EXPORT void World_addParticleImpulse(World* instance, int inx, Vec3 impulse){
    instance->addParticleImpulse(inx, impulse);
}

EXPORT void World_addParticlePositionDelta(World* instance, int inx, Vec3 delta){
    instance->addParticlePositionDelta(inx, delta);
}

EXPORT void World_setParticleLayerEnabled(World* instance, int inx, int layer, bool enabled){
    instance->setParticleLayerEnabled(inx, layer, enabled); 
}

EXPORT void World_zeroParticleLayerMask(World* instance, int inx){
    instance->zeroParticleLayerMask(inx); 
}



EXPORT int World_addGlobalForce(World* instance, Vec3 position, Vec3 direction, float strength, Vec3 boundSize, BoundShapeType boundShape, 
                                float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation){
    return instance->globalForceSystem.addGlobalForce(position, direction, strength, boundSize, boundShape, boundThickness, boundFalloff, boundInvRotation); 
}

EXPORT GlobalForce* World_getGlobalForcePtr(World* instance, int inx){
    return instance->globalForceSystem.getGlobalForcePtr(inx); 
}

EXPORT void World_destroyGlobalForce(World* instance, int inx){
    instance->globalForceSystem.destroyGlobalForce(inx); 
}

EXPORT void World_clearGlobalForces(World* instance){
    instance->globalForceSystem.clearGlobalForces(); 
}

EXPORT void World_setGlobalForceLayerEnabled(World* instance, int inx, int layer, bool enabled){
    instance->globalForceSystem.setGlobalForceLayerEnabled(inx, layer, enabled); 
} 

EXPORT void World_zeroGlobalForceLayerMask(World* instance, int inx){
    instance->globalForceSystem.zeroGlobalForceLayerMask(inx); 
}   

EXPORT void World_setGlobalForcePosition(World* instance, int inx, Vec3 position){
    instance->globalForceSystem.setGlobalForcePosition(inx, position); 
}

EXPORT void World_setGlobalForceDirection(World* instance, int inx, Vec3 direction){
    instance->globalForceSystem.setGlobalForceDirection(inx, direction); 
}

EXPORT void World_setGlobalForceStrength(World* instance, int inx, float strength){
    instance->globalForceSystem.setGlobalForceStrength(inx, strength); 
}

EXPORT void World_setGlobalForceBoundSize(World* instance, int inx, Vec3 boundSize){
    instance->globalForceSystem.setGlobalForceBoundSize(inx, boundSize); 
}

EXPORT void World_setGlobalForceBoundShape(World* instance, int inx, BoundShapeType boundShape){
    instance->globalForceSystem.setGlobalForceBoundShape(inx, boundShape); 
}

EXPORT void World_setGlobalForceBoundThickness(World* instance, int inx, float boundThickness){
    instance->globalForceSystem.setGlobalForceBoundThickness(inx, boundThickness);
}

EXPORT void World_setGlobalForceBoundFalloff(World* instance, int inx, BoundFalloff boundFalloff){
    instance->globalForceSystem.setGlobalForceBoundFalloff(inx, boundFalloff);
}

EXPORT void World_setGlobalForceBoundInvRotation(World* instance, int inx, Mat3 invRotation){
    instance->globalForceSystem.setGlobalForceBoundInvRotation(inx, invRotation);
}



EXPORT int World_addAttrator(World* instance, Vec3 position, float strength, float minDist, float maxDist, Falloff falloff){
    return instance->attractorSystem.addAttrator(position, strength, minDist, maxDist, falloff); 
}

EXPORT Attractor* World_getAttractorPtr(World* instance, int inx){
    return instance->attractorSystem.getAttractorPtr(inx);
}

EXPORT void World_destroyAttractor(World* instance, int inx){
    instance->attractorSystem.destroyAttractor(inx);
}

EXPORT void World_clearAttractors(World* instance){
    instance->attractorSystem.clearAttractors(); 
}

EXPORT void World_setAttractorLayerEnabled(World* instance, int inx, int layer, bool enabled){
    instance->attractorSystem.setAttractorLayerEnabled(inx, layer, enabled); 
} 

EXPORT void World_zeroAttractorLayerMask(World* instance, int inx){
    instance->attractorSystem.zeroAttractorLayerMask(inx); 
} 

EXPORT void World_setAttractorPosition(World* instance, int inx, Vec3 position){
    instance->attractorSystem.setAttractorPosition(inx, position);
}

EXPORT void World_setAttractorStrength(World* instance, int inx, float strength){
    instance->attractorSystem.setAttractorStrength(inx, strength);
}

EXPORT void World_setAttractorMinDist(World* instance, int inx, float minDist){
    instance->attractorSystem.setAttractorMinDist(inx, minDist); 
}

EXPORT void World_setAttractorMaxDist(World* instance, int inx, float maxDist){
    instance->attractorSystem.setAttractorMaxDist(inx, maxDist); 
}

EXPORT void World_setAttractorFalloff(World* instance, int inx, Falloff falloff){
    instance->attractorSystem.setAttractorFalloff(inx, falloff); 
}



EXPORT int World_addStrangeAttractor(World* instance, Vec3 position, float scale, StrangeAttractorType type, float strength, float targetSpeed, FieldMode fieldMode, 
                                     float minDist, float maxDist, Falloff falloff, float a, float b, float c, float d, float e, float f){
    return instance->strangeAttractorSystem.addStrangeAttractor(position, scale, type, strength, targetSpeed, fieldMode, minDist, maxDist, falloff, a, b, c, d, e, f); 
}

EXPORT StrangeAttractor* World_getStrangeAttractorPtr(World* instance, int inx){
    return instance->strangeAttractorSystem.getStrangeAttractorPtr(inx);
}

EXPORT void World_destroyStrangeAttractor(World* instance, int inx){
    instance->strangeAttractorSystem.destroyStrangeAttractor(inx);
}

EXPORT void World_clearStrangeAttractors(World* instance){
    instance->strangeAttractorSystem.clearStrangeAttractors(); 
}

EXPORT void World_setStrangeAttractorLayerEnabled(World* instance, int inx, int layer, bool enabled){
    instance->strangeAttractorSystem.setStrangeAttractorLayerEnabled(inx, layer, enabled); 
} 

EXPORT void World_zeroStrangeAttractorLayerMask(World* instance, int inx){
    instance->strangeAttractorSystem.zeroStrangeAttractorLayerMask(inx); 
} 

EXPORT void World_setStrangeAttractorPosition(World* instance, int inx, Vec3 position){
    instance->strangeAttractorSystem.setStrangeAttractorPosition(inx, position);
}

EXPORT void World_setStrangeAttractorScale(World* instance, int inx, float scale){
    instance->strangeAttractorSystem.setStrangeAttractorScale(inx, scale);
}

EXPORT void World_setStrangeAttractorType(World* instance, int inx, StrangeAttractorType type){
    instance->strangeAttractorSystem.setStrangeAttractorType(inx, type);
}

EXPORT void World_setStrangeAttractorStrength(World* instance, int inx, float strength){
    instance->strangeAttractorSystem.setStrangeAttractorStrength(inx, strength);
}

EXPORT void World_setStrangeAttractorTargetSpeed(World* instance, int inx, float targetSpeed){
    instance->strangeAttractorSystem.setStrangeAttractorTargetSpeed(inx, targetSpeed); 
}

EXPORT void World_setStrangeAttractorFieldMode(World* instance, int inx, FieldMode fieldMode){
    instance->strangeAttractorSystem.setStrangeAttractorFieldMode(inx, fieldMode);
}

EXPORT void World_setStrangeAttractorMinDist(World* instance, int inx, float minDist){
    instance->strangeAttractorSystem.setStrangeAttractorMinDist(inx, minDist); 
}

EXPORT void World_setStrangeAttractorMaxDist(World* instance, int inx, float maxDist){
    instance->strangeAttractorSystem.setStrangeAttractorMaxDist(inx, maxDist); 
}

EXPORT void World_setStrangeAttractorFalloff(World* instance, int inx, Falloff falloff){
    instance->strangeAttractorSystem.setStrangeAttractorFalloff(inx, falloff); 
}

EXPORT void World_setStrangeAttractorCoeffs(World* instance, int inx, float a, float b, float c, float d, float e, float f){
    instance->strangeAttractorSystem.setStrangeAttractorCoeffs(inx, a, b, c, d, e, f); 
}



EXPORT int World_addVortex(World* instance, Vec3 position, Vec3 normal, float strength, float minDist, float maxDist, Falloff falloff){
    return instance->vortexSystem.addVortex(position, normal, strength, minDist, maxDist, falloff);
}

EXPORT Vortex* World_getVortexPtr(World* instance, int inx){
    return instance->vortexSystem.getVortexPtr(inx);
}

EXPORT void World_destroyVortex(World* instance, int inx){
    instance->vortexSystem.destroyVortex(inx); 
}

EXPORT void World_clearVortices(World* instance){
    instance->vortexSystem.clearVortices(); 
}

EXPORT void World_setVortexLayerEnabled(World* instance, int inx, int layer, bool enabled){
    instance->vortexSystem.setVortexLayerEnabled(inx, layer, enabled); 
} 

EXPORT void World_zeroVortexLayerMask(World* instance, int inx){
    instance->vortexSystem.zeroVortexLayerMask(inx); 
} 

EXPORT void World_setVortexPosition(World* instance, int inx, Vec3 position){
    instance->vortexSystem.setVortexPosition(inx, position); 
}

EXPORT void World_setVortexNormal(World* instance, int inx, Vec3 normal){
    instance->vortexSystem.setVortexNormal(inx, normal); 
}

EXPORT void World_setVortexStrength(World* instance, int inx, float strength){
    instance->vortexSystem.setVortexStrength(inx, strength); 
}

EXPORT void World_setVortexMinDist(World* instance, int inx, float minDist){
    instance->vortexSystem.setVortexMinDist(inx, minDist); 
}

EXPORT void World_setVortexMaxDist(World* instance, int inx, float maxDist){
    instance->vortexSystem.setVortexMaxDist(inx, maxDist); 
}

EXPORT void World_setVortexFalloff(World* instance, int inx, Falloff falloff){
    instance->vortexSystem.setVortexFalloff(inx, falloff); 
}




EXPORT int World_addRod(World* instance, int a, int b, float length, float stiffness){
    return instance->rodSystem.addRod(a, b, length, stiffness); 
}

EXPORT void World_destroyRod(World* instance, int inx){
    instance->rodSystem.destroyRod(inx);
}

EXPORT void World_setRodStiffness(World* instance, int inx, float stiffness){
    instance->rodSystem.setRodStiffness(inx, stiffness); 
}

EXPORT void World_setRodLength(World* instance, int inx, float length){
    instance->rodSystem.setRodLength(inx, length); 
}



EXPORT int World_addAnchorRod(World* instance, int a, Vec3 position, float length, float stiffness){
    return instance->rodSystem.addAnchorRod(a, position, length, stiffness); 
}

EXPORT void World_destroyAnchorRod(World* instance, int inx){
    instance->rodSystem.destroyAnchorRod(inx);
}

EXPORT void World_setAnchorRodStiffness(World* instance, int inx, float stiffness){
    instance->rodSystem.setAnchorRodStiffness(inx, stiffness); 
}

EXPORT void World_setAnchorRodLength(World* instance, int inx, float length){
    instance->rodSystem.setAnchorRodLength(inx, length); 
}

EXPORT void World_setAnchorRodPosition(World* instance, int inx, Vec3 position){
    instance->rodSystem.setAnchorRodPosition(inx, position);
}



EXPORT int World_addAngleConstraint(World* instance, int a, int b, int c, float angle, float stiffness){
    return instance->angleConstraintSystem.addAngleConstraint(a, b, c, angle, stiffness); 
}

EXPORT void World_destroyAngleConstraint(World* instance, int inx){
    instance->angleConstraintSystem.destroyAngleConstraint(inx);
}

EXPORT void World_setAngleConstraintStiffness(World* instance, int inx, float stiffness){
    instance->angleConstraintSystem.setAngleConstraintStiffness(inx, stiffness); 
}

EXPORT void World_setAngleConstraintAngle(World* instance, int inx, float angle){
    instance->angleConstraintSystem.setAngleConstraintAngle(inx, angle); 
}



EXPORT int World_addNoiseField(World* instance, Vec3 position, NoiseType noiseType, float strength, float targetSpeed, float noiseScale, FieldMode mode, 
                               Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation, int bakeResolution, bool useBake){
    return instance->noiseFieldSystem.addNoiseField(position, noiseType, strength, targetSpeed, noiseScale, mode, 
                                                    boundSize, boundShape, boundThickness, boundFalloff, boundInvRotation, bakeResolution, useBake); 
}

EXPORT NoiseField* World_getNoiseFieldPtr(World* instance, int inx){
    return instance->noiseFieldSystem.getNoiseFieldPtr(inx); 
}

EXPORT void World_destroyNoiseField(World* instance, int inx){
    instance->noiseFieldSystem.destroyNoiseField(inx); 
}

EXPORT void World_clearNoiseFields(World* instance){
    instance->noiseFieldSystem.clearNoiseFields(); 
}

EXPORT void World_setNoiseFieldLayerEnabled(World* instance, int inx, int layer, bool enabled){
    instance->noiseFieldSystem.setNoiseFieldLayerEnabled(inx, layer, enabled); 
} 

EXPORT void World_zeroNoiseFieldLayerMask(World* instance, int inx){
    instance->noiseFieldSystem.zeroNoiseFieldLayerMask(inx); 
} 

EXPORT void World_setNoiseFieldPosition(World* instance, int inx, Vec3 position){
    instance->noiseFieldSystem.setNoiseFieldPosition(inx, position); 
}

EXPORT void World_setNoiseFieldNoiseType(World* instance, int inx, NoiseType noiseType){
    instance->noiseFieldSystem.setNoiseFieldNoiseType(inx, noiseType);
}

EXPORT void World_setNoiseFieldStrength(World* instance, int inx, float strength){
    instance->noiseFieldSystem.setNoiseFieldStrength(inx, strength); 
}

EXPORT void World_setNoiseFieldTargetSpeed(World* instance, int inx, float targetSpeed){
    instance->noiseFieldSystem.setNoiseFieldTargetSpeed(inx, targetSpeed); 
}

EXPORT void World_setNoiseFieldNoiseScale(World* instance, int inx, float noiseScale){
    instance->noiseFieldSystem.setNoiseFieldNoiseScale(inx, noiseScale); 
}

EXPORT void World_setNoiseFieldMode(World* instance, int inx, FieldMode mode){
    instance->noiseFieldSystem.setNoiseFieldMode(inx, mode); 
}

EXPORT void World_setNoiseFieldBoundShape(World* instance, int inx, BoundShapeType boundShape){
    instance->noiseFieldSystem.setNoiseFieldBoundShape(inx, boundShape); 
}

EXPORT void World_setNoiseFieldBoundSize(World* instance, int inx, Vec3 boundSize){
    instance->noiseFieldSystem.setNoiseFieldBoundSize(inx, boundSize); 
}

EXPORT void World_setNoiseFieldBoundFalloff(World* instance, int inx, BoundFalloff boundFalloff){
    instance->noiseFieldSystem.setNoiseFieldBoundFalloff(inx, boundFalloff); 
}

EXPORT void World_setNoiseFieldBoundThickness(World* instance, int inx, float thickness){
    instance->noiseFieldSystem.setNoiseFieldBoundThickness(inx, thickness); 
}

EXPORT void World_setNoiseFieldBoundInvRotation(World* instance, int inx, Mat3 invRotation){
    instance->noiseFieldSystem.setNoiseFieldBoundInvRotation(inx, invRotation);
}

EXPORT void World_setNoiseFieldBakeResolution(World* instance, int inx, int resolution){
    instance->noiseFieldSystem.setNoiseFieldBakeResolution(inx, resolution); 
}

EXPORT void World_setNoiseFieldUseBake(World* instance, int inx, bool useBake){
    instance->noiseFieldSystem.setNoiseFieldUseBake(inx, useBake); 
}



EXPORT int World_addSphereCollider(World* instance, Vec3 position, float radius, float kineticFriction, float staticFriction, bool inverse){
    return instance->sphereColliderSystem.addSphereCollider(position, radius, kineticFriction, staticFriction, inverse); 
}

EXPORT SphereCollider* World_getSphereColliderPtr(World* instance, int inx){
    return instance->sphereColliderSystem.getSphereColliderPtr(inx); 
}

EXPORT void World_destroySphereCollider(World* instance, int inx){
    instance->sphereColliderSystem.destroySphereCollider(inx);
}

EXPORT void World_clearSphereColliders(World* instance){
    instance->sphereColliderSystem.clearSphereColliders();
}

EXPORT void World_setSphereColliderLayerEnabled(World* instance, int inx, int layer, bool enabled){
    instance->sphereColliderSystem.setSphereColliderLayerEnabled(inx, layer, enabled); 
} 

EXPORT void World_zeroSphereColliderLayerMask(World* instance, int inx){
    instance->sphereColliderSystem.zeroSphereColliderLayerMask(inx); 
} 

EXPORT void World_setSphereColliderPosition(World* instance, int inx, Vec3 position){
    instance->sphereColliderSystem.setSphereColliderPosition(inx, position); 
}

EXPORT void World_setSphereColliderRadius(World* instance, int inx, float radius){
    instance->sphereColliderSystem.setSphereColliderRadius(inx, radius); 
}

EXPORT void World_setSphereColliderKineticFriction(World* instance, int inx, float kineticFriction){
    instance->sphereColliderSystem.setSphereColliderKineticFriction(inx, kineticFriction); 
}

EXPORT void World_setSphereColliderStaticFriction(World* instance, int inx, float staticFriction){
    instance->sphereColliderSystem.setSphereColliderStaticFriction(inx, staticFriction); 
}

EXPORT void World_setSphereColliderInverse(World* instance, int inx, bool inverse){
    instance->sphereColliderSystem.setSphereColliderInverse(inx, inverse);
}



EXPORT int World_addBoxCollider(World* instance, Vec3 position, Mat3 invRotation, Vec3 size, float kineticFriction, float staticFriction, bool inverse){
    return instance->boxColliderSystem.addBoxCollider(position, invRotation, size, kineticFriction, staticFriction, inverse); 
}

EXPORT BoxCollider* World_getBoxColliderPtr(World* instance, int inx){
    return instance->boxColliderSystem.getBoxColliderPtr(inx); 
}

EXPORT void World_destroyBoxCollider(World* instance, int inx){
    instance->boxColliderSystem.destroyBoxCollider(inx); 
}

EXPORT void World_clearBoxColliders(World* instance){
    instance->boxColliderSystem.clearBoxColliders(); 
}

EXPORT void World_setBoxColliderLayerEnabled(World* instance, int inx, int layer, bool enabled){
    instance->boxColliderSystem.setBoxColliderLayerEnabled(inx, layer, enabled); 
} 

EXPORT void World_zeroBoxColliderLayerMask(World* instance, int inx){
    instance->boxColliderSystem.zeroBoxColliderLayerMask(inx); 
} 

EXPORT void World_setBoxColliderPosition(World* instance, int inx, Vec3 position){
    instance->boxColliderSystem.setBoxColliderPosition(inx, position); 
}

EXPORT void World_setBoxColliderInvRotation(World* instance, int inx, Mat3 invRotation){
    instance->boxColliderSystem.setBoxColliderInvRotation(inx, invRotation); 
}

EXPORT void World_setBoxColliderSize(World* instance, int inx, Vec3 size){
    instance->boxColliderSystem.setBoxColliderSize(inx, size); 
}

EXPORT void World_setBoxColliderKineticFriction(World* instance, int inx, float kineticFriction){
    instance->boxColliderSystem.setBoxColliderKineticFriction(inx, kineticFriction); 
}

EXPORT void World_setBoxColliderStaticFriction(World* instance, int inx, float staticFriction){
    instance->boxColliderSystem.setBoxColliderStaticFriction(inx, staticFriction); 
}

EXPORT void World_setBoxColliderInverse(World* instance, int inx, bool inverse){
    instance->boxColliderSystem.setBoxColliderInverse(inx, inverse); 
}



EXPORT int World_addDamper(World* instance, Vec3 position, float strength, 
                                    Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation){
    return instance->damperSystem.addDamper(position, strength, boundSize, boundShape, boundThickness, boundFalloff, boundInvRotation); 
}

EXPORT Damper* World_getDamperPtr(World* instance, int inx){
    return instance->damperSystem.getDamperPtr(inx); 
}

EXPORT void World_destroyDamper(World* instance, int inx){
    instance->damperSystem.destroyDamper(inx); 
} 

EXPORT void World_clearDampers(World* instance){
    instance->damperSystem.clearDampers(); 
}

EXPORT void World_setDamperLayerEnabled(World* instance, int inx, int layer, bool enabled){
    instance->damperSystem.setDamperLayerEnabled(inx, layer, enabled); 
} 

EXPORT void World_zeroDamperLayerMask(World* instance, int inx){
    instance->damperSystem.zeroDamperLayerMask(inx); 
} 

EXPORT void World_setDamperPosition(World* instance, int inx, Vec3 position){
    instance->damperSystem.setDamperPosition(inx, position); 
}

EXPORT void World_setDamperStrength(World* instance, int inx, float strength){
    instance->damperSystem.setDamperStrength(inx, strength); 
}

EXPORT void World_setDamperBoundSize(World* instance, int inx, Vec3 boundSize){
    instance->damperSystem.setDamperBoundSize(inx, boundSize); 
}

EXPORT void World_setDamperBoundShape(World* instance, int inx, BoundShapeType boundShape){
    instance->damperSystem.setDamperBoundShape(inx, boundShape); 
}

EXPORT void World_setDamperBoundThickness(World* instance, int inx, float boundThickness){
    instance->damperSystem.setDamperBoundThickness(inx, boundThickness); 
} 

EXPORT void World_setDamperBoundFalloff(World* instance, int inx, BoundFalloff boundFalloff){
    instance->damperSystem.setDamperBoundFalloff(inx, boundFalloff); 
}

EXPORT void World_setDamperBoundInvRotation(World* instance, int inx, Mat3 invRotation){
    instance->damperSystem.setDamperBoundInvRotation(inx, invRotation); 
}