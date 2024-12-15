#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>
using namespace std;

#define PI 3.14159265358979323846

class SnowflakeGenerator {
public:
    int depthLevel;

    SnowflakeGenerator(int depth) : depthLevel(depth) {}

    void drawLineSegment(float startX, float startY, float endX, float endY, int depth) {
        if (depth == 0) {
            glBegin(GL_LINES);
            glVertex2f(startX, startY);
            glVertex2f(endX, endY);
            glEnd();
        } else {
            float middleX1 = (2 * startX + endX) / 3.0;
            float middleY1 = (2 * startY + endY) / 3.0;
            float middleX2 = (startX + 2 * endX) / 3.0;
            float middleY2 = (startY + 2 * endY) / 3.0;

            float midPointX = (middleX1 + middleX2) / 2.0;
            float midPointY = (middleY1 + middleY2) / 2.0;

            float deltaX = middleX2 - middleX1;
            float deltaY = middleY2 - middleY1;

            float peakX = midPointX + deltaY * sqrt(3) / 2.0;
            float peakY = midPointY - deltaX * sqrt(3) / 2.0;

            drawLineSegment(startX, startY, middleX1, middleY1, depth - 1);
            drawLineSegment(middleX1, middleY1, peakX, peakY, depth - 1);
            drawLineSegment(peakX, peakY, middleX2, middleY2, depth - 1);
            drawLineSegment(middleX2, middleY2, endX, endY, depth - 1);
        }
    }

    void generateSnowflake() {
        float scale = 0.6;
        float vertexA_X = scale * cos(PI / 6);
        float vertexA_Y = scale * sin(PI / 6);
        float vertexB_X = scale * cos(5 * PI / 6);
        float vertexB_Y = scale * sin(5 * PI / 6);
        float vertexC_X = scale * cos(3 * PI / 2);
        float vertexC_Y = scale * sin(3 * PI / 2);

        drawLineSegment(vertexA_X, vertexA_Y, vertexB_X, vertexB_Y, depthLevel);
        drawLineSegment(vertexB_X, vertexB_Y, vertexC_X, vertexC_Y, depthLevel);
        drawLineSegment(vertexC_X, vertexC_Y, vertexA_X, vertexA_Y, depthLevel);
    }
};

SnowflakeGenerator* snowflakeApp;

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.5);
    snowflakeApp->generateSnowflake();
    glFlush();
}

void setupOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(0.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv) {
    int levelOfRecursion;
    cout << "Enter the depth level for the fractal snowflake: ";
    cin >> levelOfRecursion;

    snowflakeApp = new SnowflakeGenerator(levelOfRecursion);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Fractal Snowflake Generator");

    setupOpenGL();
    glutDisplayFunc(renderScene);
    glutMainLoop();

    delete snowflakeApp;
    return 0;
}
