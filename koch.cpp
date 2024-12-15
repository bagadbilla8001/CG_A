#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

class FractalCurve {
private:
    int currentDepth;
    int maxDepthLevel;
    float beginX, beginY, finishX, finishY;

public:
    FractalCurve(float bx, float by, float fx, float fy, int maxD)
        : currentDepth(0), maxDepthLevel(maxD), beginX(bx), beginY(by), finishX(fx), finishY(fy) {}

    void generateSegment(float x1, float y1, float x2, float y2, int depth) {
        if (depth == 0) {
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            return;
        }

        float dx = (x2 - x1) / 3.0f;
        float dy = (y2 - y1) / 3.0f;

        float x3 = x1 + dx;
        float y3 = y1 + dy;

        float x4 = x2 - dx;
        float y4 = y2 - dy;

        float midX = (x1 + x2) / 2;
        float midY = (y1 + y2) / 2;

        float peakX = midX + (sqrt(3.0f) * (y1 - y2)) / 6.0f;
        float peakY = midY + (sqrt(3.0f) * (x2 - x1)) / 6.0f;

        generateSegment(x1, y1, x3, y3, depth - 1);
        generateSegment(x3, y3, peakX, peakY, depth - 1);
        generateSegment(peakX, peakY, x4, y4, depth - 1);
        generateSegment(x4, y4, x2, y2, depth - 1);
    }

    void draw() {
        glBegin(GL_LINES);
        generateSegment(beginX, beginY, finishX, finishY, currentDepth);
        glEnd();
    }

    void increaseDepth() {
        if (currentDepth < maxDepthLevel) {
            currentDepth++;
            std::cout << "Current Depth: " << currentDepth << std::endl;
            glutPostRedisplay();
        } else {
            std::cout << "Reached Maximum Depth: " << maxDepthLevel << std::endl;
        }
    }
};

FractalCurve* fractal;
int winWidth = 800, winHeight = 800;

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.5, 1.0, 0.5);  // Light green curve
    fractal->draw();
    glFlush();
}

void reshapeWindow(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, winWidth, 0, winHeight);
    glMatrixMode(GL_MODELVIEW);
}

void mouseHandler(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        fractal->increaseDepth();
    }
}

int main(int argc, char** argv) {
    int maxDepth;
    std::cout << "Enter the maximum depth for the fractal: ";
    std::cin >> maxDepth;

    float startX = 100, startY = 400, endX = 700, endY = 400;

    fractal = new FractalCurve(startX, startY, endX, endY, maxDepth);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Fractal Curve Generator");

    glClearColor(0.0, 0.1, 0.1, 1.0);  // Dark teal background

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshapeWindow);
    glutMouseFunc(mouseHandler);

    glutMainLoop();

    delete fractal;
    return 0;
}
