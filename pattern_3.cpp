#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h> // For round function

class Pattern {
public:
    // Window dimensions
    const int windowWidth = 800;
    const int windowHeight = 600;

    // Rectangle coordinates
    const float rectLeft = 100.0f;
    const float rectRight = 400.0f;
    const float rectTop = 100.0f;
    const float rectBottom = 300.0f;

    // Calculate midpoints of the rectangle's adjacent sides
    const float midX1 = (rectLeft + rectRight) / 2.0f;
    const float midY1 = rectTop;
    const float midX2 = rectRight;
    const float midY2 = (rectTop + rectBottom) / 2.0f;
    const float midX3 = (rectLeft + rectRight) / 2.0f;
    const float midY3 = rectBottom;
    const float midX4 = rectLeft;
    const float midY4 = (rectTop + rectBottom) / 2.0f;

    // Calculate midpoints of the parallelogram's sides
    const float paraMidX1 = (midX1 + midX2) / 2.0f;
    const float paraMidY1 = (midY1 + midY2) / 2.0f;
    const float paraMidX2 = (midX2 + midX3) / 2.0f;
    const float paraMidY2 = (midY2 + midY3) / 2.0f;
    const float paraMidX3 = (midX3 + midX4) / 2.0f;
    const float paraMidY3 = (midY3 + midY4) / 2.0f;
    const float paraMidX4 = (midX4 + midX1) / 2.0f;
    const float paraMidY4 = (midY4 + midY1) / 2.0f;

    // Function to set pixel color (a workaround to draw points)
    void setPixel(int x, int y) {
        glBegin(GL_POINTS);
        glColor3f(0.0,0.0,0.0);
        glVertex2i(x, y);
        glEnd();
    }

    // DDA line generation algorithm
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

    void drawRectangle() {
        // Draw lines using DDA algorithm
        drawLineDDA(rectLeft, rectTop, rectRight, rectTop);       // Top
        drawLineDDA(rectRight, rectTop, rectRight, rectBottom);   // Right
        drawLineDDA(rectRight, rectBottom, rectLeft, rectBottom); // Bottom
        drawLineDDA(rectLeft, rectBottom, rectLeft, rectTop);     // Left
    }

    void drawParallelogram() {
        // Draw parallelogram using DDA algorithm
        drawLineDDA(midX1, midY1, midX2, midY2); // Top to right
        drawLineDDA(midX2, midY2, midX3, midY3); // Right to bottom
        drawLineDDA(midX3, midY3, midX4, midY4); // Bottom to left
        drawLineDDA(midX4, midY4, midX1, midY1); // Left to top
    }

    void drawInnerRectangle() {
        // Draw inner rectangle using DDA algorithm
        drawLineDDA(paraMidX1, paraMidY1, paraMidX2, paraMidY2); // Top side
        drawLineDDA(paraMidX2, paraMidY2, paraMidX3, paraMidY3); // Right side
        drawLineDDA(paraMidX3, paraMidY3, paraMidX4, paraMidY4); // Bottom side
        drawLineDDA(paraMidX4, paraMidY4, paraMidX1, paraMidY1); // Left side
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        drawRectangle();
        drawParallelogram();
        drawInnerRectangle();
        glFlush();
    }

    void reshape(int width, int height) {
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, windowWidth, 0, windowHeight);
        glMatrixMode(GL_MODELVIEW);
    }
};

Pattern pattern;

void display() {
    pattern.display();
}

void reshape(int width, int height) {
    pattern.reshape(width, height);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(pattern.windowWidth, pattern.windowHeight);
    glutCreateWindow("Nested Shapes using DDA");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
