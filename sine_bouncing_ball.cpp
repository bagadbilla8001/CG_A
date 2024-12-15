#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int screenW = 800;
const int screenH = 600;

// Base class for moving objects
class MovingObject {
public:
    virtual void move() = 0;  // Pure virtual function to move objects
    virtual void draw() = 0;   // Virtual draw method to be overridden by derived classes
};

// Derived class for bouncing ball moving forward
class ForwardBouncingBall : public MovingObject {
public:
    float radius;
    float xCoord;
    float yCoord;
    float speed;

    ForwardBouncingBall(float r, float startX, float startY, float sp)
        : radius(r), xCoord(startX), yCoord(startY), speed(sp) {}

    // Override move function for the ForwardBouncingBall class
    void move() override {
        xCoord += speed;
        if (xCoord > 1.0f) {
            xCoord = -1.0f;  // Wrap around
        }
        yCoord = cos(1.57-3.14f * xCoord);  // Adjusting y position based on x
    }

    // Override draw function for the ForwardBouncingBall class
    void draw() override {
        glColor3f(0.0f, 0.5f, 1.0f);
        glTranslatef(xCoord, yCoord, 0.0f);
        glutSolidSphere(radius, 50, 50);
        glTranslatef(-xCoord, -yCoord, 0.0f);
    }
};

// Derived class for bouncing ball moving backward
class BackwardBouncingBall : public MovingObject {
public:
    float radius;
    float xCoord;
    float yCoord;
    float speed;

    BackwardBouncingBall(float r, float startX, float startY, float sp)
        : radius(r), xCoord(startX), yCoord(startY), speed(sp) {}

    // Override move function for the BackwardBouncingBall class
    void move() override {
        xCoord -= speed;  // Move backward
        if (xCoord < -1.0f) {
            xCoord = 1.0f;  // Wrap around
        }
        yCoord = cos(1.57-3.14f * xCoord);  // Adjusting y position based on x
    }

    // Override draw function for the BackwardBouncingBall class
    void draw() override {
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(xCoord, yCoord, 0.0f);
        glutSolidSphere(radius, 50, 50);
        glTranslatef(-xCoord, -yCoord, 0.0f);
    }
};

// Class for drawing the cosine curve (no need for polymorphism here)
class CosineCurve {
public:
    void draw() {
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        for (float x = -1.0f; x <= 1.0f; x += 0.01f) {
            float y = cos(1.57-3.14f * x);
            glVertex2f(x, y);
        }
        glEnd();
    }
};

MovingObject* ball;  // Pointer to the base class MovingObject
CosineCurve curve;

// Flag to determine which ball is in use
bool isMovingForward = true;

// Function to render the scene
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    curve.draw();
    ball->draw();  // Polymorphic call to the ball's draw method
    glutSwapBuffers();
}

// Function to update the scene
void update(int value) {
    ball->move();  // Polymorphic call to the ball's move method
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// Function to setup OpenGL
void setupOpenGL() {
    glClearColor(0.1f, 0.0f, 0.1f, 1.0f);
}

// Mouse callback to switch between forward and backward ball movement
void mouseCallback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Switch the movement direction of the ball
        if (isMovingForward) {
            // Switch to moving backward
            delete ball;  // Delete the current ball object to avoid memory leak
            ball = new BackwardBouncingBall(0.05f, 1.0f, 0.0f, 0.01f);
        } else {
            // Switch to moving forward
            delete ball;  // Delete the current ball object to avoid memory leak
            ball = new ForwardBouncingBall(0.05f, -1.0f, 0.0f, 0.01f);
        }

        // Toggle the flag to track the ball's movement direction
        isMovingForward = !isMovingForward;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screenW, screenH);
    glutCreateWindow("Bouncing Ball Along Cosine Curve");
    setupOpenGL();

    // Initially set the ball to move forward
    ball = new ForwardBouncingBall(0.05f, -1.0f, 0.0f, 0.01f);

    // Register callbacks
    glutDisplayFunc(renderScene);
    glutTimerFunc(0, update, 0);
    glutMouseFunc(mouseCallback);  // Register mouse callback function

    glutMainLoop();
    return 0;
}
