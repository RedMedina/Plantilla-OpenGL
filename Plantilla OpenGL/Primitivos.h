#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Cube
{
public:
    Cube(float size = 1.0f) : Size(size) {}

    void Draw()
    {
        glBegin(GL_QUADS);
        // front
        glVertex3f(-Size / 2, -Size / 2, Size / 2);
        glVertex3f(Size / 2, -Size / 2, Size / 2);
        glVertex3f(Size / 2, Size / 2, Size / 2);
        glVertex3f(-Size / 2, Size / 2, Size / 2);
        // back
        glVertex3f(-Size / 2, -Size / 2, -Size / 2);
        glVertex3f(-Size / 2, Size / 2, -Size / 2);
        glVertex3f(Size / 2, Size / 2, -Size / 2);
        glVertex3f(Size / 2, -Size / 2, -Size / 2);
        // left
        glVertex3f(-Size / 2, -Size / 2, -Size / 2);
        glVertex3f(-Size / 2, -Size / 2, Size / 2);
        glVertex3f(-Size / 2, Size / 2, Size / 2);
        glVertex3f(-Size / 2, Size / 2, -Size / 2);
        // right
        glVertex3f(Size / 2, -Size / 2, -Size / 2);
        glVertex3f(Size / 2, Size / 2, -Size / 2);
        glVertex3f(Size / 2, Size / 2, Size / 2);
        glVertex3f(Size / 2, -Size / 2, Size / 2);
        // top
        glVertex3f(-Size / 2, Size / 2, -Size / 2);
        glVertex3f(-Size / 2, Size / 2, Size / 2);
        glVertex3f(Size / 2, Size / 2, Size / 2);
        glVertex3f(Size / 2, Size / 2, -Size / 2);
        // bottom
        glVertex3f(-Size / 2, -Size / 2, -Size / 2);
        glVertex3f(Size / 2, -Size / 2, -Size / 2);
        glVertex3f(Size / 2, -Size / 2, Size / 2);
        glVertex3f(-Size / 2, -Size / 2, Size / 2);
        glEnd();
    }

private:
    float Size;
};
