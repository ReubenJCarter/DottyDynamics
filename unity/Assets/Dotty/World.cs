using System.Collections;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

namespace Dotty{

    public class World: MonoBehaviour{


        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern IntPtr createWorld();

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void freeWorld(IntPtr world);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_update(IntPtr world);


        private IntPtr ntv; 

        public static World instance; 

        public void Awake(){
            ntv = createWorld();
            instance = this;
        }

        void OnDestroy(){
            freeWorld(ntv);
        }

        public void FixedUpdate(){
            World_update(ntv); 
        }




        /*
        *
        *WORLD PARAMS
        *
        */

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern IntPtr World_getWorldParamsPtr(IntPtr world); 

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setGravity(IntPtr world, float gravity);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setTimestep(IntPtr world, float timestep);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setSubsteps(IntPtr instance, float substeps); 

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setGlobalDamping(IntPtr world, float globalDamping);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setHasCollisionFloor(IntPtr instance, bool hasCollisionBound);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setCollisionFloor(IntPtr instance, float height, float staticFriction, float kineticFriction);

        unsafe public WorldParamsNtv* GetWorldParamsPtr(){
            IntPtr ptr = World_getWorldParamsPtr(ntv); 
            return (WorldParamsNtv*)ptr.ToPointer(); 
        }

        public void SetGravity(float gravity){
            World_setGravity(ntv, gravity); 
        }
        
        public void SetTimestep(float timestep){
            World_setTimestep(ntv, timestep); 
        }

        public void SetSubsteps(float substeps){
            World_setSubsteps(ntv, substeps); 
        }

        public void SetGlobalDamping(float globalDamping){
            World_setGlobalDamping(ntv, globalDamping); 
        }

        public void SetHasCollisionFloor(bool h){
            World_setHasCollisionFloor(ntv, h); 
        }

        public void SetCollisionFloor(float height, float staticFriction, float kineticFriction){
            World_setCollisionFloor(ntv, height, staticFriction, kineticFriction);
        }



        /*
        *
        *PARTICLES
        *
        */

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_addParticle(IntPtr world, Vec3 initialPosition, Vec3 initialVelocity, float invMass);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_destroyParticle(IntPtr instance, int inx);
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern IntPtr World_getParticlesPtr(IntPtr world);
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern IntPtr World_getParticlesInUsePtr(IntPtr world);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_getParticlesPoolBound(IntPtr world);

        public int AddParticle(Vector3 initialPosition, Vector3 initialVelocity, float invMass){

            Vec3 initialPositionInternal = new Vec3();
            initialPositionInternal.x = initialPosition.x; 
            initialPositionInternal.y = initialPosition.y; 
            initialPositionInternal.z = initialPosition.z; 

            Vec3 initialVelocityInternal = new Vec3();
            initialVelocityInternal.x = initialVelocity.x; 
            initialVelocityInternal.y = initialVelocity.y; 
            initialVelocityInternal.z = initialVelocity.z;

            return World_addParticle(ntv, initialPositionInternal, initialVelocityInternal, invMass); 
        }

        void DestroyParticle(int inx){
            World_destroyParticle(ntv, inx);
        }

        public int GetParticlesPoolBound(){
            return World_getParticlesPoolBound(ntv); 
        } 

        unsafe public NativeArray<Particle> GetParticlesData(){
            IntPtr ptr = World_getParticlesPtr(ntv); 
            int count = World_getParticlesPoolBound(ntv); 
            NativeArray<Particle> narr = NativeArrayUnsafeUtility.ConvertExistingDataToNativeArray<Particle>(ptr.ToPointer(), count, Allocator.None);
            #if ENABLE_UNITY_COLLECTIONS_CHECKS
            //NativeArrayUnsafeUtility.SetAtomicSafetyHandle(ref narr, AtomicSafetyHandle.GetTempMemoryHandle());
            NativeArrayUnsafeUtility.SetAtomicSafetyHandle(ref narr, AtomicSafetyHandle.Create());
            #endif
            return narr; 
        }

        unsafe public Particle* GetParticlesPtr(){
            IntPtr ptr = World_getParticlesPtr(ntv); 
            return (Particle*)ptr.ToPointer(); 
        }

        unsafe public NativeArray<bool> GetParticlesInUseData(){
            IntPtr ptr = World_getParticlesInUsePtr(ntv); 
            int count = World_getParticlesPoolBound(ntv); 
            NativeArray<bool> arr = NativeArrayUnsafeUtility.ConvertExistingDataToNativeArray<bool>(ptr.ToPointer(), count, Allocator.None);
            return arr; 
        }



