#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

const int NONE = 0;
const int BRESENHAM = 1;
const int MIDPOINT = 2;
int currentAlgorithm = NONE;

int xc, yc, r;

// Base class Pixel responsible for drawing pixels
class Pixel {
public:
    // Overloaded method to set pixel with integer coordinates
    void setPixel(int x, int y) {
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();
    }

    // Overloaded method to set pixel with float coordinates
    void setPixel(float x, float y) {
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
        glFlush();
    }
};

// Derived class Circle that handles different circle drawing algorithms
class Circle : public Pixel {
public:
    // Function to draw a circle using Bresenham's algorithm
    void drawCircleBresenham(int xc, int yc, int r) {
        int x = 0, y = r;
        int d = 3 - 2 * r;
        while (y >= x) {
            setPixel(xc + x, yc + y);
            setPixel(xc - x, yc + y);
            setPixel(xc + x, yc - y);
            setPixel(xc - x, yc - y);
            setPixel(xc + y, yc + x);
            setPixel(xc - y, yc + x);
            setPixel(xc + y, yc - x);
            setPixel(xc - y, yc - x);
            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            } else {
                d = d + 4 * x + 6;
            }
            x++;
        }
    }

    // Function to draw a circle using Midpoint algorithm
    void drawCircleMidpoint(int xc, int yc, int r) {
        int x = r, y = 0;
        int p = 1 - r;
        while (x > y) {
            y++;
            if (p <= 0) {
                p = p + 2 * y + 1;
            } else {
                x--;
                p = p + 2 * y - 2 * x + 1;
            }
            if (x < y) break;
            setPixel((float)(xc + x), (float)(yc + y));
            setPixel(xc - x, yc + y);
            setPixel(xc + x, yc - y);
            setPixel(xc - x, yc - y);
            if (x != y) {
                setPixel(xc + y, yc + x);
                setPixel(xc - y, yc + x);
                setPixel(xc + y, yc - x);
                setPixel(xc - y, yc - x);
            }
        }
    }
};

Circle circle;  // Create an object of Circle class

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    switch (currentAlgorithm) {
        case BRESENHAM:
            glColor3f(0.0, 1.0, 0.0);  // Green color
            circle.drawCircleBresenham(xc, yc, r);
            break;
        case MIDPOINT:
            glColor3f(0.0, 0.0, 1.0);  // Blue color
            circle.drawCircleMidpoint(xc, yc, r);
            break;
        default:
            break;
    }
}

void menu(int option) {
    currentAlgorithm = option;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        exit(0);
    }
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

int main(int argc, char** argv) {
    cout << "Enter the center of the circle (xc yc): ";
    cin >> xc >> yc;
    cout << "Enter the radius of the circle: ";
    cin >> r;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle Drawing Algorithms");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutCreateMenu(menu);
    glutAddMenuEntry("Bresenham Algorithm", BRESENHAM);
    glutAddMenuEntry("Midpoint Algorithm", MIDPOINT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 0;
}