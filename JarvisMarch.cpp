#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glut.h>

#define FLOAT_MIN 0.000001

class Point2D
{
    float x, y;

public:
    Point2D() : x(0), y(0) {}
    Point2D(float px, float py) : x(px), y(py) {}
    float Get_x() const { return x; }
    float Get_y() const { return y; }
    void Mark() const;
    void DrawText() const;
    void DrawLine(Point2D p) const;

    bool operator==(const Point2D &other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point2D &other) const
    {
        return !(*this == other);
    }
};

void Point2D::Mark() const
{
    glPointSize(10.0);
    glBegin(GL_POINTS);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex2f(x, y);
    glEnd();
}

void Point2D::DrawText() const
{
    glColor3f(1.0f, 1.0f, 1.0f);
    // glRasterPos2f(x + 5, y + 5);
    std::string text = "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    for (const char &c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void Point2D::DrawLine(Point2D p) const
{
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(x, y);
    glVertex2f(p.x, p.y);
    glEnd();
}

std::vector<Point2D> points;

void init()
{
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1920, 0, 1080);
}

void drawPoints()
{
    for (const auto &point : points)
    {
        point.Mark();
        point.DrawText();
    }
}

int orientation(Point2D p, Point2D q, Point2D r)
{
    float val = (q.Get_y() - p.Get_y()) * (r.Get_x() - q.Get_x()) -
                (q.Get_x() - p.Get_x()) * (r.Get_y() - q.Get_y());

    if (val == 0)
        return 0;
    return (val > 0) ? 1 : 2;
}

void drawConvexHull()
{
    if (points.size() < 3)
    {
        return;
    }

    Point2D start = points[0];
    for (const auto &point : points)
    {
        if (point.Get_x() > start.Get_x())
        {
            start = point;
        }
    }

    Point2D current = start;
    Point2D next;

    do
    {
        next = points[0];

        for (const auto &point : points)
        {
            if (next == current || orientation(current, next, point) == 2)
            {
                next = point;
            }
        }

        current.DrawLine(next);
        current = next;
    } while (current != start);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawPoints();
    drawConvexHull();
    glutSwapBuffers();
}

void MouseClick(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
        int oglY = windowHeight - y;
        points.emplace_back(x, oglY);
        glutPostRedisplay();
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Convex Hull - Jarvis March");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(MouseClick);
    glutMainLoop();
    return 0;
}
