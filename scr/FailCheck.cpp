#include <iostream>
#include <mutex>
#include "Vars.h"

static std::mutex mtx; 

void failCheck() {
	while (true) {
		std::cout << bodies.size() << std::endl; 
		mtx.lock();
		for (int i = 0; i < bodies.size(); i++) {
			if (bodies[i].pos[0] > (int)(width + 1000) || bodies[i].pos[0] < -1000 || bodies[i].pos[1] > (int)(height + 1000) || bodies[i].pos[1] < -1000) {
				
				bodies.erase(bodies.begin() + i);
				i--; 
				
			}
		}
		mtx.unlock();


		mtx.lock(); 

		for (int i = 0; i < futurePos.size(); i++) {
			if (futurePos[i].x > (int)(width + 1000) || futurePos[i].x < -1000 || futurePos[i].y > (int)(height + 1000) || futurePos[i].y < -1000) {

				futurePos.erase(futurePos.begin() + i);
				i--;

			}
		}

		mtx.unlock(); 

	}
}