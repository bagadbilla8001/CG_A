#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h>

// Define constants
const int WIDTH = 800;
const int HEIGHT = 600;
const int RADIUS_CIRCLE = 100;

class PixelDrawer {
public:
    static void drawPixel(int x, int y) {
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
    }
};
class LineDrawer : public PixelDrawer {
public:
    static void drawLine(int x1, int y1, int x2, int y2) {
        int d1 = x2 - x1;
        int d2 = y2 - y1;
        int s = std::max(abs(d1), abs(d2));

        float xi = d1 / (float)s;
        float yi = d2 / (float)s;

        float x = x1;
        float y = y1;

        for (int i = 0; i <= s; ++i) {
            glBegin(GL_POINTS);
            glVertex2i(round(x), round(y));
            glEnd();
            x += xi;
            y += yi;
        }
    }
};

class CircleDrawer : public PixelDrawer {
public:
    static void drawCircle(int xc, int yc, int r) {//bressenham
        int x = 0, y = r;
        int d = 3 - 2 * r;
        while (y >= x) {
            drawPixel(xc + x, yc + y);
            drawPixel(xc - x, yc + y);
            drawPixel(xc + x, yc - y);
            drawPixel(xc - x, yc - y);
            drawPixel(xc + y, yc + x);
            drawPixel(xc - y, yc + x);
            drawPixel(xc + y, yc - x);
            drawPixel(xc - y, yc - x);
            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            } else {
                d = d + 4 * x + 6;
            }
            x++;
        }
    }
    static void drawInscribedCircle(int x_center, int y_center, int radius) {
        double inscribed_radius = radius / 2.0;
        drawCircle(x_center, y_center, static_cast<int>(inscribed_radius));
    }

};

class TriangleDrawer: public LineDrawer{
public:
    static void drawTriangle(int x_center, int y_center, int radius) {
        double height = radius;
        int x1 = x_center;
        int y1 = y_center + radius;

        int x2 = x_center - radius * sqrt(3) / 2.0;
        int y2 = y_center - height / 2.0;

        int x3 = x_center + radius * sqrt(3) / 2.0;
        int y3 = y_center - height / 2.0;

        drawLine(x1, y1, x2, y2);
        drawLine(x2, y2, x3, y3);
        drawLine(x3, y3, x1, y1);
    }
};

class Scene {
public:
    static void display() {
        glClear(GL_COLOR_BUFFER_BIT);

        int x_center = WIDTH / 2;
        int y_center = HEIGHT / 2;
        glColor3f(1.0, 0.0, 0.0);

        CircleDrawer::drawCircle(x_center, y_center, RADIUS_CIRCLE);

        TriangleDrawer::drawTriangle(x_center, y_center, RADIUS_CIRCLE);

        CircleDrawer::drawInscribedCircle(x_center, y_center, RADIUS_CIRCLE);

        glFlush();
    }
};

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle and Triangle Pattern");

    init();
    glutDisplayFunc(Scene::display);
    glutMainLoop();

    return 0;
}