        /*
        *
        *GLOBAL FORCE
        *
        */

         #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_addGlobalForce(IntPtr instance, Vec3 position, Vec3 direction, float strength, 
                                                       Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation);
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern IntPtr World_getGlobalForcePtr(IntPtr instance, int inx); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_destroyGlobalForce(IntPtr instance, int inx); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_clearGlobalForces(IntPtr instance);
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setGlobalForcePosition(IntPtr instance, int inx, Vec3 position); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setGlobalForceDirection(IntPtr instance, int inx, Vec3 direction); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setGlobalForceStrength(IntPtr instance, int inx, float strength); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setGlobalForceBoundSize(IntPtr instance, int inx, Vec3 boundSize); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setGlobalForceBoundShape(IntPtr instance, int inx, BoundShapeType boundShape); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setGlobalForceBoundThickness(IntPtr instance, int inx, float boundThickness); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setGlobalForceBoundFalloff(IntPtr instance, int inx, BoundFalloff boundFalloff);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setGlobalForceBoundInvRotation(IntPtr instance, int inx, Mat3 boundInvRotation);

        public int AddGlobalForce(Vector3 position, Vector3 direction, float strength, Vector3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Matrix4x4 boundInvRotation){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            
            Vec3 dir = new Vec3();
            dir.x = direction.x; 
            dir.y = direction.y; 
            dir.z = direction.z;

            Vec3 sz = new Vec3();
            sz.x = boundSize.x; 
            sz.y = boundSize.y; 
            sz.z = boundSize.z;

            Mat3 ir = new Mat3(); 
            ir.x0 = boundInvRotation[0, 0]; 
            ir.y0 = boundInvRotation[1, 0]; 
            ir.z0 = boundInvRotation[2, 0]; 
            ir.x1 = boundInvRotation[0, 1]; 
            ir.y1 = boundInvRotation[1, 1]; 
            ir.z1 = boundInvRotation[2, 1]; 
            ir.x2 = boundInvRotation[0, 2]; 
            ir.y2 = boundInvRotation[1, 2]; 
            ir.z2 = boundInvRotation[2, 2]; 
            
            return World_addGlobalForce(ntv, pos, dir, strength, sz, boundShape, boundThickness, boundFalloff, ir);
        }

        unsafe public GlobalForceNtv* GetGlobalForcePtr(int inx){
            IntPtr ptr = World_getGlobalForcePtr(ntv, inx); 
            return (GlobalForceNtv*)ptr.ToPointer(); 
        } 

        public void DestroyGlobalForce(int inx){
            World_destroyGlobalForce(ntv, inx); 
        }

        public void ClearGlobalForces(){
            World_clearGlobalForces(ntv); 
        }

        void SetGlobalForcePosition(int inx, Vector3 position){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            World_setGlobalForcePosition(ntv, inx, pos); 
        }

        void SetGlobalForceDirection(int inx, Vector3 direction){
            Vec3 dir = new Vec3();
            dir.x = direction.x; 
            dir.y = direction.y; 
            dir.z = direction.z;
            World_setGlobalForceDirection(ntv, inx, dir); 
        }

        void SetGlobalForceStrength(int inx, float strength){
            World_setGlobalForceStrength(ntv, inx, strength); 
        }

        void SetGlobalForceBoundSize(int inx, Vector3 boundSize){
            Vec3 sz = new Vec3();
            sz.x = boundSize.x; 
            sz.y = boundSize.y; 
            sz.z = boundSize.z;
            World_setGlobalForceBoundSize(ntv, inx, sz); 
        }

        void SetGlobalForceBoundShape(int inx, BoundShapeType boundShape){
            World_setGlobalForceBoundShape(ntv, inx, boundShape); 
        }

        void SetGlobalForceBoundThickness(int inx, float boundThickness){
            World_setGlobalForceBoundThickness(ntv, inx, boundThickness); 
        }

        void SetGlobalForceBoundFalloff(int inx, BoundFalloff boundFalloff){
            World_setGlobalForceBoundFalloff(ntv, inx, boundFalloff); 
        }

