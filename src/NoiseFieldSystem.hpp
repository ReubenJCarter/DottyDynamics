#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"
#include "NoiseGenerator.hpp"

class NoiseFieldSystem {
    private:
        DynamicPool<NoiseField> noiseFields;   
        DynamicPool<NoiseGenerator> noiseGenerators;   

    public: 
        NoiseFieldSystem(){
            noiseFields.setPoolSize(50); 
            noiseGenerators.setPoolSize(50); 
        }

        void updateNoiseFieldsForces(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles){
            unsigned int noiseFieldsCount = noiseFields.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, noiseFieldsCount, &particles, &params](const int a, const int b){
                
                float timestep = params.timestep; 

                for(int i = 0; i < noiseFieldsCount; i++){

                    if(!noiseFields.isInUse(i))
                        continue; 

                    if(!noiseFields[i].mode == FieldMode::CorrectionForce){
                        if(noiseFields[i].noiseType == NoiseType::Simplex || noiseFields[i].noiseType == NoiseType::Perlin || noiseFields[i].noiseType == NoiseType::Value){
                            for(int p = a; p < b; p++){

                                Vec3 coord = particles[p].position; 
                                coord.mults(noiseFields[i].noiseScale); 
                                
                                Vec3 n = noiseGenerators[i].get3(coord);

                                float fX = n.x * noiseFields[i].strength; 
                                float fY = n.y * noiseFields[i].strength; 
                                float fZ = n.z * noiseFields[i].strength; 
                                
                                particles[p].velocity.x += timestep * particles[p].invMass * fX;
                                particles[p].velocity.y += timestep * particles[p].invMass * fY;
                                particles[p].velocity.z += timestep * particles[p].invMass * fZ;
                            }
                        }
                        else if(noiseFields[i].noiseType == NoiseType::SimplexCurl || noiseFields[i].noiseType == NoiseType::PerlinCurl || noiseFields[i].noiseType == NoiseType::ValueCurl){
                            for(int p = a; p < b; p++){

                                Vec3 coord = particles[p].position; 
                                coord.mults(noiseFields[i].noiseScale); 
                                
                                Vec3 n = noiseGenerators[i].curl(coord); 

                                float fX = n.x * noiseFields[i].strength; 
                                float fY = n.y * noiseFields[i].strength; 
                                float fZ = n.z * noiseFields[i].strength; 
                                
                                particles[p].velocity.x += timestep * particles[p].invMass * fX;
                                particles[p].velocity.y += timestep * particles[p].invMass * fY;
                                particles[p].velocity.z += timestep * particles[p].invMass * fZ;
                            }
                        }
                    }
                    else{
                        if(noiseFields[i].noiseType == NoiseType::Simplex || noiseFields[i].noiseType == NoiseType::Perlin || noiseFields[i].noiseType == NoiseType::Value){
                            for(int p = a; p < b; p++){

                                Vec3 coord = particles[p].position; 
                                coord.mults(noiseFields[i].noiseScale); 
                                
                                Vec3 n = noiseGenerators[i].get3(coord);
                                
                                float targetVelocityX = n.x * noiseFields[i].strength; 
                                float targetVelocityY = n.y * noiseFields[i].strength; 
                                float targetVelocityZ = n.z * noiseFields[i].strength; 
                                float verrX = targetVelocityX - particles[p].velocity.x; 
                                float verrY = targetVelocityY - particles[p].velocity.y; 
                                float verrZ = targetVelocityZ - particles[p].velocity.z;
                                float kp =  1;
                                float correctingForceX = verrX * kp; 
                                float correctingForceY = verrY * kp;
                                float correctingForceZ = verrZ * kp;

                                particles[p].velocity.x += timestep * particles[p].invMass * correctingForceX;
                                particles[p].velocity.y += timestep * particles[p].invMass * correctingForceY;
                                particles[p].velocity.z += timestep * particles[p].invMass * correctingForceZ;
                            }
                        }
                        else if(noiseFields[i].noiseType == NoiseType::SimplexCurl || noiseFields[i].noiseType == NoiseType::PerlinCurl || noiseFields[i].noiseType == NoiseType::ValueCurl){
                            for(int p = a; p < b; p++){

                                Vec3 coord = particles[p].position; 
                                coord.mults(noiseFields[i].noiseScale); 
                                
                                Vec3 n = noiseGenerators[i].curl(coord); 
                                
                                float targetVelocityX = n.x * noiseFields[i].strength; 
                                float targetVelocityY = n.y * noiseFields[i].strength; 
                                float targetVelocityZ = n.z * noiseFields[i].strength; 
                                float verrX = targetVelocityX - particles[p].velocity.x; 
                                float verrY = targetVelocityY - particles[p].velocity.y; 
                                float verrZ = targetVelocityZ - particles[p].velocity.z;
                                float kp =  1;
                                float correctingForceX = verrX * kp; 
                                float correctingForceY = verrY * kp;
                                float correctingForceZ = verrZ * kp;

                                particles[p].velocity.x += timestep * particles[p].invMass * correctingForceX;
                                particles[p].velocity.y += timestep * particles[p].invMass * correctingForceY;
                                particles[p].velocity.z += timestep * particles[p].invMass * correctingForceZ;
                            }
                        }
                    }
                  
                }

            }).wait();
        }

        int addNoiseField(Vec3 position, NoiseType noiseType, float strength, float noiseScale, FieldMode mode, 
                          BoundShapeType boundShape, Vec3 boundSize, Falloff boundFalloff, float boundThickness){
            NoiseField n; 

            n.position = position; 
            n.boundShape = boundShape; 
            n.boundSize = boundSize; 
            n.boundFalloff = boundFalloff; 
            n.boundThickness = boundThickness; 

            n.noiseType = noiseType; 
            n.strength = strength; 
            n.noiseScale = noiseScale;
            n.mode = mode; 

            noiseFields.add(n);

            NoiseGenerator gen; 
            gen.setType(noiseType); 

            return noiseGenerators.add(gen);   
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
        }

        void setNoiseFieldStrength(int inx, float strength){
            noiseFields[inx].strength = strength; 
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

        void setNoiseFieldBoundFalloff(int inx, Falloff falloff){
            noiseFields[inx].boundFalloff = falloff; 
        }

        void setNoiseFieldBoundThickness(int inx, float thickness){
            noiseFields[inx].boundThickness = thickness; 
        }

        void setNoiseFieldBoundInvRotation(int inx, Mat3 invRotation){
            noiseFields[inx].boundInvRotation = invRotation; 
        }
};