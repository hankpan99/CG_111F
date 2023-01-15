#include<stdlib.h>
#include<stdio.h>
#include<GL/glut.h>
#include<algorithm>

static int thumb[3] = {0, 0, -50}, fore[3] = {0, 0, -7}, middle[3] = {0, 0, 0}, ring[3] = {0, 0, 7}, pinky[3] = {0, 0, 10};
const float rgb_value[2][3] = { {255.0 / 255.0, 204.0 / 255.0, 153.0 / 255.0},
                                {255.0 / 255.0, 229.0 / 255.0, 204.0 / 255.0}};

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);

    // palm
    glColor3f(rgb_value[1][0], rgb_value[1][1], rgb_value[1][2]);
    glBegin(GL_POLYGON);
        glVertex3f(-0.92, 0.0, 0.0);
        glVertex3f(0.52, 0.0, 0.0);
        glVertex3f(0.52, -1.5, 0.0);
        glVertex3f(-0.8, -1.5, 0.0);
    glEnd();

    // thumb
    glPushMatrix();
    glTranslatef(0.5, -1.0, 0.0);
    glRotatef((GLfloat) thumb[2], 0.0, 0.0, 1.0);
    glRotatef((GLfloat) thumb[1], 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.3, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.6, 0.2);
    glColor3f(rgb_value[1][0], rgb_value[1][1], rgb_value[1][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, 0.3, 0.0);
    glRotatef((GLfloat) thumb[0], 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.3, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.6, 0.2);
    glColor3f(rgb_value[0][0], rgb_value[0][1], rgb_value[0][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
    
    // fore finger
    glPushMatrix();
    glTranslatef(0.4, 0.0, 0.0);
    glRotatef((GLfloat) fore[2], 0.0, 0.0, 1.0);
    glRotatef((GLfloat) fore[1], 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.45, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.9, 0.2);
    glColor3f(rgb_value[1][0], rgb_value[1][1], rgb_value[1][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, 0.45, 0.0);
    glRotatef((GLfloat) fore[0], 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.45, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.9, 0.2);
    glColor3f(rgb_value[0][0], rgb_value[0][1], rgb_value[0][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // middle finger
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef((GLfloat) middle[2], 0.0, 0.0, 1.0);
    glRotatef((GLfloat) middle[1], 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.5, 0.0);
    glPushMatrix();
    glScalef(0.2, 1.0, 0.2);
    glColor3f(rgb_value[1][0], rgb_value[1][1], rgb_value[1][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, 0.5, 0.0);
    glRotatef((GLfloat) middle[0], 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.5, 0.0);
    glPushMatrix();
    glScalef(0.2, 1.0, 0.2);
    glColor3f(rgb_value[0][0], rgb_value[0][1], rgb_value[0][2]);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPopMatrix();

    // ring finger
    glPushMatrix();
    glTranslatef(-0.4, 0.0, 0.0);
    glRotatef((GLfloat) ring[2], 0.0, 0.0, 1.0);
    glRotatef((GLfloat) ring[1], 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.4, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.8, 0.2);
    glColor3f(rgb_value[1][0], rgb_value[1][1], rgb_value[1][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, 0.4, 0.0);
    glRotatef((GLfloat) ring[0], 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.4, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.8, 0.2);
    glColor3f(rgb_value[0][0], rgb_value[0][1], rgb_value[0][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();

    // ring finger
    glPushMatrix();
    glTranslatef(-0.8, 0.0, 0.0);
    glRotatef((GLfloat) pinky[2], 0.0, 0.0, 1.0);
    glRotatef((GLfloat) pinky[1], 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.3, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.6, 0.2);
    glColor3f(rgb_value[1][0], rgb_value[1][1], rgb_value[1][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(0.0, 0.3, 0.0);
    glRotatef((GLfloat) pinky[0], 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.3, 0.0);
    glPushMatrix();
    glScalef(0.2, 0.6, 0.2);
    glColor3f(rgb_value[0][0], rgb_value[0][1], rgb_value[0][2]);
    glutSolidCube(1.0);
    glPopMatrix();

    glPopMatrix();
    
    glutSwapBuffers();
}

void reshape(int w, int h){
    glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

void processNormalKeys(unsigned char key, int x, int y){
    switch(key){
        // thumb
        case 't':
            thumb[0] = std::min(thumb[0] + 5, 90) % 360;
            glutPostRedisplay();
            break;

        case 'T':
            thumb[0] = std::max(thumb[0] - 5, 0) % 360;
            glutPostRedisplay();
            break;

        case 'g':
            thumb[1] = std::min(thumb[1] + 5, 90) % 360;
            glutPostRedisplay();
            break;

        case 'G':
            thumb[1] = std::max(thumb[1] - 5, 0) % 360;
            glutPostRedisplay();
            break;
        
        case 'b':
            thumb[2] = std::min(thumb[2] + 5, 0) % 360;
            glutPostRedisplay();
            break;

        case 'B':
            thumb[2] = std::max(thumb[2] - 5, -45) % 360;
            glutPostRedisplay();
            break;

        // fore finger
        case 'r':
            fore[0] = std::min(fore[0] + 5, 90) % 360;
            glutPostRedisplay();
            break;

        case 'R':
            fore[0] = std::max(fore[0] - 5, 0) % 360;
            glutPostRedisplay();
            break;

        case 'f':
            fore[1] = std::min(fore[1] + 5, 90) % 360;
            glutPostRedisplay();
            break;

        case 'F':
            fore[1] = std::max(fore[1] - 5, 0) % 360;
            glutPostRedisplay();
            break;

        case 'v':
            fore[2] = std::min(fore[2] + 5, 15) % 360;
            glutPostRedisplay();
            break;

        case 'V':
            fore[2] = std::max(fore[2] - 5, -15) % 360;
            glutPostRedisplay();
            break;

        // middle finger
        case 'e':
            middle[0] = std::min(middle[0] + 5, 90) % 360;
            glutPostRedisplay();
            break;

        case 'E':
            middle[0] = std::max(middle[0] - 5, 0) % 360;
            glutPostRedisplay();
            break;

        case 'd':
            middle[1] = std::min(middle[1] + 5, 90) % 360;
            glutPostRedisplay();
            break;

        case 'D':
            middle[1] = std::max(middle[1] - 5, 0) % 360;
            glutPostRedisplay();
            break;

        case 'c':
            middle[2] = std::min(middle[2] + 5, 15) % 360;
            glutPostRedisplay();
            break;

        case 'C':
            middle[2] = std::max(middle[2] - 5, -15) % 360;
            glutPostRedisplay();
            break;

        // ring finger
        case 'w':
            ring[0] = std::min(ring[0] + 5, 90) % 360;
            glutPostRedisplay();
            break;

        case 'W':
            ring[0] = std::max(ring[0] - 5, 0) % 360;
            glutPostRedisplay();
            break;

        case 's':
            ring[1] = std::min(ring[1] + 5, 90) % 360;
            glutPostRedisplay();
            break;

        case 'S':
            ring[1] = std::max(ring[1] - 5, 0) % 360;
            glutPostRedisplay();
            break;

        case 'x':
            ring[2] = std::min(ring[2] + 5, 15) % 360;
            glutPostRedisplay();
            break;

        case 'X':
            ring[2] = std::max(ring[2] - 5, -15) % 360;
            glutPostRedisplay();
            break;

        // pinky
        case 'q':
            pinky[0] = std::min(pinky[0] + 5, 90) % 360;
            glutPostRedisplay();
            break;

        case 'Q':
            pinky[0] = std::max(pinky[0] - 5, 0) % 360;
            glutPostRedisplay();
            break;

        case 'a':
            pinky[1] = std::min(pinky[1] + 5, 90) % 360;
            glutPostRedisplay();
            break;

        case 'A':
            pinky[1] = std::max(pinky[1] - 5, 0) % 360;
            glutPostRedisplay();
            break;

        case 'z':
            pinky[2] = std::min(pinky[2] + 5, 15) % 360;
            glutPostRedisplay();
            break;

        case 'Z':
            pinky[2] = std::max(pinky[2] - 5, -15) % 360;
            glutPostRedisplay();
            break;
        
        // exit
        case 27:
            exit(0);
            break;

        default:
            break;
    }
}

int main(int argc, char** argv){

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition (100, 100);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Dexterous Hand");

    // register callbacks 
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    // keyboard input
    glutKeyboardUpFunc(processNormalKeys);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}