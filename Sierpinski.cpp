#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

using namespace std;

class SierpinskiTriangle {
private:
    int depth;  // The depth for the recursion
    
public:
    // Constructor to initialize the depth
    SierpinskiTriangle(int d) : depth(d) {}

    // Function to draw a single triangle
    void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
        glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
        glEnd();
    }

    // Recursive function to draw the Sierpinski Triangle
    void sierpinski(float x1, float y1, float x2, float y2, float x3, float y3, int d) {
        if (d == 0) {
            drawTriangle(x1, y1, x2, y2, x3, y3);
            return;
        }

        float x12 = (x1 + x2) / 2;
        float y12 = (y1 + y2) / 2;
        float x23 = (x2 + x3) / 2;
        float y23 = (y2 + y3) / 2;
        float x31 = (x3 + x1) / 2;
        float y31 = (y3 + y1) / 2;

        sierpinski(x1, y1, x12, y12, x31, y31, d - 1);
        sierpinski(x12, y12, x2, y2, x23, y23, d - 1);
        sierpinski(x31, y31, x23, y23, x3, y3, d - 1);
    }

    // Function to initiate drawing the Sierpinski triangle
    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 0.0, 0.0); // Red color for the triangles
        sierpinski(-1.0, -0.5, 1.0, -0.5, 0.0, 1.0, depth); // Starting coordinates
        glFlush();
    }

    // Function to handle window resizing and setting the projection
    void reshape(int w, int h) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-1.5, 1.5, -1.0, 1.0); // Set up the orthogonal view
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }
};

SierpinskiTriangle* fractal;

void display() {
    fractal->display();  // Delegate to the SierpinskiTriangle class
}

void reshape(int w, int h) {
    fractal->reshape(w, h);  // Delegate to the SierpinskiTriangle class
}

int main(int argc, char** argv) {
    // Get the depth of recursion from the user
    int depth;
    cout << "Enter the number of iterations: ";
    cin >> depth;

    // Create an instance of SierpinskiTriangle with the given depth
    fractal = new SierpinskiTriangle(depth);

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Sierpinski Triangle");

    // Register display and reshape functions with GLUT
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Enter the GLUT main loop
    glutMainLoop();

    // Clean up
    delete fractal;

    return 0;
}
