#pragma once
#include <vector>

struct DoubleArr {
    double xVel, yVel; 
};

class Body {
public:

    double pos[2];
    double vel[2] = { 0.0, 0.0 };
    int mass;
    int radius;
    bool isMovable = true;
    bool isBeingCreated = false; 
    std::vector<DoubleArr> acc; 

    Body(const int& x, const int& y, const int radius, const int mass) {
        this->pos[0] = x;
        this->pos[1] = y;
        this->radius = radius;
        this->mass = mass;
    }

    Body(const double& x, const double& y, const int radius, const int mass, const bool movable) {
        this->pos[0] = x;
        this->pos[1] = y;
        this->radius = radius;
        this->mass = mass;
        this->isMovable = movable;
    }

    Body(const double& x, const double& y, const int radius, const int mass, const double (&setVel)[2]) {
        this->pos[0] = x;
        this->pos[1] = y;
        this->radius = radius;
        this->mass = mass;
        this->vel[0] = setVel[0];
        this->vel[1] = setVel[1];
    }
};

struct Arr {
    int x, y;
};

extern std::vector<Body> bodies;
extern std::vector<Arr> futurePos;

const int width = 3000;
const int height = 2000;