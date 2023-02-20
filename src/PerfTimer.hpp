#pragma once 

#include <vector>
#include <iostream>
#include <chrono>


#if UNIXLIKE

class PerfTimer {
    public:
        typedef std::chrono::high_resolution_clock Clock;

        std::chrono::_V2::system_clock::time_point startTime; 
        std::vector< double > times; 


        void lap(){
            auto t = Clock::now();
            int64_t tus = std::chrono::duration_cast<std::chrono::microseconds>(t - startTime).count(); 
            double tms = (double)tus / 1000.0; 
            
            std::cout << "t" << times.size() << ":" << tms << "ms\n";
            
            times.push_back(tms); 
            startTime = Clock::now();
        }

        void reset(){
            startTime = Clock::now();
        }

        void clear(){
            times.clear(); 
            startTime = Clock::now();
        }
}; 

#else

class PerfTimer {
    public:
        std::chrono::high_resolution_clock::time_point startTime;
        std::vector< double > times; 
        typedef std::chrono::high_resolution_clock Clock;

        void lap(){
            auto t = Clock::now();
            int64_t tus = std::chrono::duration_cast<std::chrono::milliseconds>(t - startTime).count(); 
            double tms = (double)tus; 
            
            std::cout << "t" << times.size() << ":" << tms << "ms\n";
            
            times.push_back(tms); 
            startTime = Clock::now();
        }

        void reset(){
            startTime = Clock::now();
        }

        void clear(){
            times.clear(); 
            startTime = Clock::now();
        }
};

#endif