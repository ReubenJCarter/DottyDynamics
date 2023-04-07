#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class RodSystem {
    private:
        DynamicPool<Rod> rods;
        DynamicPool<AnchorRod> anchorRods;

    public: 
        RodSystem(){
            rods.setPoolSize(200000);
            anchorRods.setPoolSize(200000);
        }

        void updateRods(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles, DynamicPool<Vec3>& particleDeltas, DynamicPool<int>& particleDeltaCount){
            
            unsigned int maxRodCount = rods.getBound(); 

            for(int i = 0; i < maxRodCount; i++){
                if(!rods.isInUse(i)){
                    continue; 
                }
                
                float invMassA = particles[rods[i].a].invMass;
                float invMassB = particles[rods[i].b].invMass;
                float dx = (particles[rods[i].a].positionNext.x - particles[rods[i].b].positionNext.x);
                float dy = (particles[rods[i].a].positionNext.y - particles[rods[i].b].positionNext.y);
                float dz = (particles[rods[i].a].positionNext.z - particles[rods[i].b].positionNext.z);

                float diffLen2 = dx * dx + dy * dy + dz * dz;
                float diffLen = sqrt(diffLen2);
                float rodDiffLen = diffLen - rods[i].length;

                float demon = diffLen * (invMassA + invMassB); 
                if(demon == 0)
                    continue;

                float factor = (rods[i].stiffness * rodDiffLen) / demon;

                dx *= factor;
                dy *= factor;
                dz *= factor;
                /*
                particleDeltas[rods[i].a].x -= invMassA * dx;
                particleDeltas[rods[i].a].y -= invMassA * dy;
                particleDeltas[rods[i].a].z -= invMassA * dz;
                
                particleDeltas[rods[i].b].x += invMassB * dx;
                particleDeltas[rods[i].b].y += invMassB * dy;
                particleDeltas[rods[i].b].z += invMassB * dz;

                particleDeltaCount[rods[i].a]++;
                particleDeltaCount[rods[i].b]++;*/

                particles[rods[i].a].positionNext.x -= invMassA * dx;
                particles[rods[i].a].positionNext.y -= invMassA * dy;
                particles[rods[i].a].positionNext.z -= invMassA * dz;
                particles[rods[i].b].positionNext.x += invMassB * dx;
                particles[rods[i].b].positionNext.y += invMassB * dy;
                particles[rods[i].b].positionNext.z += invMassB * dz;
            }


            unsigned int maxAnchorRodCount = anchorRods.getBound(); 

            for(int i = 0; i < maxAnchorRodCount; i++){
                if(!anchorRods.isInUse(i)){
                    continue; 
                }
                
                float invMassA = particles[anchorRods[i].a].invMass;
                float dx = (particles[anchorRods[i].a].positionNext.x - anchorRods[i].position.x);
                float dy = (particles[anchorRods[i].a].positionNext.y - anchorRods[i].position.y);
                float dz = (particles[anchorRods[i].a].positionNext.z - anchorRods[i].position.z);

                float diffLen2 = dx * dx + dy * dy + dz * dz;
                float diffLen = diffLen2 == 0 ? 1 : sqrt(diffLen2);
                float rodDiffLen = diffLen - anchorRods[i].length;

                float factor = (anchorRods[i].stiffness * rodDiffLen) / (diffLen * (invMassA));

                dx *= factor;
                dy *= factor;
                dz *= factor;
            
                /*
                particleDeltas[anchorRods[i].a].x -= invMassA * dx;
                particleDeltas[anchorRods[i].a].y -= invMassA * dy;
                particleDeltas[anchorRods[i].a].z -= invMassA * dz;

                particleDeltaCount[anchorRods[i].a]++;
                */
                particles[anchorRods[i].a].positionNext.x -= invMassA * dx;
                particles[anchorRods[i].a].positionNext.y -= invMassA * dy;
                particles[anchorRods[i].a].positionNext.z -= invMassA * dz;
            }


            
        }



        int addRod(int a, int b, float length, float stiffness){
            Rod r;
            r.a = a; 
            r.b = b; 
            r.length = length; 
            r.stiffness = stiffness; 
            return rods.add(r); 
        }

        void destroyRod(int inx){
            rods.remove(inx); 
        }

        void clearRods(){
            rods.clear(); 
        }

        void setRodStiffness(int inx, float stiffness){
            rods[inx].stiffness = stiffness;
        }

        void setRodLength(int inx, float length){
            rods[inx].length = length;
        }

        int addAnchorRod(int a, Vec3 position, float length, float stiffness){
            AnchorRod r;
            r.a = a; 
            r.position = position; 
            r.length = length; 
            r.stiffness = stiffness; 
            return anchorRods.add(r); 
        }

        void destroyAnchorRod(int inx){
            anchorRods.remove(inx); 
        }

        void clearAnchorRods(){
            anchorRods.clear(); 
        }

        void setAnchorRodStiffness(int inx, float stiffness){
            anchorRods[inx].stiffness = stiffness;
        }

        void setAnchorRodLength(int inx, float length){
            anchorRods[inx].length = length;
        }

        void setAnchorRodPosition(int inx, Vec3 position){
            anchorRods[inx].position = position;
        }
}; 