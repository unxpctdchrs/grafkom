#pragma warning(disable: 4996)
#include <freeglut.h>
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glut.h"
#include "vendor/imgui/imgui_impl_opengl2.h"
#include <cmath>
#include <iostream>
#include <string>
#include <chrono>
#include <glm/glm.hpp>
#include <vector>

const float pi = 3.14f;
bool enRotX = 0;
bool enRotY = 0;
bool enRotZ = 0;
bool translateX = 0;
bool translateY = 0;
bool enAutoRotation = 0;
int stateX = 1;
int stateY = -1;
float angle = 0.0f;
float x_pos = 0.0f;
float y_pos = 0.0f;
float z_pos = -8.0f;
float skalaX;
float skalaY;
float skalaZ;
float xShear;
float yShear;
float zShear;
bool xIsMirrored = 0;
bool yIsMirrored = 0;

bool enRotX1 = 0;
bool enRotY1 = 0;
bool enRotZ1 = 0;
bool translateX1 = 0;
bool translateY1 = 0;
bool enAutoRotation1 = 0;
int stateX1 = 1;
int stateY1 = -1;
float angle1 = 0.0f;
float x_pos1 = 0.0f;
float y_pos1 = 0.0f;
float z_pos1 = -8.0f;
float skalaX1;
float skalaY1;
float skalaZ1;
float xShear1;
float yShear1;
float zShear1;
bool xIsMirrored1 = 0;
bool yIsMirrored1 = 0;

bool c1ischecked = 0;
bool c2ischecked = 0;

float cameraXpos = 0.0f;
float cameraYpos = 0.0f;
float cameraZpos = 0.0f;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;

GLuint elephant;
void loadObj(const char* fname)
{
    FILE* fp;
    int read;
    GLfloat x, y, z;
    char ch;

    elephant = glGenLists(1);
    fp = fopen(fname, "r");

    if (!fp)
    {
        printf("Can't open file %s\n", fname);
        exit(1);
    }

    glPointSize(2.0);
    glNewList(elephant, GL_COMPILE);
    {
        glBegin(GL_TRIANGLES); // Use GL_TRIANGLES for rendering triangles

        while (!(feof(fp)))
        {
            read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);

            if (read == 4 && ch == 'v')
            {
                glVertex3f(x, y, z);
            }
        }

        glEnd();
    }
    glEndList();

    fclose(fp);
}

void autoRotation() {
    angle += 0.8f;
    if (angle > 360) {
        angle = angle - 360;
    }
}

void autoRotation1() {
    angle1 += 0.8f;
    if (angle1 > 360) {
        angle1 = angle1 - 360;
    }
}

void resetTranslation() {
    x_pos = 0.0f;
    y_pos = 0.0f;
    z_pos = -8.0f;
    x_pos1 = 0.0f;
    y_pos1 = 0.0f;
    z_pos1 = -8.0f;
}

void resetRotation() {
    angle = 0.0f;
    angle1 = 0.0f;
}

void resetSkala() {
    skalaX = 0.0f;
    skalaY = 0.0f;
    skalaZ = 0.0f;
    skalaX1 = 0.0f;
    skalaY1 = 0.0f;
    skalaZ1 = 0.0f;
}

void resetShear() {
    xShear = 0.0f;
    yShear = 0.0f;
    zShear = 0.0f;
    xShear1 = 0.0f;
    yShear1 = 0.0f;
    zShear1 = 0.0f;
}

void resetAll() {
    x_pos = 0.0f;
    y_pos = 0.0f;
    z_pos = -8.0f;
    angle = 0.0f;
    skalaX = 0.0f;
    skalaY = 0.0f;
    skalaZ = 0.0f;
    xShear = 0.0f;
    yShear = 0.0f;
    zShear = 0.0f;
    xIsMirrored = 0;
    yIsMirrored = 0;

    x_pos1 = 0.0f;
    y_pos1 = 0.0f;
    z_pos1 = -8.0f;
    angle1 = 0.0f;
    skalaX1 = 0.0f;
    skalaY1 = 0.0f;
    skalaZ1 = 0.0f;
    xShear1 = 0.0f;
    yShear1 = 0.0f;
    zShear1 = 0.0f;
    xIsMirrored1 = 0;
    yIsMirrored1 = 0;
}

