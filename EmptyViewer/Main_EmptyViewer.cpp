/* //Q1
#include <GL/glew.h>    
#include <GL/glut.h>
#include <cmath>
#include "load_mesh.hpp"
#include "frame_timer.hpp"

void init_lighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambient_light[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // original value
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);

    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    GLfloat ka[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ks[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
}

void set_projection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);
    glViewport(0, 0, 1280, 1280);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ✅ 조명 방향 (정면에서 비추는 방향으로 테스트)
    GLfloat light_dir[] = { 0.0f, 0.0f, 1.0f, 0.0f };  // brighter test direction
    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

    start_timing();

    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    glBegin(GL_TRIANGLES);
    for (size_t i = 0; i < gTriangles.size(); i++) {
        for (int j = 0; j < 3; j++) {
            unsigned int idx = gTriangles[i].indices[j];
            Vector3 n = gNormals[idx];
            Vector3 v = gPositions[idx];
            glNormal3f(n.x, n.y, n.z);
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();

    float timeElapsed = stop_timing();
    gTotalFrames++;
    gTotalTimeElapsed += timeElapsed;
    float fps = gTotalFrames / gTotalTimeElapsed;
    char title[128];
    sprintf(title, "Q1: Immediate Mode - %.2f FPS", fps);
    glutSetWindowTitle(title);

    glutSwapBuffers();
    glutPostRedisplay();
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glClearColor(0, 0, 0, 1);
    init_timer();
    init_lighting();
    set_projection();
    load_mesh("bunny.obj");  // 실행 디렉토리에 bunny.obj 있어야 함
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("HW8 Bunny - Q1 Immediate Mode");
    glewInit();

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
*/

//Q2
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include "load_mesh.hpp"
#include "frame_timer.hpp"

GLuint vao = 0;
GLuint vbo_positions = 0;
GLuint vbo_normals = 0;
GLuint ebo = 0;

void init_lighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambient_light[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);

    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    GLfloat ka[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ks[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
}

void set_projection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);
    glViewport(0, 0, 1280, 1280);
}

void setup_vertex_array()
{
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<unsigned int> indices;

    unsigned int index = 0;
    for (const auto& tri : gTriangles) {
        for (int j = 0; j < 3; ++j) {
            unsigned int idx = tri.indices[j];
            Vector3 v = gPositions[idx];
            Vector3 n = gNormals[idx];

            positions.push_back(v.x);
            positions.push_back(v.y);
            positions.push_back(v.z);

            normals.push_back(n.x);
            normals.push_back(n.y);
            normals.push_back(n.z);

            indices.push_back(index++); // ✅ 순차 인덱스
        }
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo_positions);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_positions);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, 0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat d = sqrtf(3.0f);
    GLfloat light_dir[] = { -1.0f / d, -1.0f / d, -1.0f / d, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);

    start_timing();

    glTranslatef(0.1f, -1.0f, -3.0f);
    glScalef(8.0f, 8.0f, 8.0f);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_positions);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glNormalPointer(GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, gTriangles.size() * 3, GL_UNSIGNED_INT, 0);

    float timeElapsed = stop_timing();
    gTotalFrames++;
    gTotalTimeElapsed += timeElapsed;
    float fps = gTotalFrames / gTotalTimeElapsed;
    char title[128];
    sprintf(title, "Q2: Vertex Array - %.2f FPS", fps);
    glutSetWindowTitle(title);

    glutSwapBuffers();
    glutPostRedisplay();
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glClearColor(0, 0, 0, 1);
    init_timer();
    init_lighting();
    set_projection();
    load_mesh("bunny.obj");
    setup_vertex_array();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("HW8 Bunny - Q2 Vertex Array");
    glewInit();

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}