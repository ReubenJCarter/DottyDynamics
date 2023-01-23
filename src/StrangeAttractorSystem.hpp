#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class StrangeAttractorSystem {
    private:
        DynamicPool<StrangeAttractor> strangeAttractors; 

    public: 
        StrangeAttractorSystem(){
            strangeAttractors.setPoolSize(1000); 
        }

        void updateStrangeAttractors(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles){

            unsigned int maxAttractorCount = strangeAttractors.getBound(); 
            unsigned int pcount = particles.getBound();

            threadPool.parallelize_loop(pcount, [this, maxAttractorCount, &particles, &params](const int a, const int b){

                float timestep = params.timestep; 

                for(int i = 0; i < maxAttractorCount; i++){

                    if(!strangeAttractors.isInUse(i))
                        continue;          
               

                
                }
            }).wait();

        }

        int addStrangeAttrator(Vec3 position, float scale, StrangeAttractorType type, float strength, float minDist, float maxDist, Falloff falloff){
            StrangeAttractor a; 
            a.position = position;
            a.scale = scale;  
            a.type = type; 
            a.strength = strength;
            a.minDist = minDist;
            a.maxDist = maxDist; 
            a.falloff = falloff;
            return strangeAttractors.add(a);  
        }

        StrangeAttractor* getStrangeAttractorPtr(int inx){
            return &(strangeAttractors[inx]); 
        }

        void destroyStrangeAttractor(int inx){
            strangeAttractors.remove(inx); 
        }

        void clearStrangeAttractors(){
            strangeAttractors.clear(); 
        }

        void setStrangeAttractorPosition(int inx, Vec3 position){
            strangeAttractors[inx].position = position; 
        }

        void setStrangeAttractorScale(int inx, float scale){
            strangeAttractors[inx].scale = scale; 
        }

        void setStrangeAttractorType(int inx, StrangeAttractorType type){
            strangeAttractors[inx].type = type; 
        }

        void setStrangeAttractorStrength(int inx, float strength){
            strangeAttractors[inx].strength = strength; 
        }

        void setStrangeAttractorMinDist(int inx, float minDist){
            strangeAttractors[inx].minDist = minDist; 
        }

        void setStrangeAttractorMaxDist(int inx, float maxDist){
            strangeAttractors[inx].maxDist = maxDist; 
        }

        void setStrangeAttractorFalloff(int inx, Falloff falloff){
            strangeAttractors[inx].falloff = falloff;
        }

        void setStrangeAttractorK(int inx, int kinx, float k){
            strangeAttractors[inx].k[kinx] = k;
        }
};