using System.Collections;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;

namespace Dotty{

    class World{

        [StructLayout(LayoutKind.Sequential)]
        public struct Vec3{
            public float x, y, z;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Particle{
            public Vec3 position;
            public Vec3 positionNext; 
            public Vec3 velocity; 
            public Vec3 rodDelta; 
            public int rodCount; 
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
        private static extern int World_addAttrator(IntPtr instance, Vec3 position, float strength);

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
        private static extern int World_addRod(IntPtr instance, int a, int b, float length, float strength);

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
        private static extern int World_setRodStrength(IntPtr instance, int inx, float strength);

        #if UNITY_IPHONE
        [DllImport ("__Internal")]
        #else
        [DllImport ("Dotty")]   
        #endif
        private static extern int World_setRodLength(IntPtr instance, int inx, float length);


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

        unsafe public NativeArray<Particle> getParticlesData(){
            IntPtr ptr = World_getParticlesPtr(ntv); 
            int count = World_getParticlesPoolBound(ntv); 
            NativeArray<Particle> narr = NativeArrayUnsafeUtility.ConvertExistingDataToNativeArray<Particle>(ptr.ToPointer(), count, Allocator.None);
            #if ENABLE_UNITY_COLLECTIONS_CHECKS
            //NativeArrayUnsafeUtility.SetAtomicSafetyHandle(ref narr, AtomicSafetyHandle.GetTempMemoryHandle());
            NativeArrayUnsafeUtility.SetAtomicSafetyHandle(ref narr, AtomicSafetyHandle.Create());
            #endif
            return narr; 
        }

        unsafe public NativeArray<bool> getParticlesInUseData(){
            IntPtr ptr = World_getParticlesInUsePtr(ntv); 
            int count = World_getParticlesPoolBound(ntv); 
            NativeArray<bool> arr = NativeArrayUnsafeUtility.ConvertExistingDataToNativeArray<bool>(ptr.ToPointer(), count, Allocator.None);
            return arr; 
        }


        public int AddAttractor(Vector3 position, float strength){
            Vec3 pos = new Vec3();
            pos.x = position.x; 
            pos.y = position.y; 
            pos.z = position.z;
            
            return World_addAttrator(ntv, pos, strength); 
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



        public int AddRod(int a, int b, float length, float strength){
            return World_addRod(ntv, a, b, length, strength); 
        }

        public void DestroyRod(int inx){
            World_destroyRod(ntv, inx); 
        }

        public void SetRodLength(int inx, float length){
            World_setRodLength(ntv, inx, length); 
        }

        public void SetRodStrength(int inx, float strength){
            World_setRodStrength(ntv, inx, strength); 
        }
    }
}