using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;


namespace Dotty{
    public enum Falloff{Constant, Linear, Squared, LinearWell, SquaredWell}
    public enum NoiseType{Simplex, SimplexCurl, Perlin, PerlinCurl, Value, ValueCurl}
    public enum ShapeType{Box, Sphere, Ellipsoid}

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
}
