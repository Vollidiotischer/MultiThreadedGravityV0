#include <iostream> 
#include <thread> 
#include <mutex>
#include <vector> 
#include "Vars.h"

void drawLoop(); 
void calcLoop(); 
void futureCalcLoop(); 
void failCheck(); 


std::vector<Body> bodies;
std::vector<Arr> futurePos; 

int main() {


    
    std::thread t1(drawLoop); 

    std::thread t2(calcLoop); 

    std::thread t3(futureCalcLoop); 

    //std::thread t4(failCheck); 

    t1.join(); 
    t2.join();
    t3.join(); 
    //t4.join(); 
    

    return 0;
}
