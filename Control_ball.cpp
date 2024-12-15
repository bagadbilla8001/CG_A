#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#define M_PI 3.14

// Define the base Ball class
class Ball {
public:
    float x, y;
    float velocity;
    float radius;

    Ball(float x, float y, float velocity, float radius) {
        this->x = x;
        this->y = y;
        this->velocity = velocity;
        this->radius = radius;
    }

    virtual void move(int direction) {
        switch (direction) {
            case GLUT_KEY_UP:
                y += velocity;
                break;
            case GLUT_KEY_DOWN:
                y -= velocity;
                break;
            case GLUT_KEY_LEFT:
                x -= velocity;
                break;
            case GLUT_KEY_RIGHT:
                x += velocity;
                break;
        }
    }

    virtual void draw() {
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i <= 360; i++) {
            float angle = i * M_PI / 180.0f;
            glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
        }
        glEnd();
    }

    virtual ~Ball() {}  // Virtual destructor for proper cleanup
};

// Derive the BouncingBall class
class BouncingBall : public Ball {
public:
    BouncingBall(float x, float y, float velocity, float radius) : Ball(x, y, velocity, radius) {}

    void move(int direction) override {
        Ball::move(direction);

        // Check for bouncing off the window edges
        if (x - radius < 0 || x + radius > glutGet(GLUT_WINDOW_WIDTH)) {
            velocity *= -1;
        }
        if (y - radius < 0 || y + radius > glutGet(GLUT_WINDOW_HEIGHT)) {
            velocity *= -1;
        }
    }
};

// Derive the RollingBall class
class RollingBall : public Ball {
public:
    RollingBall(float x, float y, float velocity, float radius) : Ball(x, y, velocity, radius) {}

    void move(int direction) override {
        Ball::move(direction);

        // Check for rolling off the window edges
        if (x - radius < 0 || x + radius > glutGet(GLUT_WINDOW_WIDTH)) {
            velocity = 0;
        }
        if (y - radius < 0 || y + radius > glutGet(GLUT_WINDOW_HEIGHT)) {
            velocity = 0;
        }
    }
};

// Global variables
Ball *ball;  // Pointer to base class (polymorphism in action)

// OpenGL functions
void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    ball->draw();  // Polymorphic call to the appropriate draw method
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void special(int key, int x, int y) {
    ball->move(key);  // Polymorphic call to the appropriate move method
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Ball Control");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);

    // Create a bouncing ball using polymorphism
    //ball = new BouncingBall(320, 240, 5.0f, 20.0f);  // You can change this to RollingBall or any derived class
    ball = new RollingBall(320, 240, 5.0f, 20.0f); // Uncomment to test RollingBall instead

    glutMainLoop();
    return 0;
}
