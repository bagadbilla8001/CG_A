#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h>

class Pattern {
public:
    void setPixel(int x, int y) {
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();
    }

    void drawLineDDA(int x1, int y1, int x2, int y2) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        int steps = std::max(abs(dx), abs(dy));
        float xIncrement = dx / (float)steps;
        float yIncrement = dy / (float)steps;
        float x = x1;
        float y = y1;

        for (int i = 0; i <= steps; i++) {
            setPixel(round(x), round(y));
            x += xIncrement;
            y += yIncrement;
        }
    }

    void drawCircleBressen(int xc, int yc, int r) {//bressenham
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

        int x1 = 100, y1 = 100;
        int x2 = 300, y2 = 100;
        int x3 = 300, y3 = 200;
        int x4 = 100, y4 = 200;
    void drawRectangleWithMidpoints() {

        drawLineDDA(x1, y1, x2, y2);
        drawLineDDA(x2, y2, x3, y3);
        drawLineDDA(x3, y3, x4, y4);
        drawLineDDA(x4, y4, x1, y1);
    }

    void drawParallelogramWithMidpoints() {
        int mx1 = (x1 + x2) / 2, my1 = (y1 + y2) / 2;
        int mx2 = (x2 + x3) / 2, my2 = (y2 + y3) / 2;
        int mx3 = (x3 + x4) / 2, my3 = (y3 + y4) / 2;
        int mx4 = (x4 + x1) / 2, my4 = (y4 + y1) / 2;

        drawLineDDA(mx1, my1, mx2, my2);
        drawLineDDA(mx2, my2, mx3, my3);
        drawLineDDA(mx3, my3, mx4, my4);
        drawLineDDA(mx4, my4, mx1, my1);
    }

    void drawCircle() {
        int centerX = (x1+x2)/2;
        int centerY = (y2+y3)/2;
        int radius = 44.72;

        drawCircleBressen(centerX, centerY, radius);
    }
};

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Pattern pattern;
    pattern.drawRectangleWithMidpoints();
    pattern.drawParallelogramWithMidpoints();
    pattern.drawCircle();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pattern Drawing using DDA Algorithm");
    gluOrtho2D(0, 500, 0, 500);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
