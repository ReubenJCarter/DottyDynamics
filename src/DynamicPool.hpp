#pragma once 

template<class T>
class DynamicPool {
    private:
        T* data;
        bool* inUse;
        int* emptyList;
        unsigned int poolSize;
        unsigned int emptyCount;
        unsigned int bound; 
        unsigned int count; 

    public: 
        DynamicPool(){
            poolSize = 0;
            emptyCount = 0;
            bound = 0;
            count = 0; 
        }

        void setPoolSize(unsigned int sz){

            if(poolSize > 0){
                delete[] data;
                delete[] inUse;
                delete[] emptyList;
            }

            poolSize = sz;
            emptyCount = 0;

            data = new T[poolSize];
            inUse = new bool[poolSize];
            emptyList = new int[poolSize];

            for(int i = 0; i < poolSize; i++){
                inUse[i] = false; 
            }
        }

        int add(T& v){
            if(emptyCount == 0){

                if(bound == poolSize)
                    return -1; 

                int dataInx = bound; 
                data[dataInx] = v;
                inUse[dataInx] = true; 
                bound++; 
                count++; 
                
                return dataInx; 
            }
            else{
                int emptyInx = emptyCount-1; 
                int dataInx = emptyList[emptyInx]; 

                inUse[dataInx] = true; 
                data[dataInx] = v; 

                emptyCount--; 
                count++; 

                return dataInx; 
            }
            
        }

        void remove(unsigned int inx){
            if(!inUse[inx] || inx > bound){
                return; 
            }

            inUse[inx] = false; 

            if(inx == bound-1){
                bound--;
            }
            else{
                emptyList[emptyCount] = inx; 
                emptyCount++;
            }

            count--; 
        }

        T& get(unsigned int inx){
            return data[inx];
        }

        T& operator[](unsigned int inx){
            return data[inx];
        }

        bool isInUse(unsigned int inx){
            return inUse[inx]; 
        }

        unsigned int getBound(){
            return bound; 
        }

        T* getDataPtr(){
            return data;  
        }

        bool* getInUsePtr(){
            return inUse;  
        }
}; 