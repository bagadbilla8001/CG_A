#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

// Constants for Cohen-Sutherland outcodes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Window dimensions and clipping boundaries
const int WIDTH = 800;
const int HEIGHT = 600;
int xmin, ymin, xmax, ymax;

// Structure to hold the end points of a line
struct Line {
    int x1, y1, x2, y2;
};

// Store the lines
vector<Line> lines;
vector<int> Boundary;
int counting = 0;

// Function prototypes
void display();
void mouse(int button, int state, int x, int y);
void clipLines();
void clipLine(int &x1, int &y1, int &x2, int &y2);
int computeOutcode(int x, int y);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Line Clipping with GLUT");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WIDTH, HEIGHT, 0.0);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}

bool clip_win=true;
bool first_p =false;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0); // Set color to red
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    for (size_t i = 0; i < lines.size(); ++i) {
        glVertex2i(lines[i].x1, lines[i].y1);
        glVertex2i(lines[i].x2, lines[i].y2);
    }
    glEnd();

    glFlush();
}

int xa,xb,ya,yb;
bool firstClick = true;
void mouse(int button, int state, int x, int y) {

    if(clip_win){
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && first_p == false) {
                xa = x;
                ya = y;
                first_p=true;
            }
            else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN &&  first_p==true) {
                xb=x;
                yb=y;
                xmin=min(xa,xb);
                xmax=max(xa,xb);
                ymin=min(ya,yb);
                ymax=max(ya,yb);
                clip_win=false;
                glutPostRedisplay();
            }
    }
    else{
        static Line line;
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

            if (firstClick) {
                line.x1 = x;
                line.y1 = y;
                firstClick = false;
                cout<<"hey"<<endl;
            }
            else {
                line.x2 = x;
                line.y2 = y;
                lines.push_back(line);
                counting++;
                firstClick = true;
            }
            glutPostRedisplay();
        }
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
            if (firstClick) {
                clipLines();
                firstClick = true;
            }
            glutPostRedisplay();
        }
    }
}

void clipLines() {
    for (size_t i = 0; i < lines.size(); ++i) {
        clipLine(lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
    }
}

void clipLine(int &x1, int &y1, int &x2, int &y2) {
    int outcode1 = computeOutcode(x1, y1);
    int outcode2 = computeOutcode(x2, y2);
    bool accept = false;

    while (true) {
        if (!(outcode1 | outcode2)) {
            // Both points inside
            accept = true;
            break;
        } else if (outcode1 & outcode2) {
            // Both points outside
            break;
        } else {
            // At least one point is outside
            int outcodeOut = outcode1 ? outcode1 : outcode2;
            int x, y;

            if (outcodeOut & TOP) { // Point is above the clip rectangle
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            } else if (outcodeOut & BOTTOM) { // Point is below the clip rectangle
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            } else if (outcodeOut & RIGHT) { // Point is to the right of clip rectangle
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            } else if (outcodeOut & LEFT) { // Point is to the left of clip rectangle
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (outcodeOut == outcode1) {
                x1 = x;
                y1 = y;
                outcode1 = computeOutcode(x1, y1);
            } else {
                x2 = x;
                y2 = y;
                outcode2 = computeOutcode(x2, y2);
            }
        }
    }
    if (!accept) {
        x1 = y1 = x2 = y2 = 0;
    }
}

int computeOutcode(int x, int y) {
    int outcode = INSIDE;

    if (x < xmin) outcode |= LEFT;
    else if (x > xmax) outcode |= RIGHT;
    if (y < ymin) outcode |= BOTTOM;
    else if (y > ymax) outcode |= TOP;

    return outcode;
}
