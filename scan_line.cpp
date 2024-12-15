#include <windows.h>
#include <GL/glut.h>
#include <vector>
#include <algorithm>

struct Point {
    int x, y;
};

std::vector<Point> points;  // To store polygon points
bool isPolygonComplete = false;  // To check if the polygon is closed

class Basic {
public:
    static void setPixel(int x, int y) {
        glBegin(GL_POINTS);
        glVertex2i(x, y);
        glEnd();
        glFlush();
    }

    static void drawLine(Point p1, Point p2) {
        glBegin(GL_LINES);
        glVertex2i(p1.x, p1.y);
        glVertex2i(p2.x, p2.y);
        glEnd();
        glFlush();
    }

    // Sorting points by y-coordinate
    static bool compareByY(Point p1, Point p2) {
        return p1.y < p2.y;
    }
};

class Final : public Basic {
public:
    void scanlineFill() {
        if (points.size() < 3) return;  // Minimum 3 points required to form a polygon

        // Find the bounding box of the polygon
        int ymin = points[0].y, ymax = points[0].y;
        for (const auto& point : points) {
            if (point.y < ymin) ymin = point.y;
            if (point.y > ymax) ymax = point.y;
        }

        // Start filling using scanline algorithm
        for (int y = ymin; y <= ymax; y++) {
            std::vector<int> intersectionX;  // Store intersection points for this scanline

            // Find intersections with polygon edges
            for (int i = 0; i < points.size(); i++) {
                Point p1 = points[i];
                Point p2 = points[(i + 1) % points.size()];  // Next point (wrapping around)

                if (p1.y == p2.y) continue;  // Ignore horizontal edges

                if (y >= std::min(p1.y, p2.y) && y <= std::max(p1.y, p2.y)) {
                    // Calculate intersection x-coordinate
                    int x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                    intersectionX.push_back(x);
                }
            }

            // Sort intersections
            std::sort(intersectionX.begin(), intersectionX.end());

            // Fill between pairs of intersections
            for (size_t i = 0; i < intersectionX.size(); i += 2) {
                if (i + 1 < intersectionX.size()) {
                    for (int x = intersectionX[i]; x <= intersectionX[i + 1]; x++) {
                        Basic::setPixel(x, y);  // Corrected to use Basic::setPixel
                    }
                }
            }
        }
    }
};

Final obj;

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        exit(0);
    }
}

// Mouse function to handle polygon drawing and filling
void mouseCallback(int button, int state, int x, int y) {
    y = 480 - y;  // Flip y-coordinate to match OpenGL's coordinate system

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !isPolygonComplete) {
        Point newPoint = {x, y};
        points.push_back(newPoint);

        if (points.size() > 1) {
            Basic::drawLine(points[points.size() - 2], points[points.size() - 1]);  // Corrected to use Basic::drawLine
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && !isPolygonComplete) {
        if (points.size() > 1) {
            obj.drawLine(points.back(), points[0]);  // Close the polygon
            isPolygonComplete = true;
        }
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && isPolygonComplete) {
        obj.scanlineFill();  // Fill the polygon after it's complete
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);  // Set background to white
    glColor3f(0.0, 0.0, 0.0);  // Set drawing color to black
    glPointSize(1.0);
    gluOrtho2D(0, 640, 0, 480);  // Set up coordinate system
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Scanline Polygon Filling");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouseCallback);  // Register mouse callback function
    glutMainLoop();

    return 0;
}
