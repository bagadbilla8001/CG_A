#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

#define WIDTH 500
#define HEIGHT 500

float pointsArray[30][2], transformedPoints[30][2];
int pointCount = 0, vertexCount;

void initialize() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);
}

void drawPixel(GLint x, GLint y) {
    glColor3f(1.0, 0.5, 0.0); // Changed color to orange
    glPointSize(2.0);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
    glFlush();
}

class GeometryTransform {
public:
    void matrixMultiply(float matrix[3][3], float result[30][2], int size) {
        for (int i = 0; i < size; i++) {
            result[i][0] = matrix[0][0] * pointsArray[i][0] + matrix[0][1] * pointsArray[i][1] + matrix[0][2];
            result[i][1] = matrix[1][0] * pointsArray[i][0] + matrix[1][1] * pointsArray[i][1] + matrix[1][2];
        }
    }

    void renderPolygon(float matrix[][2], int size) {
        glColor3f(0.0, 0.0, 1.0); // Keep color blue for the polygon
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < size; i++)
            glVertex2f(matrix[i][0], matrix[i][1]);
        glEnd();
        glFlush();
    }

    void translate(float translateX, float translateY) {
        float translationMatrix[3][3] = {
            {1, 0, translateX},
            {0, 1, translateY},
            {0, 0, 1}
        };
        matrixMultiply(translationMatrix, transformedPoints, vertexCount);
        renderPolygon(transformedPoints, vertexCount);
    }

    void scale(float scaleX, float scaleY) {
        float scalingMatrix[3][3] = {
            {scaleX, 0, 0},
            {0, scaleY, 0},
            {0, 0, 1}
        };
        matrixMultiply(scalingMatrix, transformedPoints, vertexCount);
        renderPolygon(transformedPoints, vertexCount);
    }

    void rotate(float rotationAngle, bool isClockwise) {
        float radians = rotationAngle * (3.14 / 180);
        float rotationMatrix[3][3] = {
            {cos(radians), isClockwise ? sin(radians) : -sin(radians), 0},
            {isClockwise ? -sin(radians) : sin(radians), cos(radians), 0},
            {0, 0, 1}
        };
        matrixMultiply(rotationMatrix, transformedPoints, vertexCount);
        renderPolygon(transformedPoints, vertexCount);
    }

    void reflect(char reflectionAxis) {
        float reflectionMatrix[3][3];
        if (reflectionAxis == 'x' || reflectionAxis == 'X') {
            reflectionMatrix[0][0] = 1;   reflectionMatrix[0][1] = 0;   reflectionMatrix[0][2] = 0;
            reflectionMatrix[1][0] = 0;   reflectionMatrix[1][1] = -1;  reflectionMatrix[1][2] = 0;
            reflectionMatrix[2][0] = 0;   reflectionMatrix[2][1] = 0;   reflectionMatrix[2][2] = 1;
        } else if (reflectionAxis == 'y' || reflectionAxis == 'Y') {
            reflectionMatrix[0][0] = -1;  reflectionMatrix[0][1] = 0;   reflectionMatrix[0][2] = 0;
            reflectionMatrix[1][0] = 0;   reflectionMatrix[1][1] = 1;   reflectionMatrix[1][2] = 0;
            reflectionMatrix[2][0] = 0;   reflectionMatrix[2][1] = 0;   reflectionMatrix[2][2] = 1;
        } else {
            cout << "\nInvalid reflection axis! Please use 'x' or 'y'.";
            return;
        }
        matrixMultiply(reflectionMatrix, transformedPoints, vertexCount);
        renderPolygon(transformedPoints, vertexCount);
    }

    void shear(char shearAxis, float shearFactor) {
        float shearingMatrix[3][3] = {
            {1, (shearAxis == 'x' ? shearFactor : 0), 0},
            {(shearAxis == 'y' ? shearFactor : 0), 1, 0},
            {0, 0, 1}
        };
        matrixMultiply(shearingMatrix, transformedPoints, vertexCount);
        renderPolygon(transformedPoints, vertexCount);
    }
} geometryTransform;

void createMenu(int selection) {
    float translateX, translateY, scaleX, scaleY, rotationAngle;
    char axis;
    if (selection == 1) {
        cout << "\nTranslation->\nEnter tx factor::";
        cin >> translateX;
        cout << "\nEnter ty factor::";
        cin >> translateY;
        geometryTransform.translate(translateX, translateY);
        cout << "Translation Complete\n";
    } else if (selection == 2) {
        cout << "\nScaling:\nEnter Sx factor::";
        cin >> scaleX;
        cout << "\nEnter Sy factor::";
        cin >> scaleY;
        geometryTransform.scale(scaleX, scaleY);
        cout << "Scaling Complete\n";
    } else if (selection == 3) {
        cout << "\nRotation:\nEnter angle::";
        cin >> rotationAngle;
        cout << "1.For Anti-Clockwise rotation\n2.For Clockwise rotation\nEnter your choice::";
        int rotationChoice;
        cin >> rotationChoice;
        geometryTransform.rotate(rotationAngle, rotationChoice == 2);
        cout << "Rotation Complete\n";
    } else if (selection == 4) {
        cout << "\nEnter reflection axis (x/y): ";
        cin >> axis;
        geometryTransform.reflect(axis);
        cout << "Reflection Complete\n";
    } else if (selection == 5) {
        cout << "\nEnter shearing axis (x/y): ";
        cin >> axis;
        float factor;
        cout << "Enter shearing factor: ";
        cin >> factor;
        geometryTransform.shear(axis, factor);
        cout << "Shearing Complete\n";
    } else if (selection == 6) {
        exit(0);
    } else {
        cout << "\nInvalid option. Try again.";
    }
}

void handleKeyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);

    if (key == 'l' || key == 'L') {
        glColor3f(0.0, 0.0, 1.0); // Keep color blue for polygon
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < pointCount; i++)
            glVertex2f(pointsArray[i][0], pointsArray[i][1]);
        glEnd();
        vertexCount = pointCount;
        pointCount = 0;
        glFlush();
    }
}

void handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        pointsArray[pointCount][0] = (float)(x - 250);
        pointsArray[pointCount][1] = (float)(250 - y);
        glColor3f(0.0, 1.0, 0.0); // Changed color to green for points
        glPointSize(3.0);
        glBegin(GL_POINTS);
        glVertex2f(pointsArray[pointCount][0], pointsArray[pointCount][1]);
        glEnd();
        pointCount++;
        glFlush();
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("2D Geometry Transformations");
    cout << "Use the mouse pointer to place points on the graphics screen.\nUse 'l' or 'L' to create a polygon on the graphics window.\nUse the right mouse button to access the 2D transformations menu.";
    glutDisplayFunc(initialize);
    glutKeyboardFunc(handleKeyboard);
    glutMouseFunc(handleMouse);

    glutCreateMenu(createMenu);
    glutAddMenuEntry("TRANSLATION", 1);
    glutAddMenuEntry("SCALING", 2);
    glutAddMenuEntry("ROTATION", 3);
    glutAddMenuEntry("REFLECTION", 4);
    glutAddMenuEntry("SHEARING", 5);
    glutAddMenuEntry("EXIT", 6);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
