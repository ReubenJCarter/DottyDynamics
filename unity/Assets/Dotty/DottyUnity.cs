using System.Collections;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

namespace Dotty{

    class World{

        public enum Falloff{Constant, Linear, Squared, LinearWell, SquaredWell}
        public enum NoiseType{Simplex, SimplexCurl, Perlin, PerlinCurl, Value, ValueCurl}

        public enum BoundShape{Cuboid, Sphere, Ellipsoid}


        [StructLayout(LayoutKind.Sequential)]
        public struct Vec3{
            public float x, y, z;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Particle{
            public Vec3 position;
            public Vec3 positionNext; 
            public Vec3 velocity; 
            public float invMass;
        }


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
        private static extern void World_destroyAttractor(IntPtr instance, int inx);

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
        private static extern int World_addRod(IntPtr instance, int a, int b, float length, float stiffness);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_destroyRod(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_setRodStiffness(IntPtr instance, int inx, float stiffness);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_setRodLength(IntPtr instance, int inx, float length);



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
        private static extern int World_destroyAnchorRod(IntPtr instance, int inx);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_setAnchorRodStiffness(IntPtr instance, int inx, float stiffness);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_setAnchorRodLength(IntPtr instance, int inx, float length);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_setAnchorRodPosition(IntPtr instance, int inx, Vec3 position);



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
        private static extern int World_setNoiseFieldViscosity(IntPtr instance, int inx, float viscosity);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_destroyNoiseField(IntPtr instance, int inx);

        

        private IntPtr ntv; 


        public World(){
            ntv = createWorld();
        }

        ~World(){
            freeWorld(ntv);
        }

        public void Update(){
            World_update(ntv); 
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

        public void DestroyAttractor(int inx){
            World_destroyAttractor(ntv, inx); 
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
    }
}