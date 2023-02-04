#pragma once 

#include "thirdparty/VecMath/VecMath.hpp"

class Vec3Field3{
    private:
        Vec3* data; 
        int sx; 
        int sy; 
        int sz; 
        int sysx; 

    public: 
        Vec3Field3(){
            data = NULL;
            sx = 0; 
            sy = 0; 
            sz = 0; 
            sysx = 0; 
        }

        ~Vec3Field3(){
            if(sx > 0 && sy > 0 && sz > 0)
                delete[] data;
        }

        Vec3Field3(const Vec3Field3& other) : sx(other.sx), sy(other.sy), sz(other.sz) {
            
            sx = other.sx; 
            sy = other.sy; 
            sz = other.sz; 
            sysx = sy * sx; 

            if(sx > 0 && sy > 0 && sz > 0)
                data = new Vec3[sx * sy * sz]; 

            for(int i = 0; i < sz * sysx; i++)
                data[i] = other.data[i]; 
        }
    
        Vec3Field3& operator=(const Vec3Field3& other){
            if(sx > 0 && sy > 0 && sz > 0)
                delete[] data;

            sx = other.sx; 
            sy = other.sy; 
            sz = other.sz; 
            sysx = sy * sx; 

            if(sx > 0 && sy > 0 && sz > 0)
                data = new Vec3[sx * sy * sz]; 

            for(int i = 0; i < sz * sysx; i++)
                data[i] = other.data[i];

            return *this;  
        }

        inline void allocate(int x, int y, int z){

            if(sx > 0 && sy > 0 && sz > 0)
                delete[] data;

            sx = x; 
            sy = y; 
            sz = z; 
            sysx = sy * sx; 

            if(sx > 0 && sy > 0 && sz > 0)
                data = new Vec3[sx * sy * sz]; 
        }

        inline int getSizeX(){
            return sx; 
        }

        inline int getSizeY(){
            return sy; 
        }

        inline int getSizeZ(){
            return sz; 
        }

        inline int getInx(int x, int y, int z){
            return z * sysx + y * sx + x; 
        }

        inline Vec3 getValue(int x, int y, int z){
            return data[ getInx(x, y, z) ];
        }

        inline void setValue(int x, int y, int z, Vec3 v){
            data[ getInx(x, y, z) ] = v; 
        }
};