void pushMatrix() {
    glPushMatrix();
}

void popMatrix() {
    glPopMatrix();
}

void translasi(float x, float y, float z) {
    float matrix[16]; //4x4 matrix

    // inisialisasi matrix sebagai matrix identitas
    for (int i = 0; i < 16; i++) {
        matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    // terapkan translasi
    matrix[12] += x;
    matrix[13] += y;
    matrix[14] += z;

    glMultMatrixf(matrix);
}

void skala(float x, float y, float z) {
    float matrix[16]; //4x4 matrix

    // inisialisasi matrix sebagai matrix identitas
    for (int i = 0; i < 16; i++) {
        matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    // terapkan translasi
    matrix[0] += x;
    matrix[5] += y;
    matrix[10] += z;

    glMultMatrixf(matrix);
}

void rotasiX(float sudut, float x, float y, float z) {
    float matrix[16]; //4x4 matrix

    // inisialisasi matrix sebagai matrix identitas
    for (int i = 0; i < 16; i++) {
        matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    // Convert degrees to radians
    float cosTheta = cos(sudut * pi / 180.0f);
    float sinTheta = sin(sudut * pi / 180.0f);

    /*matrix[5] = y * cosTheta;
    matrix[6] = y * sinTheta;
    matrix[9] = -(z * sinTheta);
    matrix[10] = (z * cosTheta);*/

    matrix[5] = cosTheta;
    matrix[6] = -sinTheta;
    matrix[9] = sinTheta;
    matrix[10] = cosTheta;


    glMultMatrixf(matrix);
}

void rotasiY(float sudut, float x, float y, float z) {
    float matrix[16]; //4x4 matrix

    // inisialisasi matrix sebagai matrix identitas
    for (int i = 0; i < 16; i++) {
        matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    // Convert degrees to radians
    float cosTheta = cos(sudut * pi / 180.0f);
    float sinTheta = sin(sudut * pi / 180.0f);

    /*matrix[5] = y * cosTheta;
    matrix[6] = y * sinTheta;
    matrix[9] = -(z * sinTheta);
    matrix[10] = (z * cosTheta);*/

    matrix[0] = cosTheta;
    matrix[2] = -sinTheta;
    matrix[8] = sinTheta;
    matrix[10] = cosTheta;


    glMultMatrixf(matrix);
}

void rotasiZ(float sudut, float x, float y, float z) {
    float matrix[16]; //4x4 matrix

    // inisialisasi matrix sebagai matrix identitas
    for (int i = 0; i < 16; i++) {
        matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    // Convert degrees to radians
    float cosTheta = cos(sudut * pi / 180.0f);
    float sinTheta = sin(sudut * pi / 180.0f);

    /*matrix[5] = y * cosTheta;
    matrix[6] = y * sinTheta;
    matrix[9] = -(z * sinTheta);
    matrix[10] = (z * cosTheta);*/

    matrix[0] = cosTheta;
    matrix[1] = sinTheta;
    matrix[4] = -sinTheta;
    matrix[5] = cosTheta;


    glMultMatrixf(matrix);
}

void shearX(float shx, float y, float z) {
    float matrix[16]; //4x4 matrix

    // inisialisasi matrix sebagai matrix identitas
    for (int i = 0; i < 16; i++) {
        matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    matrix[4] = shx;

    glMultMatrixf(matrix);
}

void shearY(float x, float shy, float z) {
    float matrix[16]; //4x4 matrix

    // inisialisasi matrix sebagai matrix identitas
    for (int i = 0; i < 16; i++) {
        matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    matrix[1] = shy;

    glMultMatrixf(matrix);
}

void shearZ(float x, float y, float shz) {
    float matrix[16]; //4x4 matrix

    // inisialisasi matrix sebagai matrix identitas
    for (int i = 0; i < 16; i++) {
        matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    matrix[2] = shz;

    glMultMatrixf(matrix);
}

void mirrorX() {
    float reflectionMatrix[16] = {
        1.0f,  0.0f,  0.0f,  0.0f,
        0.0f, -1.0f,  0.0f,  0.0f,
        0.0f,  0.0f,  1.0f,  0.0f,
        0.0f,  0.0f,  0.0f,  1.0f
    };

    // Apply the reflection matrix to mirror the cube.
    glMultMatrixf(reflectionMatrix);
}

void mirrorY() {
    float reflectionMatrix[16] = {
       -1.0f,  0.0f,  0.0f,  0.0f,
        0.0f,  1.0f,  0.0f,  0.0f,
        0.0f,  0.0f,  1.0f,  0.0f,
        0.0f,  0.0f,  0.0f,  1.0f
    };

    // Apply the reflection matrix to mirror the cube.
    glMultMatrixf(reflectionMatrix);
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}


void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);

    if (enAutoRotation) {
        autoRotation();
    }
    if (enAutoRotation1) {
        autoRotation1();
    }

    if (translateX == 1) {
        switch (stateX) {
        case 1:
            if (x_pos < 2) {
                x_pos += 0.05;
            }
            else {
                stateX = -1;
            }
            break;
        case -1:
            if (x_pos > -2) {
                x_pos -= 0.05;
            }
            else stateX = 1;
        }
    }

    if (translateY == 1) {
        switch (stateY) {
        case 1:
            if (y_pos < 2) {
                y_pos += 0.05;
            }
            else {
                stateY = -1;
            }
            break;
        case -1:
            if (y_pos > -2) {
                y_pos -= 0.05;
            }
            else stateY = 1;
        }
    }


    if (translateX1 == 1) {
        switch (stateX1) {
        case 1:
            if (x_pos1 < 2) {
                x_pos1 += 0.05;
            }
            else {
                stateX1 = -1;
            }
            break;
        case -1:
            if (x_pos1 > -2) {
                x_pos1 -= 0.05;
            }
            else stateX1 = 1;
        }
    }

    if (translateY1 == 1) {
        switch (stateY1) {
        case 1:
            if (y_pos1 < 2) {
                y_pos1 += 0.05;
            }
            else {
                stateY1 = -1;
            }
            break;
        case -1:
            if (y_pos1 > -2) {
                y_pos1 -= 0.05;
            }
            else stateY1 = 1;
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case'w':
        cameraZpos += 0.2f;
        break;
    case'a':
        cameraXpos += 0.2f;
        break;
    case's':
        cameraZpos -= 0.2f;
        break;
    case'd':
        cameraXpos -= 0.2f;
        break;
    case'4':
        cameraAngleY -= 0.8f;
        break;
    case'6':
        cameraAngleY += 0.8f;
        break;
    case'8':
        cameraAngleX -= 0.8f;
        break;
    case'5':
        cameraAngleX += 0.8f;
        break;
    }
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        cameraYpos -= 0.2f;
        break;
    case GLUT_KEY_DOWN:
        cameraYpos += 0.2f;
    }
}

void cube() {
    glBegin(GL_QUADS);
    //front
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-1.0,  1.0, 1.0);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f( 1.0, -1.0, 1.0);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f( 1.0, 1.0,  1.0);
    //back
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f( 1.0,  1.0, -1.0);
    glVertex3f( 1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0,  1.0, -1.0);
    //right
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(1.0,  1.0,  1.0);
    glVertex3f(1.0, -1.0,  1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0,  1.0, -1.0);
    //left
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-1.0,  1.0, -1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0,  1.0);
    glVertex3f(-1.0,  1.0,  1.0);
    //top
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f(-1.0, 1.0,  1.0);
    glVertex3f( 1.0, 1.0,  1.0);
    glVertex3f( 1.0, 1.0, -1.0);
    //bottom
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0,  1.0);
    glVertex3f( 1.0, -1.0,  1.0);
    glVertex3f( 1.0, -1.0, -1.0);

    glEnd();
}

void plane() {
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);  
    glVertex3f(-1.0, 0.0, -1.0);
    glVertex3f(-1.0, 0.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 1.0); 
    glVertex3f(1.0, 0.0, -1.0);
    glEnd();
}

void sponza() {
    loadObj("sponza.obj");
}

// Forward declarations of helper functions
void MainLoopStep();

static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

int main(int argc, char** argv)
{
    // Create GLUT window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    //glutInitWindowPosition(550, 100);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("3D Transformation");

    glutDisplayFunc(MainLoopStep);
    glutTimerFunc(0, timer, 0);
    init();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();

    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL2_Init();
    ImGui_ImplGLUT_InstallFuncs();
    
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return 0;
}

void MainLoopStep()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();

    //Imgui Window
    ImGui::SetNextWindowSize(ImVec2(350, 200));
    if (ImGui::Begin("Cube Controller"), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize) {
        ImGui::Checkbox("Cube 1", &c1ischecked);
        ImGui::Checkbox("Cube 2", &c2ischecked);
        
        if(c1ischecked)
        {
            ImGui::SetNextWindowSize(ImVec2(350, 600));
            if (ImGui::Begin("Cube 1"))
            {
                ImGui::Text("Sumbu");
                ImGui::Checkbox("X Axis", &enRotX);
                ImGui::Checkbox("Y Axis", &enRotY);
                ImGui::Checkbox("Z Axis", &enRotZ);

                //translasi
                ImGui::Text("Translasi");
                ImGui::Checkbox("TranslasiX", &translateX);
                ImGui::Checkbox("TranslasiY", &translateY);
                ImGui::SliderFloat("Translasi X", &x_pos, -2.0f, 2.0f);
                ImGui::SliderFloat("Translasi Y", &y_pos, -2.0f, 2.0f);
                ImGui::SliderFloat("Translasi Z", &z_pos, -5.0f, -20.0f);

                //rotasi
                ImGui::Text("Rotasi");
                ImGui::SliderFloat("Sudut (DEG)", &angle, 0.0f, 360.0f);
                ImGui::Checkbox("Auto Rotation", &enAutoRotation);

                //scale
                ImGui::Text("Skala");
                ImGui::SliderFloat("Skala X", &skalaX, 0.0f, 1.5f);
                ImGui::SliderFloat("Skala Y", &skalaY, 0.0f, 1.5f);
                ImGui::SliderFloat("Skala Z", &skalaZ, 0.0f, 1.5f);

                //shear
                ImGui::Text("Shear");
                ImGui::SliderFloat("Shear X", &xShear, -1.5f, 1.5f);
                ImGui::SliderFloat("Shear Y", &yShear, -1.5f, 1.5f);
                ImGui::SliderFloat("Shear Z", &zShear, -1.5f, 1.5f);

                //Mirror
                ImGui::Text("Mirror");
                ImGui::Checkbox("Mirror X", &xIsMirrored);
                ImGui::Checkbox("Mirror Y", &yIsMirrored);
            }ImGui::End();
        }

        if (c2ischecked)
        {
            ImGui::SetNextWindowSize(ImVec2(350, 600));
            if (ImGui::Begin("Cube 2"))
            {
                ImGui::Text("Sumbu");
                ImGui::Checkbox("X Axis", &enRotX1);
                ImGui::Checkbox("Y Axis", &enRotY1);
                ImGui::Checkbox("Z Axis", &enRotZ1);

                //translasi
                ImGui::Text("Translasi");
                ImGui::Checkbox("TranslasiX", &translateX1);
                ImGui::Checkbox("TranslasiY", &translateY1);
                ImGui::SliderFloat("Translasi X", &x_pos1, -2.0f, 2.0f);
                ImGui::SliderFloat("Translasi Y", &y_pos1, -2.0f, 2.0f);
                ImGui::SliderFloat("Translasi Z", &z_pos1, -5.0f, -20.0f);

                //rotasi
                ImGui::Text("Rotasi");
                ImGui::SliderFloat("Sudut (DEG)", &angle1, 0.0f, 360.0f);
                ImGui::Checkbox("Auto Rotation", &enAutoRotation1);

                //scale
                ImGui::Text("Skala");
                ImGui::SliderFloat("Skala X", &skalaX1, 0.0f, 1.5f);
                ImGui::SliderFloat("Skala Y", &skalaY1, 0.0f, 1.5f);
                ImGui::SliderFloat("Skala Z", &skalaZ1, 0.0f, 1.5f);

                //shear
                ImGui::Text("Shear");
                ImGui::SliderFloat("Shear X", &xShear1, -1.5f, 1.5f);
                ImGui::SliderFloat("Shear Y", &yShear1, -1.5f, 1.5f);
                ImGui::SliderFloat("Shear Z", &zShear1, -1.5f, 1.5f);

                //Mirror
                ImGui::Text("Mirror");
                ImGui::Checkbox("Mirror X", &xIsMirrored1);
                ImGui::Checkbox("Mirror Y", &yIsMirrored1);
            }ImGui::End();
        }
        

        //reset btn
        if (ImGui::Button("Reset Translasi")) {
            resetTranslation();
        }
        if (ImGui::Button("Reset Rotasi")) {
            resetRotation();
        }
        if (ImGui::Button("Reset Skala")) {
            resetSkala();
        }
        if (ImGui::Button("Reset Shear")) {
            resetShear();
        }
        if (ImGui::Button("Reset All")) {
            resetAll();
        }
    }ImGui::End();

    // Rendering
    ImGui::Render();

    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 3D Transformation    

    //global transformation
    translasi(cameraXpos, cameraYpos, cameraZpos);
    rotasiX(cameraAngleX, 1.0f, 0.0f, 0.0f);
    rotasiY(cameraAngleY, 0.0f, 1.0f, 0.0f);


    //cube 1
    pushMatrix();
        //translasi
        translasi(x_pos - 5, y_pos, z_pos);

        //rotasi
        if (enRotX) {
            rotasiX(angle, enRotX, 0.0f, 0.0f);
        }
        if (enRotY) {
            rotasiY(angle, 0.0f, enRotY, 0.0f);
        }
        if (enRotZ) {
            rotasiZ(angle, 0.0f, 0.0f, enRotZ);
        }

        //skala
        skala(skalaX, skalaY, skalaZ);

        //shear
        shearX(xShear, 0, 0);
        shearY(0.0f, yShear, 0.0f);
        shearZ(0.0f, 0.0f, zShear);

        //mirror
        if (xIsMirrored) {
            mirrorX();
        }
        if (yIsMirrored) {
            mirrorY();
        }
        cube();
    popMatrix();

    //cube 2
    pushMatrix();
        //translasi
        translasi(x_pos1 + 5, y_pos1, z_pos1);

        //rotasi
        if (enRotX1) {
            rotasiX(angle1, enRotX1, 0.0f, 0.0f);
        }
        if (enRotY1) {
            rotasiY(angle1, 0.0f, enRotY1, 0.0f);
        }
        if (enRotZ1) {
            rotasiZ(angle1, 0.0f, 0.0f, enRotZ1);
        }

        //skala
        skala(skalaX1, skalaY1, skalaZ1);

        //shear
        shearX(xShear1, 0, 0);
        shearY(0.0f, yShear1, 0.0f);
        shearZ(0.0f, 0.0f, zShear1);

        //mirror
        if (xIsMirrored1) {
            mirrorX();
        }
        if (yIsMirrored1) {
            mirrorY();
        }
        cube();
    popMatrix();

    pushMatrix();
        skala(50.0f, 0.0f, 50.0f);
        translasi(0.0f, -5.0f, 0.0f);
        plane();
    popMatrix();

    //sponza();

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
}