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



        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_addNoiseField(IntPtr instance, NoiseType noiseType, float strength, float noiseScale, bool isVelocity);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern void World_setNoiseFieldViscosity(IntPtr instance, int inx, float viscosity);

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



        public int AddNoiseField(NoiseType noiseType, float strength, float noiseScale, bool isVelocity){
            return World_addNoiseField(ntv, noiseType, strength, noiseScale, isVelocity); 
        }

        public void SetNoiseFieldViscosity(int inx, float viscosity){
            World_setNoiseFieldViscosity(ntv, inx, viscosity); 
        }

        public void DestroyNoiseField(int inx){
            World_destroyNoiseField(ntv, inx); 
        }

        public void ClearNoiseFields(){
            World_clearNoiseFields(ntv); 
        }



        
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


        public int AddBoxCollider(Vector3 position, Matrix4x4 invRotation, Vector3 size, float kineticFriction, float staticFriction, bool inverse){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;

            Mat3 invRot = new Mat3(); 
            
            Vec3 sz = new Vec3();
            sz.x = size.x; 
            sz.y = size.y; 
            sz.z = size.z;

            return World_addBoxCollider(ntv, pos, invRot, sz, kineticFriction, staticFriction, inverse); 
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
            Mat3 invRot = new Mat3();
            //invRot.x0
            
            World_setBoxColliderInvRotation(ntv, inx, invRot); 
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