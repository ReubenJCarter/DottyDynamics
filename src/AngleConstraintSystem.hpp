#pragma once 

#include "thirdparty/thread-pool-3.3.0/BS_thread_pool.hpp"
#include "thirdparty/VecMath/VecMath.hpp"
#include "Primitives.hpp"
#include "DynamicPool.hpp"

#include <sstream>

class AngleConstraintSystem {
    private:
        DynamicPool<AngleConstraint> angleConstraints;

    public: 
        AngleConstraintSystem(){
            angleConstraints.setPoolSize(200000);
        }

        Vec3 calcCM(Particle& pa, Particle& pb, Particle& pc){
            float ma = 1.0f / pa.invMass; 
            float mb = 1.0f / pb.invMass; 
            float mc = 1.0f / pc.invMass; 
            float massSum = ma + mb + mc; 

            Vec3 pam = pa.positionNext; 
            pam.mults(ma); 

            Vec3 pbm = pb.positionNext; 
            pbm.mults(mb); 

            Vec3 pcm = pc.positionNext; 
            pcm.mults(mc); 

            Vec3 cm = pam; 
            cm.add(pbm); 
            cm.add(pcm); 

            cm.divs(massSum); 

            return cm; 
        }

        void updateAngleConstraints(
            BS::thread_pool& threadPool, 
            WorldParams& params, 
            DynamicPool<Particle>& particles, 
            DynamicPool<Vec3>& particleDeltas, 
            DynamicPool<int>& particleDeltaCount, 
            FuncCallBack debugCallback
        ){
            
            unsigned int maxCount = angleConstraints.getBound(); 

            for(int i = 0; i < maxCount; i++){

                if(!angleConstraints.isInUse(i))
                        continue;  

                int a = angleConstraints[i].a; 
                int b = angleConstraints[i].b; 
                int c = angleConstraints[i].c; 
                float invMassA = particles[a].invMass;
                float invMassB = particles[b].invMass;
                float invMassC = particles[c].invMass;
                float k = angleConstraints[i].stiffness; 
                
                Particle pa = particles[a]; 
                Particle pb = particles[b]; 
                Particle pc = particles[c]; 

                //compute diff vecs 
                Vec3 pab = pb.positionNext;
                pab.sub(pa.positionNext);  
                Vec3 pac = pc.positionNext;
                pac.sub(pa.positionNext);

                //compute their lens
                float pabLen = pab.len(); 
                float pacLen = pac.len();   

                //if one of the diff vecs is zero then continue 
                if(pabLen == 0 || pacLen == 0)
                    continue; 
                
                //compute normal vector tp surface of diff vecs
                Vec3 norm = pab; 
                norm.cross(pac);
                float xlen = norm.len(); 
                
                //if the cross product is zer0, the vextors are parallel, just make up vector for normal perpendicular to the current vectors
                if(xlen == 0){

                    //find the best axis (the least utalized by the vector )
                    Vec3 bestAxis; 
                    if(abs(pab.x) < abs(pab.y) && abs(pab.x) < abs(pab.z)){
                        bestAxis.x = 1; 
                        bestAxis.y = 0; 
                        bestAxis.z = 0; 
                    }
                    else{
                        bestAxis.x = 0; 
                        bestAxis.y = 0; 
                        bestAxis.z = 1;
                    }

                    //compute a vector perpendicular to the diff vecs 
                    norm = pab; 
                    norm.cross(bestAxis); 
                    norm.norm(); 
                }
                else{
                    //normalize the normal (not really needed ) get rdi?
                    norm.divs(xlen);  
                }

                //compute angle between diff vecs
                Vec3 pabNorm = pab;
                pabNorm.divs(pabLen); 
                Vec3 pacNorm = pac; 
                pacNorm.divs(pacLen); 
                float angle = acos( pabNorm.dot(pacNorm) );

                //compute the ammount needed to rotate each vec
                float angleDiff = angle - angleConstraints[i].angle; 
                float halfAngleDiff = angleDiff / 2; 

                //compute rotation matrix 
                Quat quat0; 
                quat0.setAxisAngle(norm, halfAngleDiff); 
                Mat4 rotMat0 = quat0.getRotationMatrix(); 

                Quat quat1; 
                quat1.setAxisAngle(norm, -halfAngleDiff); 
                Mat4 rotMat1 = quat1.getRotationMatrix(); 

                //apply rotation matrix to points b and c 
                Vec3 pabRot = pab; 
                pabRot.multm4(rotMat0); 
                Vec3 pacRot = pac; 
                pacRot.multm4(rotMat1); 

                //compute origional cm 
                Vec3 cmOrig = calcCM(particles[a], particles[b], particles[c]); 

                //set new positions of b and c
                pb.positionNext = pabRot; 
                pb.positionNext.add(pa.positionNext); 
                pc.positionNext = pacRot; 
                pc.positionNext.add(pa.positionNext); 
                
                //compute new cm
                Vec3 cmNew =calcCM(pa, pb, pc); 
                
                //move all points so the new cm is the same as the orig. (vector from new cm to orig)
                Vec3 cmDiff = cmOrig; 
                cmDiff.sub(cmNew); 
                
                pa.positionNext.add(cmDiff); 
                pb.positionNext.add(cmDiff); 
                pc.positionNext.add(cmDiff); 

                if (k == 1){
                    particles[a].positionNext = pa.positionNext; 
                    particles[b].positionNext = pb.positionNext;
                    particles[c].positionNext = pc.positionNext;
                }
                else{
                
                    Vec3 diffA = pa.positionNext; 
                    diffA.sub(particles[a].positionNext); 
                    Vec3 diffB = pb.positionNext; 
                    diffB.sub(particles[b].positionNext); 
                    Vec3 diffC = pc.positionNext; 
                    diffC.sub(particles[c].positionNext); 

                    //multiply diffs by the stiffness 
                    diffA.mults(k);
                    diffB.mults(k);
                    diffC.mults(k);  

                    //add diffs to next position 
                    particles[a].positionNext.add(diffA); 
                    particles[b].positionNext.add(diffB); 
                    particles[c].positionNext.add(diffC); 
                    
                    //recompute cm and adjust to make same a orig cm
                    Vec3 cmNew2 = calcCM(particles[a], particles[b], particles[c]); 
                    Vec3 cmDiff2 = cmOrig;
                    cmDiff2.sub(cmNew2); 
                    particles[a].positionNext.add(cmDiff2); 
                    particles[b].positionNext.add(cmDiff2); 
                    particles[c].positionNext.add(cmDiff2); 
                }

                /*
                std::stringstream ss;
                ss << "angleDiff" << angleDiff 
                << " norm " << norm.x << ", " << norm.y << ", " << norm.z << ", "
                << " quat0 " << quat0.x << ", " << quat0.y << ", " << quat0.z << ", " << quat0.w
                << "  a " << pa.positionNext.x << ", "  << pa.positionNext.y << ", "  << pa.positionNext.z  
                << "  b " << pb.positionNext.x << ", "  << pb.positionNext.y << ", "  << pb.positionNext.z
                << "  c " << pc.positionNext.x << ", "  << pc.positionNext.y << ", "  << pc.positionNext.z
                << "\n"; 
                log(debugCallback, ss.str()); 
                */
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