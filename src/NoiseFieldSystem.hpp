#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"
#include "NoiseGenerator.hpp"
#include "IntersectionTesting.hpp"
#include "Vec3Field3.hpp"

class NoiseFieldSystem {
    private:
        DynamicPool<NoiseField> noiseFields;   
        DynamicPool<NoiseGenerator> noiseGenerators;   
        DynamicPool<Vec3Field3> bakes; 
        DynamicPool<bool> bakeDirty; 

    public: 
        NoiseFieldSystem(){
            noiseFields.setPoolSize(50); 
            noiseGenerators.setPoolSize(50); 
            bakes.setPoolSize(50); 
            bakeDirty.setPoolSize(50); 
        }

        void bakeNoise(BS::thread_pool& threadPool, int inx){
            bool isCurlType = noiseFields[inx].noiseType == NoiseType::SimplexCurl || 
                              noiseFields[inx].noiseType == NoiseType::PerlinCurl || 
                              noiseFields[inx].noiseType == NoiseType::ValueCurl; 

            NoiseField& noiseField = noiseFields[inx];
            NoiseGenerator& noiseGen = noiseGenerators[inx]; 
            Vec3Field3& bake = bakes[inx];

            int resolution = noiseField.bakeResolution; 
            
            bake.allocate(resolution, resolution, resolution); 

            threadPool.parallelize_loop(resolution, [this, resolution, &bake, &noiseGen, isCurlType](const int a, const int b){
                for(int k = a; k < b; k++){
                    for(int j = 0; j < resolution; j++){
                        for(int i = 0; i < resolution; i++){
                            Vec3 coord(i, j, k); 
                            Vec3 n0 = isCurlType ? noiseGen.curl(coord) : noiseGen.get3(coord);
                            bake.setValue(i, j, k, n0);
                        }
                    }
                }
            }).wait(); 
        
        }

        void updateNoiseFieldsForces(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles){
            unsigned int noiseFieldsCount = noiseFields.getBound(); 
            unsigned int pcount = particles.getBound();

            for(int i = 0; i < noiseFieldsCount; i++){
                if(bakeDirty[i] && noiseFields[i].useBake){
                    bakeNoise(threadPool, i); 
                    bakeDirty[i] = false; 
                }
            }

            threadPool.parallelize_loop(pcount, [this, noiseFieldsCount, &particles, &params](const int a, const int b){
                
                float timestep = params.timestep; 

                for(int i = 0; i < noiseFieldsCount; i++){

                    if(!noiseFields.isInUse(i))
                        continue; 

                    
                    
                    bool isCurlType = noiseFields[i].noiseType == NoiseType::SimplexCurl || noiseFields[i].noiseType == NoiseType::PerlinCurl || noiseFields[i].noiseType == NoiseType::ValueCurl; 

                    for(int p = a; p < b; p++){

                        float boundStrength; 
                        if(noiseFields[i].boundShape == BoundShapeType::Sphere){
                            boundStrength = getSphereBoundStrength( 
                                particles[p].positionNext, 
                                noiseFields[i].position, 
                                noiseFields[i].boundSize, 
                                noiseFields[i].boundShape, 
                                noiseFields[i].boundThickness, 
                                noiseFields[i].boundFalloff);
                        }
                        else if(noiseFields[i].boundShape == BoundShapeType::Box){
                            boundStrength = getBoxBoundStrength( 
                                particles[p].positionNext, 
                                noiseFields[i].position, 
                                noiseFields[i].boundSize, 
                                noiseFields[i].boundShape, 
                                noiseFields[i].boundThickness, 
                                noiseFields[i].boundFalloff, 
                                noiseFields[i].boundInvRotation);
                        }
                        else{
                            boundStrength = 1;
                        }

                        Vec3 coord = particles[p].position; 
                        Vec3 n; 

                        
                        if(noiseFields[i].useBake){
                            
                            coord.mults(noiseFields[i].noiseScale);

                            int res = noiseFields[i].bakeResolution; 
                            int X = (int)coord.x; 
                            int Y = (int)coord.y; 
                            int Z = (int)coord.z; 
                            /*
                            if(X < 0) 
                                X = 0; 
                            if(X >= res) 
                                X = res-1;    

                            if(Y < 0) 
                                Y = 0; 
                            if(Y >= res) 
                                Y = res-1; 
                            
                            if(Z < 0) 
                                Z = 0; 
                            if(Z >= res) 
                                Z = res-1; 
                            */
                            
                            X = abs(X) % noiseFields[i].bakeResolution; 
                            Y = abs(Y) % noiseFields[i].bakeResolution; 
                            Z = abs(Z) % noiseFields[i].bakeResolution;
                            
                            n = bakes[i].getValue(X, Y, Z);
                        }
                        else{
                            coord.mults(noiseFields[i].noiseScale); 
                            n = isCurlType ? noiseGenerators[i].curl(coord) : noiseGenerators[i].get3(coord);
                        }



                        if(noiseFields[i].mode == FieldMode::Force){
                            float fX = n.x * noiseFields[i].strength; 
                            float fY = n.y * noiseFields[i].strength; 
                            float fZ = n.z * noiseFields[i].strength; 
                            
                            particles[p].velocity.x += timestep * particles[p].invMass * fX * boundStrength;
                            particles[p].velocity.y += timestep * particles[p].invMass * fY * boundStrength;
                            particles[p].velocity.z += timestep * particles[p].invMass * fZ * boundStrength;
                        }
                        else if (noiseFields[i].mode == FieldMode::CorrectionForce){
                            float targetVelocityX = n.x * noiseFields[i].targetSpeed;
                            float targetVelocityY = n.y * noiseFields[i].targetSpeed; 
                            float targetVelocityZ = n.z * noiseFields[i].targetSpeed; 
                            float verrX = targetVelocityX - particles[p].velocity.x; 
                            float verrY = targetVelocityY - particles[p].velocity.y; 
                            float verrZ = targetVelocityZ - particles[p].velocity.z;
                            float kp =  noiseFields[i].strength;
                            float correctingForceX = verrX * kp; 
                            float correctingForceY = verrY * kp;
                            float correctingForceZ = verrZ * kp;

                            particles[p].velocity.x += timestep * particles[p].invMass * correctingForceX * boundStrength;
                            particles[p].velocity.y += timestep * particles[p].invMass * correctingForceY * boundStrength;
                            particles[p].velocity.z += timestep * particles[p].invMass * correctingForceZ * boundStrength;
                        }
                    }   
                    
                }

            }).wait();
        }

