#include <iostream>
#include <mutex>
#include <vector>
#include <cmath>
#include "Vars.h"

void futureCalcLoop(); 
void getNextPosition(int, int); 
void sleep(int);

static std::mutex mtx; 

void futureCalcLoop() {
    while (true) {
        sleep(50);
        mtx.lock();
        if (bodies.size() > 0) {
            if (bodies[bodies.size() - 1].isMovable) {
                
                futurePos.clear(); 
                getNextPosition(bodies.size() - 1, 1000);
                
            }

        }
        else {
            
            futurePos.clear(); 
            
        }
        mtx.unlock();
    }
}

static void getNextPosition(int bodyPos, int anzahl) {

    //mtx.lock(); 
    std::vector<Body> bodycopy;
    
    for (int i = 0; i < bodies.size(); i++) {
        bodycopy.push_back(Body(bodies[i].pos[0], bodies[i].pos[1], bodies[i].radius, bodies[i].mass,{bodies[i].vel[0], bodies[i].vel[1]}));
        bodycopy[i].isMovable = bodies[i].isMovable;
    } 

    //mtx.unlock(); 


    for (int i = 0; i < anzahl; i++) {
        for (int i2 = 0; i2 < bodycopy.size(); i2++) {
            if (bodycopy[i2].isMovable) {
                for (int i3 = 0; i3 < bodycopy.size(); i3++) {
                    if (i3 != i2) {

                        double a = bodycopy[i2].pos[0] - bodycopy[i3].pos[0];
                        double b = bodycopy[i2].pos[1] - bodycopy[i3].pos[1];

                        double c = sqrt(a * a + b * b);

                        if (c > 0.1) {

                            double gForce = (200) * (((double)bodycopy[i2].mass * (double)bodycopy[i3].mass) / (c * c));//6.67408e-11

                            bodycopy[i2].vel[0] += -(gForce * ((double)a / ((double)abs(a) + (double)abs(b)))) / (double)bodycopy[i2].mass; //-(gForce * ((double)a/((double)Math.abs(a)+(double)Math.abs(b))))/(double)b1.mass;
                            bodycopy[i2].vel[1] += -(gForce * ((double)b / ((double)abs(a) + (double)abs(b)))) / (double)bodycopy[i2].mass; //-(gForce * ((double)b/((double)Math.abs(a)+(double)Math.abs(b))))/(double)b1.mass;
                        }
                    }
                }
            }
        }
        for (int i2 = 0; i2 < bodycopy.size(); i2++) {
            if (bodycopy[i2].isMovable) {
                bodycopy[i2].pos[0] += bodycopy[i2].vel[0];
                bodycopy[i2].pos[1] += bodycopy[i2].vel[1];
            }
        }

        futurePos.push_back({ (int)bodycopy[bodyPos].pos[0], (int)bodycopy[bodyPos].pos[1] });
    }
}

static void sleep(int mills) {
    std::this_thread::sleep_for(std::chrono::milliseconds(mills));

}
