#include <windows.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

class CurveFractal {
private:
    int depthLevel;

public:
    CurveFractal(int depth) : depthLevel(depth) {}

    void generateCurve(int n, float startX, float startY, float deltaX1, float deltaX2, float deltaY1, float deltaY2) {
        if (n <= 0) {
            float x = startX + (deltaX1 + deltaY1) / 2;
            float y = startY + (deltaX2 + deltaY2) / 2;
            glVertex2f(x, y);
        } else {
            generateCurve(n - 1, startX, startY, deltaY1 / 2, deltaY2 / 2, deltaX1 / 2, deltaX2 / 2);
            generateCurve(n - 1, startX + deltaX1 / 2, startY + deltaX2 / 2, deltaX1 / 2, deltaX2 / 2, deltaY1 / 2, deltaY2 / 2);
            generateCurve(n - 1, startX + deltaX1 / 2 + deltaY1 / 2, startY + deltaX2 / 2 + deltaY2 / 2, deltaX1 / 2, deltaX2 / 2, deltaY1 / 2, deltaY2 / 2);
            generateCurve(n - 1, startX + deltaX1 / 2 + deltaY1, startY + deltaX2 / 2 + deltaY2, -deltaY1 / 2, -deltaY2 / 2, -deltaX1 / 2, -deltaX2 / 2);
        }
    }

    void render() {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(0.0, 1.0, 0.0); // Green curve
        glBegin(GL_LINE_STRIP);
        generateCurve(depthLevel, -0.5, -0.5, 1, 0, 0, 1);
        glEnd();
        glFlush();
    }

    void setupGraphics() {
        glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
        glColor3f(0.0, 1.0, 0.0);         // Green drawing color
        gluOrtho2D(-1, 1, -1, 1);
    }
};

CurveFractal* fractalCurve;

void renderWrapper() {
    fractalCurve->render();
}

int main(int argc, char** argv) {
    int depth;
    cout << "Enter the depth for the curve: ";
    cin >> depth;

    fractalCurve = new CurveFractal(depth);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Fractal Curve Generator");

    fractalCurve->setupGraphics();
    glutDisplayFunc(renderWrapper);
    glutMainLoop();

    delete fractalCurve;

    return 0;
}