        int addNoiseField(Vec3 position, NoiseType noiseType, float strength, float targetSpeed, float noiseScale, FieldMode mode, 
                          Vec3 boundSize, BoundShapeType boundShape, float boundThickness, BoundFalloff boundFalloff, Mat3 boundInvRotation, int bakeResolution, bool useBake){
            NoiseField n; 

            n.position = position; 
            n.boundShape = boundShape; 
            n.boundSize = boundSize; 
            n.boundFalloff = boundFalloff; 
            n.boundThickness = boundThickness; 
            n.boundInvRotation = boundInvRotation; 

            n.noiseType = noiseType; 
            n.strength = strength;
            n.targetSpeed = targetSpeed;  
            
            n.noiseScale = noiseScale;
            n.mode = mode; 

            n.bakeResolution = bakeResolution; 
            n.useBake = useBake; 

            noiseFields.add(n);

            Vec3Field3 bakeField; 
            bakes.add(bakeField); 

            bool u = true; 
            bakeDirty.add(u);

            NoiseGenerator gen; 
            gen.setType(noiseType); 

            return noiseGenerators.add(gen);   
        }

        NoiseField* getNoiseFieldPtr(int inx){
            return &(noiseFields[inx]); 
        }

        void destroyNoiseField(int inx){
            noiseFields.remove(inx); 
            noiseGenerators.remove(inx);
        }

        void clearNoiseFields(){
            noiseFields.clear(); 
            noiseGenerators.clear(); 
        }

        void setNoiseFieldPosition(int inx, Vec3 position){
            noiseFields[inx].position = position; 
        }

        void setNoiseFieldNoiseType(int inx, NoiseType noiseType){
            noiseFields[inx].noiseType = noiseType; 
            NoiseGenerator& gen = noiseGenerators[inx]; 
            gen.setType(noiseType); 
        }

        void setNoiseFieldStrength(int inx, float strength){
            noiseFields[inx].strength = strength; 
        }

        void setNoiseFieldTargetSpeed(int inx, float targetSpeed){
            noiseFields[inx].targetSpeed = targetSpeed; 
        }

        void setNoiseFieldNoiseScale(int inx, float noiseScale){
            noiseFields[inx].noiseScale = noiseScale; 
        }

        void setNoiseFieldMode(int inx, FieldMode mode){
            noiseFields[inx].mode = mode; 
        }

        void setNoiseFieldBoundShape(int inx, BoundShapeType boundShape){
            noiseFields[inx].boundShape = boundShape; 
        }

        void setNoiseFieldBoundSize(int inx, Vec3 boundSize){
            noiseFields[inx].boundSize = boundSize; 
        }

        void setNoiseFieldBoundFalloff(int inx, BoundFalloff falloff){
            noiseFields[inx].boundFalloff = falloff; 
        }

        void setNoiseFieldBoundThickness(int inx, float thickness){
            noiseFields[inx].boundThickness = thickness; 
        }

        void setNoiseFieldBoundInvRotation(int inx, Mat3 invRotation){
            noiseFields[inx].boundInvRotation = invRotation; 
        }

        void setNoiseFieldBakeResolution(int inx, int resolution){
            noiseFields[inx].bakeResolution = resolution; 
        }

        void setNoiseFieldUseBake(int inx, bool useBake){
            noiseFields[inx].useBake = useBake; 
        }
};