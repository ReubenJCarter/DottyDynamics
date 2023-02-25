using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using System;


namespace Dotty{

    [System.Flags]
    public enum Mask : uint {
        None=0x00, Layer0 =0x01, Layer1=0x02, Layer2=0x04, Layer3=0x08, Layer4=0x10, Layer5=0x20, Layer6=0x40, Layer7=0x80
    };

    public enum Falloff{Constant, InvDist, InvDist2, InvDistWell, InvDist2Well, LinearRange, SquaredRange, CubedRange};
    public enum NoiseType{Simplex, SimplexCurl, Perlin, PerlinCurl, Value, ValueCurl};
    public enum FieldMode{Force, CorrectionForce};
    public enum BoundShapeType{Box, Sphere, Infinite};
    public enum BoundFalloff{Linear, Squared, Cubed}; 
    public enum StrangeAttractorType{Aizawa, Arneodo, Dadras, Dequan, Lorenz, LorenzMod2, Thomas };

    [StructLayout(LayoutKind.Sequential)]
    public struct Vec3{
        public float x, y, z;
    }

    public struct Mat3{
        public float x0, y0, z0, x1, y1, z1, x2, y2, z2; 
    }

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

    [StructLayout(LayoutKind.Sequential)]
    public struct Particle{
        public Vec3 position;
        public Vec3 positionNext; 
        public Vec3 velocity; 
        public float invMass;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct GlobalForceNtv{
        public uint layerMask; 
        public Vec3 position; 
        public Vec3 direction;
        public float strength; 
        public Vec3 boundSize; 
        public BoundShapeType boundShape;
        public float boundThickness; 
        public BoundFalloff boundFalloff; 
        public Mat3 boundInvRotation; 
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct AttractorNtv{
        public uint layerMask; 
        public Vec3 position; 
        public float strength;
        public float minDist; 
        public float maxDist; 
        public Falloff falloff; 
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct VortexNtv{
        public uint layerMask; 
        public Vec3 position; 
        public Vec3 normal; 
        public float strength;
        public float minDist; 
        public float maxDist; 
        public Falloff falloff; 
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct SphereColliderNtv {
        public uint layerMask; 
        public Vec3 position; 
        public float radius;
        public float staticFriction; 
        public float kineticFriction; 
        public bool inverse;
    } 

    [StructLayout(LayoutKind.Sequential)]
    public struct BoxColliderNtv {
        public uint layerMask; 
        public Vec3 position; 
        public Mat3 invRotation; 
        public Vec3 size;
        public float staticFriction; 
        public float kineticFriction; 
        public bool inverse; 
    } 

    [StructLayout(LayoutKind.Sequential)]
    public struct NoiseFieldNtv{
        public uint layerMask; 
        public Vec3 position; 
        public float noiseScale; 
        public float strength;
        public float targetSpeed; 
        public NoiseType noiseType;  
        public FieldMode mode; 
        public Vec3 boundSize; 
        public BoundShapeType boundShape;
        public float boundThickness; 
        public BoundFalloff boundFalloff; 
        public Mat3 boundInvRotation;
        public int bakeResolution; 
        public bool useBake; 
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct StrangeAttractorNtv {
        public uint layerMask; 
        public Vec3 position; 
        public float scale; 
        public StrangeAttractorType type; 
        public float strength;
        public float targetSpeed; 
        public FieldMode fieldMode; 
        public float minDist; 
        public float maxDist;
        public Falloff falloff;  
        public float a;
        public float b;
        public float c;
        public float d;
        public float e;
        public float f;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct DamperNtv {
        public uint layerMask; 
        public Vec3 position; 
        public float strength; 
        public Vec3 boundSize; 
        public BoundShapeType boundShape;
        public float boundThickness; 
        public BoundFalloff boundFalloff; 
        public Mat3 boundInvRotation; 
    }
}