        void SetGlobalForceBoundInvRotation(int inx, Matrix4x4 boundInvRotation){

            Mat3 ir = new Mat3(); 
            ir.x0 = boundInvRotation[0, 0]; 
            ir.y0 = boundInvRotation[1, 0]; 
            ir.z0 = boundInvRotation[2, 0]; 
            ir.x1 = boundInvRotation[0, 1]; 
            ir.y1 = boundInvRotation[1, 1]; 
            ir.z1 = boundInvRotation[2, 1]; 
            ir.x2 = boundInvRotation[0, 2]; 
            ir.y2 = boundInvRotation[1, 2]; 
            ir.z2 = boundInvRotation[2, 2];

            World_setGlobalForceBoundInvRotation(ntv, inx, ir); 
        }



        /*
        *
        *ATTRACTOR
        *
        */

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_addAttrator(IntPtr instance, Vec3 position, float strength, float minDist, float maxDist, Falloff falloff);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern IntPtr World_getAttractorPtr(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_destroyAttractor(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_clearAttractors(IntPtr instance);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setAttractorPosition(IntPtr instance, int inx, Vec3 position);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setAttractorStrength(IntPtr instance, int inx, float strength);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setAttractorMinDist(IntPtr instance, int inx, float minDist);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setAttractorMaxDist(IntPtr instance, int inx, float maxDist);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setAttractorFalloff(IntPtr instance, int inx, Falloff falloff);

        public int AddAttractor(Vector3 position, float strength, float minDist, float maxDist, Falloff falloff){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            
            return World_addAttrator(ntv, pos, strength, minDist, maxDist, falloff); 
        }

        unsafe public AttractorNtv* GetAttractorPtr(int inx){
            IntPtr ptr = World_getAttractorPtr(ntv, inx); 
            return (AttractorNtv*)ptr.ToPointer(); 
        }

        public void DestroyAttractor(int inx){
            World_destroyAttractor(ntv, inx); 
        }

        public void ClearAttractors(){
            World_clearAttractors(ntv); 
        }

        public void SetAttractorPosition(int inx, Vector3 position){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            
            World_setAttractorPosition(ntv, inx, pos); 
        }

        public void SetAttractorStrength(int inx, float strength){
            World_setAttractorStrength(ntv, inx, strength); 
        }

        public void SetAttractorMinDist(int inx, float minDist){
            World_setAttractorMinDist(ntv, inx, minDist);
        }

        public void SetAttractorMaxDist(int inx, float maxDist){
            World_setAttractorMaxDist(ntv, inx, maxDist);
        }

        public void SetAttractorFalloff(int inx, Falloff falloff){
            World_setAttractorFalloff(ntv, inx, falloff);
        }



        /*
        *
        *STRANGE ATTRACTOR
        *
        */

        



        /*
        *
        *VORTEX
        *
        */

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]
        #endif
        private static extern int World_addVortex(IntPtr instance, Vec3 position, Vec3 normal, float strength, float minDist, float maxDist, Falloff falloff);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern IntPtr World_getVortexPtr(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_destroyVortex(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_clearVortices(IntPtr instance);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setVortexPosition(IntPtr instance, int inx, Vec3 position);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setVortexNormal(IntPtr instance, int inx, Vec3 normal);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setVortexStrength(IntPtr instance, int inx, float strength);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setVortexMinDist(IntPtr instance, int inx, float minDist);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setVortexMaxDist(IntPtr instance, int inx, float maxDist);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setVortexFalloff(IntPtr instance, int inx, Falloff falloff);

        public int AddVortex(Vector3 position, Vector3 normal, float strength, float minDist, float maxDist, Falloff falloff){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            Vec3 norm = new Vec3(); 
            norm.x = normal.x; 
            norm.y = normal.y; 
            norm.z = normal.z; 
            
            return World_addVortex(ntv, pos, norm, strength, minDist, maxDist, falloff); 
        }

        unsafe public VortexNtv* GetVortexPtr(int inx){
            IntPtr ptr = World_getVortexPtr(ntv, inx); 
            return (VortexNtv*)ptr.ToPointer(); 
        }

        public void DestroyVortex(int inx){
            World_destroyVortex(ntv, inx); 
        }

        public void ClearVortices(){
            World_clearVortices(ntv); 
        }

        public void SetVortexPosition(int inx, Vector3 position){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            
            World_setVortexPosition(ntv, inx, pos); 
        }

        public void SetVortexNormal(int inx, Vector3 normal){
            Vec3 norm = new Vec3();
            norm.x = normal.x; 
            norm.y = normal.y; 
            norm.z = normal.z;
            
            World_setVortexNormal(ntv, inx, norm); 
        }

        public void SetVortexStrength(int inx, float strength){
            World_setVortexStrength(ntv, inx, strength); 
        }

        public void SetVortexMinDist(int inx, float minDist){
            World_setVortexMinDist(ntv, inx, minDist);
        }

        public void SetVortexMaxDist(int inx, float maxDist){
            World_setVortexMaxDist(ntv, inx, maxDist);
        }

        public void SetVortexFalloff(int inx, Falloff falloff){
            World_setVortexFalloff(ntv, inx, falloff);
        }



        /*
        *
        *RODS
        *
        */

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_addRod(IntPtr instance, int a, int b, float length, float stiffness);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_destroyRod(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setRodStiffness(IntPtr instance, int inx, float stiffness);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setRodLength(IntPtr instance, int inx, float length);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_addAnchorRod(IntPtr instance, int a, Vec3 position, float length, float stiffness);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_destroyAnchorRod(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setAnchorRodStiffness(IntPtr instance, int inx, float stiffness);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setAnchorRodLength(IntPtr instance, int inx, float length);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setAnchorRodPosition(IntPtr instance, int inx, Vec3 position);

        public int AddRod(int a, int b, float length, float stiffness){
            return World_addRod(ntv, a, b, length, stiffness); 
        }

        public void DestroyRod(int inx){
            World_destroyRod(ntv, inx); 
        }

        public void SetRodLength(int inx, float length){
            World_setRodLength(ntv, inx, length); 
        }

        public void SetRodStiffness(int inx, float stiffness){
            World_setRodStiffness(ntv, inx, stiffness); 
        }

        public int AddAnchorRod(int a, Vector3 position, float length, float stiffness){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;

            return World_addAnchorRod(ntv, a, pos, length, stiffness); 
        }

        public void DestroyAnchorRod(int inx){
            World_destroyAnchorRod(ntv, inx); 
        }

        public void SetAnchorRodLength(int inx, float length){
            World_setAnchorRodLength(ntv, inx, length); 
        }

        public void SetAnchorRodStiffness(int inx, float stiffness){
            World_setAnchorRodStiffness(ntv, inx, stiffness); 
        }

        public void SetAnchorRodPosition(int inx, Vector3 position){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;

            World_setAnchorRodPosition(ntv, inx, pos); 
        }



        /*
        *
        *NOISE FIELD
        *
        */

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_addNoiseField(IntPtr instance, Vec3 position, NoiseType noiseType, float strength, float noiseScale, FieldMode mode, 
                                                      Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation);
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern IntPtr World_getNoiseFieldPtr(IntPtr instance, int inx); 

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_destroyNoiseField(IntPtr instance, int inx); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_clearNoiseFields(IntPtr instance);
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldPosition(IntPtr instance, int inx, Vec3 position); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldNoiseType(IntPtr instance, int inx, NoiseType noiseType); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldStrength(IntPtr instance, int inx, float strength); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldNoiseScale(IntPtr instance, int inx, float noiseScale); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldMode(IntPtr instance, int inx, FieldMode mode); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldBoundShape(IntPtr instance, int inx, BoundShapeType boundShape); 
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldBoundSize(IntPtr instance, int inx, Vec3 boundSize);
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldBoundFalloff(IntPtr instance, int inx, BoundFalloff boundFalloff);  
        
        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldBoundThickness(IntPtr instance, int inx, float thickness); 

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldBoundInvRotation(IntPtr instance, int inx, Mat3 invRotation); 

        public int AddNoiseField(Vector3 position, NoiseType noiseType, float strength, float noiseScale, FieldMode mode,
                                 Vector3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Matrix4x4 boundInvRotation){

            Vec3 pos = new Vec3();
            pos.x = position.x;  
            pos.y = position.y; 
            pos.z = position.z;

            Vec3 sz = new Vec3();
            sz.x = boundSize.x; 
            sz.y = boundSize.y; 
            sz.z = boundSize.z;

            Mat3 ir = new Mat3(); 
            ir.x0 = boundInvRotation[0, 0]; 
            ir.y0 = boundInvRotation[1, 0]; 
            ir.z0 = boundInvRotation[2, 0]; 
            ir.x1 = boundInvRotation[0, 1]; 
            ir.y1 = boundInvRotation[1, 1]; 
            ir.z1 = boundInvRotation[2, 1]; 
            ir.x2 = boundInvRotation[0, 2]; 
            ir.y2 = boundInvRotation[1, 2]; 
            ir.z2 = boundInvRotation[2, 2];

            return World_addNoiseField(ntv, pos, noiseType, strength, noiseScale, mode, sz, boundShape, boundThickness, boundFalloff, ir); 
        }

        unsafe public NoiseFieldNtv* GetNoiseFieldPtr(int inx){
            IntPtr ptr = World_getNoiseFieldPtr(ntv, inx); 
            return (NoiseFieldNtv*)ptr.ToPointer(); 
        } 

        public void DestroyNoiseField(int inx){
            World_destroyNoiseField(ntv, inx); 
        }

        public void ClearNoiseFields(){
            World_clearNoiseFields(ntv); 
        }

        public void SetNoiseFieldPosition(int inx, Vector3 position){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;

            World_setNoiseFieldPosition(ntv, inx, pos);
        }

        public void SetNoiseFieldNoiseType(int inx, NoiseType noiseType){
            World_setNoiseFieldNoiseType(ntv, inx, noiseType);
        }

        public void SetNoiseFieldStrength(int inx, float strength){
            World_setNoiseFieldStrength(ntv, inx, strength);
        }

        public void SetNoiseFieldNoiseScale(int inx, float noiseScale){
            World_setNoiseFieldNoiseScale(ntv, inx, noiseScale);
        }

        public void SetNoiseFieldMode(int inx, FieldMode fieldMode){
            World_setNoiseFieldMode(ntv, inx, fieldMode);
        }

        public void SetNoiseFieldBoundShape(int inx, BoundShapeType boundShape){
            World_setNoiseFieldBoundShape(ntv, inx, boundShape); 
        }

        public void SetNoiseFieldBoundSize(int inx, Vector3 boundSize){
            Vec3 sz = new Vec3();
            sz.x = boundSize.x; 
            sz.y = boundSize.y; 
            sz.z = boundSize.z;

            World_setNoiseFieldBoundSize(ntv, inx, sz); 
        }

        public void SetNoiseFieldBoundFalloff(int inx, BoundFalloff boundFalloff){
            World_setNoiseFieldBoundFalloff(ntv, inx, boundFalloff); 
        }

        public void SetNoiseFieldBoundThickness(int inx, float boundThickness){
            World_setNoiseFieldBoundThickness(ntv, inx, boundThickness); 
        }

        public void SetNoiseFieldBoundInvRotation(int inx, Matrix4x4 boundInvRotation){
            Mat3 ir = new Mat3(); 
            ir.x0 = boundInvRotation[0, 0]; 
            ir.y0 = boundInvRotation[1, 0]; 
            ir.z0 = boundInvRotation[2, 0]; 
            ir.x1 = boundInvRotation[0, 1]; 
            ir.y1 = boundInvRotation[1, 1]; 
            ir.z1 = boundInvRotation[2, 1]; 
            ir.x2 = boundInvRotation[0, 2]; 
            ir.y2 = boundInvRotation[1, 2]; 
            ir.z2 = boundInvRotation[2, 2];
            World_setNoiseFieldBoundInvRotation(ntv, inx, ir); 
        }


        /*
        *
        *SPHERE COLLIDER
        *
        */

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_addSphereCollider(IntPtr instance, Vec3 position, float radius, float kineticFriction, float staticFriction, bool inverse);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern IntPtr World_getSphereColliderPtr(IntPtr instance, int inx); 

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_destroySphereCollider(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_clearSphereColliders(IntPtr instance);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setSphereColliderPosition(IntPtr instance, int inx, Vec3 position);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setSphereColliderRadius(IntPtr instance, int inx, float radius);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setSphereColliderKineticFriction(IntPtr instance, int inx, float kineticFriction); 

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setSphereColliderStaticFriction(IntPtr instance, int inx, float staticFriction); 

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setSphereColliderInverse(IntPtr instance, int inx, bool inverse); 
        
        public int AddSphereCollider(Vector3 position, float radius, float kineticFriction, float staticFriction, bool inverse){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            
            return World_addSphereCollider(ntv, pos, radius, kineticFriction, staticFriction, inverse); 
        }

        unsafe public SphereColliderNtv* GetSphereColliderPtr(int inx){
            IntPtr ptr = World_getSphereColliderPtr(ntv, inx); 
            return (SphereColliderNtv*)ptr.ToPointer(); 
        }

        public void DestroySphereCollider(int inx){
            World_destroySphereCollider(ntv, inx); 
        }

        public void ClearSphereColliders(){
            World_clearSphereColliders(ntv); 
        }

        public void SetSphereColliderPosition(int inx, Vector3 position){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            
            World_setSphereColliderPosition(ntv, inx, pos); 
        }

        public void SetSphereColliderRadius(int inx, float radius){
            World_setSphereColliderRadius(ntv, inx, radius); 
        }

        public void SetSphereColliderKineticFriction(int inx, float kineticFriction){
            World_setSphereColliderKineticFriction(ntv, inx, kineticFriction);
        }

        public void SetSphereColliderStaticFriction(int inx, float staticFriction){
            World_setSphereColliderStaticFriction(ntv, inx, staticFriction);
        }

        public void SetSphereColliderInverse(int inx, bool inverse){
            World_setSphereColliderInverse(ntv, inx, inverse);
        }



        /*
        *
        *BOX COLLIDER
        *
        */

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_addBoxCollider(IntPtr instance, Vec3 position, Mat3 invRotation, Vec3 size, float kineticFriction, float staticFriction, bool inverse);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern IntPtr World_getBoxColliderPtr(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_destroyBoxCollider(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_clearBoxColliders(IntPtr instance);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setBoxColliderPosition(IntPtr instance, int inx, Vec3 position);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setBoxColliderInvRotation(IntPtr instance, int inx, Mat3 invRotation);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setBoxColliderSize(IntPtr instance, int inx, Vec3 size);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setBoxColliderKineticFriction(IntPtr instance, int inx, float kineticFriction);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setBoxColliderStaticFriction(IntPtr instance, int inx, float staticFriction);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setBoxColliderInverse(IntPtr instance, int inx, bool inverse);

        public int AddBoxCollider(Vector3 position, Matrix4x4 invRotation, Vector3 size, float kineticFriction, float staticFriction, bool inverse){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            
            Vec3 sz = new Vec3();
            sz.x = size.x; 
            sz.y = size.y; 
            sz.z = size.z;

            Mat3 ir = new Mat3(); 
            ir.x0 = invRotation[0, 0]; 
            ir.y0 = invRotation[1, 0]; 
            ir.z0 = invRotation[2, 0]; 
            ir.x1 = invRotation[0, 1]; 
            ir.y1 = invRotation[1, 1]; 
            ir.z1 = invRotation[2, 1]; 
            ir.x2 = invRotation[0, 2]; 
            ir.y2 = invRotation[1, 2]; 
            ir.z2 = invRotation[2, 2]; 

            return World_addBoxCollider(ntv, pos, ir, sz, kineticFriction, staticFriction, inverse); 
        }

        unsafe public BoxColliderNtv* GetBoxColliderPtr(int inx){
            IntPtr ptr = World_getBoxColliderPtr(ntv, inx); 
            return (BoxColliderNtv*)ptr.ToPointer(); 
        }

        public void DestroyBoxCollider(int inx){
            World_destroyBoxCollider(ntv, inx); 
        }

        public void ClearBoxColliders(){
            World_clearBoxColliders(ntv); 
        }

        public void SetBoxColliderPosition(int inx, Vector3 position){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            
            World_setBoxColliderPosition(ntv, inx, pos); 
        }

        public void SetBoxColliderInvRotation(int inx, Matrix4x4 invRotation){

            Mat3 ir = new Mat3(); 
            ir.x0 = invRotation[0, 0]; 
            ir.y0 = invRotation[1, 0]; 
            ir.z0 = invRotation[2, 0]; 
            ir.x1 = invRotation[0, 1]; 
            ir.y1 = invRotation[1, 1]; 
            ir.z1 = invRotation[2, 1]; 
            ir.x2 = invRotation[0, 2]; 
            ir.y2 = invRotation[1, 2]; 
            ir.z2 = invRotation[2, 2];
            
            World_setBoxColliderInvRotation(ntv, inx, ir); 
        }

        public void SetBoxColliderSize(int inx, Vector3 size){
            Vec3 sz = new Vec3();
            sz.x = size.x; 
            sz.y = size.y; 
            sz.z = size.z;

            World_setBoxColliderSize(ntv, inx, sz); 
        }

        public void SetBoxColliderKineticFriction(int inx, float kineticFriction){
            World_setBoxColliderKineticFriction(ntv, inx, kineticFriction);
        }

        public void SetBoxColliderStaticFriction(int inx, float staticFriction){
            World_setBoxColliderStaticFriction(ntv, inx, staticFriction);
        }

        public void SetBoxColliderInverse(int inx, bool inverse){
            World_setBoxColliderInverse(ntv, inx, inverse);
        }
    }
}