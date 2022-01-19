#include <iostream>
#include <mutex>
#include <cmath>
#include "Vars.h"


void calcLoop(); 
void calculate(Body&, Body&); 
void sleep(int); 

static std::mutex mtx;


void calcLoop() {
	while (true) {
        sleep(10);
        //mtx.lock();
        if (bodies.size() > 1) {
            for (int i = 0; i < bodies.size(); i++) {
                bodies[i].acc.clear();
                if (bodies[i].isMovable && !bodies[i].isBeingCreated) {
                    for (int i2 = 0; i2 < bodies.size(); i2++) {
                        if (i != i2 && !bodies[i2].isBeingCreated) {
                            mtx.lock();
                            calculate(bodies[i], bodies[i2]);
                            mtx.unlock();
                        }
                    }
                }
            }
        }
        mtx.lock(); 
        for (int i = 0; i < bodies.size(); i++) {
            if (!bodies[i].isBeingCreated) {
                bodies[i].pos[0] += bodies[i].vel[0]; 
                bodies[i].pos[1] += bodies[i].vel[1]; 
            }
        }
        mtx.unlock(); 
    }

}


static void calculate(Body& b1, Body& b2) {

    double a = b1.pos[0] - b2.pos[0];
    double b = b1.pos[1] - b2.pos[1];

    double c = sqrt(a * a + b * b);
    if (c > 0.1) {
        double gForce = (200) * (((double)b1.mass * (double)b2.mass) / (c * c));//6.67408e-11 //0.00000001

        double acc[2];
        acc[0] = -(gForce * (a / (abs(a) + abs(b)))) / (double)b1.mass;
        acc[1] = -(gForce * (b / (abs(a) + abs(b)))) / (double)b1.mass;

        b1.vel[0] += acc[0]; //-(gForce * ((double)a/((double)Math.abs(a)+(double)Math.abs(b))))/(double)b1.mass;
        b1.vel[1] += acc[1]; //-(gForce * ((double)b/((double)Math.abs(a)+(double)Math.abs(b))))/(double)b1.mass;

        b1.acc.push_back({acc[0], acc[1]});
    }
}


static void sleep(int mills) {
    std::this_thread::sleep_for(std::chrono::milliseconds(mills));

}