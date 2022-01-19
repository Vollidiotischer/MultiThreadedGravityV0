#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <mutex>
#include "Vars.h"
#include <cmath>

void init();
void drawLoop();
void draw();
void events();
void getMousePos(int*);
void drawCircle(int, int, int); 
void drawLine(int, int, int, int);
void drawRect(const float x, const float y, const float w, const float h);

static GLFWwindow* window;

static char pressOnce = ' '; 
static int clickPos[2]; 

static double velocity = 0.5; 


static std::mutex mtx;

void drawLoop() {
    init(); 

    while (!glfwWindowShouldClose(window)) {

        events();

        draw();

        glfwPollEvents();
    }

    glfwTerminate();
}

static void draw() {

    glClear(GL_COLOR_BUFFER_BIT);
    
    
    mtx.lock(); 
    //DRAWING BODY PATH
   
    if (futurePos.size() > 0) {
        //std::vector<Arr> usePos = futurePos;
        for (int i = 0; i < futurePos.size(); i++) {
            //std::cout << "Size: " << futurePos.size() << std::endl;
            drawCircle(futurePos[i].x, futurePos[i].y, 2);
        }
    }
    mtx.unlock(); 

    //DRAW FORCELINE FOR ALL BODIES
    /*
    mtx.lock(); 
    for (int i = 0; i < bodies.size(); i++) {
        //std::vector<DoubleArr> drawingAcc = bodies[i].acc;
        for (int i2 = 0; i2 < bodies[i].acc.size(); i2++) {
            //std::cout << (int)bodies[i].pos[0] << " " << (int)bodies[i].pos[1] << " " <<(int)(bodies[i].pos[0] + drawingAcc[i].xVel * 2000.0) << " " << (int)(bodies[i].pos[1] + drawingAcc[i].yVel * 2000.0) << std::endl; 
            drawLine((int)bodies[i].pos[0], (int)bodies[i].pos[1], (int)(bodies[i].pos[0] + bodies[i].acc[i].xVel), (int)(bodies[i].pos[1] + bodies[i].acc[i].yVel));//  * 2000.0
        }
    }
    mtx.unlock(); 
    */
    //DRAW ALL BODIES
    mtx.lock(); 
    for (Body b : bodies) {
        drawCircle((int)b.pos[0], (int)b.pos[1], b.radius);
    }
    mtx.unlock(); 
    

    glfwSwapBuffers(window);
}

static void drawRect(const float x, const float y, const float w, const float h) {
    glBegin(GL_QUADS);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

static void events() {
    // CREATING NEW BODIES
    if (glfwGetMouseButton(window, 0) == GL_TRUE) {
        if (pressOnce == ' ') {
            pressOnce = 'l';

            getMousePos(clickPos);

            bodies.push_back(Body(clickPos[0], clickPos[1], 30, 100, { 0.0, 0.0 }));
            bodies[bodies.size() - 1].isBeingCreated = true;

        }

        if (bodies.size() > 0) {
            int calculatingPos[2];
            getMousePos(calculatingPos);
            bodies[bodies.size() - 1].vel[0] = (clickPos[0] - (double)calculatingPos[0]) / 50.0;
            bodies[bodies.size() - 1].vel[1] = (clickPos[1] - (double)calculatingPos[1]) / 50.0;
        }

    }
    else {
        if (glfwGetMouseButton(window, 1) == GL_TRUE) {
            if (pressOnce == ' ') {
                pressOnce = 'r';
                int pos[2]; 
                getMousePos(pos);
                bodies.push_back(Body(pos[0], pos[1], 30, 100, false));
                futurePos.clear();
            }
        }
        else {
            if (pressOnce == 'l') {
                int currentPos[2];
                getMousePos(currentPos);
                if (bodies.size() > 0) {
                    bodies[bodies.size() - 1].isBeingCreated = false;
                }
            }
            pressOnce = ' ';
        }
    }

    //MOVING THE LAST CREATED BODY
    if (bodies.size() > 0) {
        if (glfwGetKey(window, 'A') == GL_TRUE) {
            bodies[bodies.size() - 1].vel[0] -= velocity;
        }
        else {
            if (glfwGetKey(window, 'D') == GL_TRUE) {
                bodies[bodies.size() - 1].vel[0] += velocity;
            }
        }
        if (glfwGetKey(window, 'W') == GL_TRUE) {
            bodies[bodies.size() - 1].vel[1] -= velocity;
        }
        else {
            if (glfwGetKey(window, 'S') == GL_TRUE) {
                bodies[bodies.size() - 1].vel[1] += velocity;
            }
        }
    }


    //MOVING THE PERSPECTIVE / SCREEN
    if (bodies.size() > 0) {
        if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GL_TRUE) {
            int move[] = { 0, 0, };
            if (glfwGetKey(window, GLFW_KEY_UP) == GL_TRUE) {
                move[0] = 1;
                move[1] = 20;
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GL_TRUE) {
                move[0] = 1;
                move[1] = -20;
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GL_TRUE) {
                move[0] = 0;
                move[1] = -20;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GL_TRUE) {
                move[0] = 0;
                move[1] = 20;
            }

            for (Body b : bodies) {
                b.pos[move[0]] += move[1];
            }
        }
    }
    //UTILITY

    if (glfwGetKey(window, 'C') == GL_TRUE) {
        mtx.lock(); 
        if (bodies.size() > 0) {
            if (!bodies[bodies.size() - 1].isBeingCreated) {
                bodies.clear();
                futurePos.clear(); 
            }
        }
        mtx.unlock();
    }

}

static void getMousePos(int* arr) {

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    arr[0] = (int)x;
    arr[1] = (int)y; 

}

void drawCircle(int x, int y, int r) {
    glPushMatrix(); 
    glTranslatef((float)x, (float)y, 0);
    glScalef((float)r, (float)r, 1);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    glVertex2f(0, 0);
    for (int i = 0; i <= 360; i++) { //NUM_PIZZA_SLICES decides how round the circle looks.
        double angle = 3.141592 * 2 * i / 15;
        glVertex2f((float)cos(angle), (float)sin(angle));
    }
    glEnd();

    glPopMatrix();
}

void drawLine(int x1, int y1, int x2, int y2) {
    glLineWidth(5.0);
    glBegin(GL_LINES);
    glColor4f(1.f, 0.2f, 0.2f, 1.f);
    glVertex2f((GLfloat)x1, (GLfloat)y1);
    glVertex2f((GLfloat)x2, (GLfloat)y2);
    glEnd();
}

static void init() {

    /* Initialize the library */
    if (!glfwInit()) {
        throw "Failed to initialize";
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw "Failed to create window";
    }


    /* Make the window's context current */
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);

    glewInit();

    glOrtho(0.f, width, height, 0.f, 0.f, 1.f);

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
}