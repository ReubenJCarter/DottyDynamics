#pragma once

#define DLLPROJECT_EXPORTS 


#ifdef UNIXLIKE

#define EXPORT 

#else

#ifdef DLLPROJECT_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#endif

#include "World.hpp"

extern "C" {
    EXPORT char* nativeTest();
    
    EXPORT World* createWorld();
    EXPORT void freeWorld(World* instance);
    
    EXPORT void World_update(World* instance);

    EXPORT WorldParams* World_getWorldParamsPtr(World* instance);

    EXPORT void World_setGravity(World* instance, float gravity); 
    EXPORT void World_setTimestep(World* instance, float timestep); 
    EXPORT void World_setSubsteps(World* instance, float substeps); 
    EXPORT void World_setGlobalDamping(World* instance, float globalDamping); 
    EXPORT void World_setHasCollisionFloor(World* instance, bool hasCollisionBound); 
    EXPORT void World_setCollisionFloor(World* instance, float height, float staticFriction, float kineticFriction); 

    EXPORT int World_addParticle(World* instance, Vec3 initialPosition, Vec3 initialVelocity, float invMass); 
    EXPORT void World_destroyParticle(World* instance, int inx); 
    EXPORT Particle* World_getParticlesPtr(World* instance);
    EXPORT bool* World_getParticlesInUsePtr(World* instance);
    EXPORT int World_getParticlesPoolBound(World* instance);

    EXPORT int World_addGlobalForce(World* instance, Vec3 position, Vec3 direction, float strength, 
                                    Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation);
    EXPORT GlobalForce* World_getGlobalForcePtr(World* instance, int inx); 
    EXPORT void World_destroyGlobalForce(World* instance, int inx); 
    EXPORT void World_clearGlobalForces(World* instance);
    EXPORT void World_setGlobalForcePosition(World* instance, int inx, Vec3 position); 
    EXPORT void World_setGlobalForceDirection(World* instance, int inx, Vec3 direction); 
    EXPORT void World_setGlobalForceStrength(World* instance, int inx, float strength); 
    EXPORT void World_setGlobalForceBoundSize(World* instance, int inx, Vec3 boundSize); 
    EXPORT void World_setGlobalForceBoundShape(World* instance, int inx, BoundShapeType boundShape); 
    EXPORT void World_setGlobalForceBoundThickness(World* instance, int inx, float boundThickness); 
    EXPORT void World_setGlobalForceBoundFalloff(World* instance, int inx, BoundFalloff boundFalloff); 
    EXPORT void World_setGlobalForceBoundInvRotation(World* instance, int inx, Mat3 invRotation);

    EXPORT int World_addAttrator(World* instance, Vec3 position, float strength, float minDist, float maxDist, Falloff falloff); 
    EXPORT Attractor* World_getAttractorPtr(World* instance, int inx);
    EXPORT void World_destroyAttractor(World* instance, int inx); 
    EXPORT void World_clearAttractors(World* instance);
    EXPORT void World_setAttractorPosition(World* instance, int inx, Vec3 position);
    EXPORT void World_setAttractorStrength(World* instance, int inx, float strength);
    EXPORT void World_setAttractorMinDist(World* instance, int inx, float minDist); 
    EXPORT void World_setAttractorMaxDist(World* instance, int inx, float maxDist);
    EXPORT void World_setAttractorFalloff(World* instance, int inx, Falloff falloff);

    EXPORT int World_addStrangeAttractor(World* instance, Vec3 position, float scale, StrangeAttractorType type, float strength, float targetSpeed, FieldMode fieldMode, 
                                         float minDist, float maxDist, Falloff falloff, float a, float b, float c, float d, float e, float f); 
    EXPORT StrangeAttractor* World_getStrangeAttractorPtr(World* instance, int inx);
    EXPORT void World_destroyStrangeAttractor(World* instance, int inx); 
    EXPORT void World_clearStrangeAttractors(World* instance);
    EXPORT void World_setStrangeAttractorPosition(World* instance, int inx, Vec3 position);
    EXPORT void World_setStrangeAttractorScale(World* instance, int inx, float scale);
    EXPORT void World_setStrangeAttractorType(World* instance, int inx, StrangeAttractorType type);
    EXPORT void World_setStrangeAttractorStrength(World* instance, int inx, float strength);
    EXPORT void World_setStrangeAttractorTargetSpeed(World* instance, int inx, float targetSpeed); 
    EXPORT void World_setStrangeAttractorFieldMode(World* instance, int inx, FieldMode fieldMode);
    EXPORT void World_setStrangeAttractorMinDist(World* instance, int inx, float minDist); 
    EXPORT void World_setStrangeAttractorMaxDist(World* instance, int inx, float maxDist);
    EXPORT void World_setStrangeAttractorFalloff(World* instance, int inx, Falloff falloff);
    EXPORT void World_setStrangeAttractorCoeffs(World* instance, int inx, float a, float b, float c, float d, float e, float f);

    EXPORT int World_addVortex(World* instance, Vec3 position, Vec3 normal, float strength, float minDist, float maxDist, Falloff falloff); 
    EXPORT Vortex* World_getVortexPtr(World* instance, int inx);
    EXPORT void World_destroyVortex(World* instance, int inx); 
    EXPORT void World_clearVortices(World* instance);
    EXPORT void World_setVortexPosition(World* instance, int inx, Vec3 position);
    EXPORT void World_setVortexNormal(World* instance, int inx, Vec3 normal);
    EXPORT void World_setVortexStrength(World* instance, int inx, float strength);
    EXPORT void World_setVortexMinDist(World* instance, int inx, float minDist); 
    EXPORT void World_setVortexMaxDist(World* instance, int inx, float maxDist);
    EXPORT void World_setVortexFalloff(World* instance, int inx, Falloff falloff);

    EXPORT int World_addRod(World* instance, int a, int b, float length, float stiffness);
    EXPORT void World_destroyRod(World* instance, int inx);
    EXPORT void World_setRodStiffness(World* instance, int inx, float stiffness);
    EXPORT void World_setRodLength(World* instance, int inx, float length);

    EXPORT int World_addAnchorRod(World* instance, int a, Vec3 position, float length, float stiffness);
    EXPORT void World_destroyAnchorRod(World* instance, int inx);
    EXPORT void World_setAnchorRodStiffness(World* instance, int inx, float stiffness);
    EXPORT void World_setAnchorRodLength(World* instance, int inx, float length);
    EXPORT void World_setAnchorRodPosition(World* instance, int inx, Vec3 position);

    EXPORT int World_addNoiseField(World* instance, Vec3 position, NoiseType noiseType, float strength, float targetSpeed, float noiseScale, FieldMode mode, 
                                   Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation, int bakeResolution, bool useBake);
    EXPORT NoiseField* World_getNoiseFieldPtr(World* instance, int inx); 
    EXPORT void World_destroyNoiseField(World* instance, int inx); 
    EXPORT void World_clearNoiseFields(World* instance);
    EXPORT void World_setNoiseFieldPosition(World* instance, int inx, Vec3 position); 
    EXPORT void World_setNoiseFieldNoiseType(World* instance, int inx, NoiseType noiseType); 
    EXPORT void World_setNoiseFieldStrength(World* instance, int inx, float strength); 
    EXPORT void World_setNoiseFieldTargetSpeed(World* instance, int inx, float targetSpeed); 
    EXPORT void World_setNoiseFieldNoiseScale(World* instance, int inx, float noiseScale); 
    EXPORT void World_setNoiseFieldMode(World* instance, int inx, FieldMode mode); 
    EXPORT void World_setNoiseFieldBoundShape(World* instance, int inx, BoundShapeType boundShape); 
    EXPORT void World_setNoiseFieldBoundSize(World* instance, int inx, Vec3 boundSize);
    EXPORT void World_setNoiseFieldBoundFalloff(World* instance, int inx, BoundFalloff boundFalloff);  
    EXPORT void World_setNoiseFieldBoundThickness(World* instance, int inx, float thickness); 
    EXPORT void World_setNoiseFieldBoundInvRotation(World* instance, int inx, Mat3 invRotation);
    EXPORT void World_setNoiseFieldBakeResolution(World* instance, int inx, int resolution);
    EXPORT void World_setNoiseFieldUseBake(World* instance, int inx, bool useBake);

    EXPORT int World_addSphereCollider(World* instance, Vec3 position, float radius, float kineticFriction, float staticFriction, bool inverse);
    EXPORT SphereCollider* World_getSphereColliderPtr(World* instance, int inx); 
    EXPORT void World_destroySphereCollider(World* instance, int inx);
    EXPORT void World_clearSphereColliders(World* instance);
    EXPORT void World_setSphereColliderPosition(World* instance, int inx, Vec3 position);
    EXPORT void World_setSphereColliderRadius(World* instance, int inx, float radius);
    EXPORT void World_setSphereColliderKineticFriction(World* instance, int inx, float kineticFriction); 
    EXPORT void World_setSphereColliderStaticFriction(World* instance, int inx, float staticFriction); 
    EXPORT void World_setSphereColliderInverse(World* instance, int inx, bool inverse); 

    EXPORT int World_addBoxCollider(World* instance, Vec3 position, Mat3 invRotation, Vec3 size, float kineticFriction, float staticFriction, bool inverse);
    EXPORT BoxCollider* World_getBoxColliderPtr(World* instance, int inx); 
    EXPORT void World_destroyBoxCollider(World* instance, int inx);
    EXPORT void World_clearBoxColliders(World* instance);
    EXPORT void World_setBoxColliderPosition(World* instance, int inx, Vec3 position);
    EXPORT void World_setBoxColliderInvRotation(World* instance, int inx, Mat3 invRotation);
    EXPORT void World_setBoxColliderSize(World* instance, int inx, Vec3 size);
    EXPORT void World_setBoxColliderKineticFriction(World* instance, int inx, float kineticFriction);
    EXPORT void World_setBoxColliderStaticFriction(World* instance, int inx, float staticFriction);
    EXPORT void World_setBoxColliderInverse(World* instance, int inx, bool inverse);

    EXPORT int World_addDamper(World* instance, Vec3 position, float strength, 
                                    Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation);
    EXPORT Damper* World_getDamperPtr(World* instance, int inx); 
    EXPORT void World_destroyDamper(World* instance, int inx); 
    EXPORT void World_clearDampers(World* instance);
    EXPORT void World_setDamperPosition(World* instance, int inx, Vec3 position); 
    EXPORT void World_setDamperStrength(World* instance, int inx, float strength); 
    EXPORT void World_setDamperBoundSize(World* instance, int inx, Vec3 boundSize); 
    EXPORT void World_setDamperBoundShape(World* instance, int inx, BoundShapeType boundShape); 
    EXPORT void World_setDamperBoundThickness(World* instance, int inx, float boundThickness); 
    EXPORT void World_setDamperBoundFalloff(World* instance, int inx, BoundFalloff boundFalloff); 
    EXPORT void World_setDamperBoundInvRotation(World* instance, int inx, Mat3 invRotation);
}