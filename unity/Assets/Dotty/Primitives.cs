using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;


namespace Dotty{

    [StructLayout(LayoutKind.Sequential)]
    public struct WorldParamsNtv{
        public float timestep; 
        public int substeps; 
        public float globalDamping; 
        public float gravity;
        public float collisionFloorStaticFriction; 
        public float collisionFloorKineticFriction; 
        public float collisionFloorHeight; 
        public bool hasCollisionFloor;
    }

    public enum Falloff{Constant, InvDist, InvDist2, InvDistWell, InvDist2Well}
    public enum NoiseType{Simplex, SimplexCurl, Perlin, PerlinCurl, Value, ValueCurl}
    public enum ShapeType{Box, Sphere, Ellipsoid}

    [StructLayout(LayoutKind.Sequential)]
    public struct Vec3{
        public float x, y, z;
    }

    public struct Mat3{
        public float x0, y0, z0, x1, y1, z1, x2, y2, z2; 
    }
    
    [StructLayout(LayoutKind.Sequential)]
    public struct Particle{
        public Vec3 position;
        public Vec3 positionNext; 
        public Vec3 velocity; 
        public float invMass;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct AttractorNtv{
        public Vec3 position; 
        public float strength;
        public float minDist; 
        public float maxDist; 
        public Falloff falloff; 
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct VortexNtv{
        public Vec3 position; 
        public Vec3 normal; 
        public float strength;
        public float minDist; 
        public float maxDist; 
        public Falloff falloff; 
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct SphereColliderNtv {
        public Vec3 position; 
        public float radius;
        public float staticFriction; 
        public float kineticFriction; 
        public bool inverse;
    } 

    [StructLayout(LayoutKind.Sequential)]
    public struct BoxColliderNtv {
        public Vec3 position; 
        public Mat3 invRotation; 
        public Vec3 size;
        public float staticFriction; 
        public float kineticFriction; 
        public bool inverse; 
    } 
}