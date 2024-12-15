#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h>

using namespace std;

class Pixel {
public:
    int x, y;
    Pixel(int x, int y) {
        this->x = x;
        this->y = y;
    }
    void drawPixel(int x, int y) {
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }
    void drawPixel(float x, float y) {
        glBegin(GL_POINTS);
        glVertex2i(round(x), round(y));
        glEnd();
    }
};

class LineDrawer : public Pixel {
public:
    LineDrawer(int x, int y) : Pixel(x, y) {}

    void drawLineDDA(int x1, int y1, int x2, int y2) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        int steps = max(abs(dx), abs(dy));
        float xIncrement = dx / float(steps);
        float yIncrement = dy / float(steps);
        float x = x1, y = y1;

        for (int i = 0; i <= steps; i++) {
            drawPixel(x, 480 - y); // Invert y-coordinate
            x += xIncrement;
            y += yIncrement;
        }
    }

    void drawLineBresenham(int x1, int y1, int x2, int y2) {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        int x = x1, y = y1;

        while (x != x2 && y != y2) {
            drawPixel(x, 480 - y); // Invert y-coordinate
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x += sx;
            }
            if (e2 < dx) {
                err += dx;
                y += sy;
            }
        }
        drawPixel(x, 480 - y); // Invert y-coordinate
    }
};

LineDrawer lineDrawer(0, 0);

int xStart, yStart, xEnd, yEnd;
bool useDDA = true, firstClick = true;

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (firstClick) {
            xStart = x;
            yStart = y;
            firstClick = false;
        } else {
            xEnd = x;
            yEnd = y;
            firstClick = true;
            glutPostRedisplay();
        }
    }
}

void display_line() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (useDDA) {
        glColor3f(1.0, 0.0, 0.0);
        lineDrawer.drawLineDDA(xStart, yStart, xEnd, yEnd);
    } else {
        glColor3f(0.0, 1.0, 0.0);
        lineDrawer.drawLineBresenham(xStart, yStart, xEnd, yEnd);
    }
    glFlush();
}

void changeAlgorithm(int option) {
    if (option == 1) {
        useDDA = true;
    } else if (option == 2) {
        useDDA = false;
    }
    glutPostRedisplay();
}

void createMenu() {
    int menu = glutCreateMenu(changeAlgorithm);
    glutAddMenuEntry("DDA Algorithm", 1);
    glutAddMenuEntry("Bresenham Algorithm", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Line Drawing Algorithms (DDA & Bresenham)");

    init();

    createMenu();

    glutMouseFunc(mouseClick);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display_line);

    glutMainLoop();

    return 0;
}
