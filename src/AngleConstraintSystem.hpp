#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

class AngleConstraintSystem {
    private:
        DynamicPool<AngleConstraint> angleConstraints;

    public: 
        AngleConstraintSystem(){
            angleConstraints.setPoolSize(200000);
        }

        void updateAngleConstraints(BS::thread_pool& threadPool, WorldParams& params, DynamicPool<Particle>& particles, DynamicPool<Vec3>& particleDeltas, DynamicPool<int>& particleDeltaCount){
            
            unsigned int maxCount = angleConstraints.getBound(); 

            for(int i = 0; i < maxCount; i++){
                int a = angleConstraints[i].a; 
                int b = angleConstraints[i].b; 
                int c = angleConstraints[i].c; 
                float invMassA = particles[a].invMass;
                float invMassB = particles[b].invMass;
                float invMassC = particles[c].invMass;
                
                Particle pa = particles[a]; 
                Particle pb = particles[b]; 
                Particle pc = particles[c]; 

                //Vec3 papb = pb - pa 
                //Vec3 papc = pc - pa
                //
                //acos()
            }
            
        }



        int addAngleConstraint(int a, int b, int c, float angle, float stiffness){
            AngleConstraint r;
            r.a = a; 
            r.b = b; 
            r.c = c;
            r.angle = angle; 
            r.stiffness = stiffness; 
            return angleConstraints.add(r); 
        }

        void destroyAngleConstraint(int inx){
            angleConstraints.remove(inx); 
        }

        void clearAngleConstraints(){
            angleConstraints.clear(); 
        }

        void setAngleConstraintStiffness(int inx, float stiffness){
            angleConstraints[inx].stiffness = stiffness;
        }

        void setAngleConstraintAngle(int inx, float angle){
            angleConstraints[inx].angle = angle;
        }

}; 