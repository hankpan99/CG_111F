#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<GL/glut.h>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
using namespace std;

#define KEY_UP 101
#define KEY_DOWN 103

struct Vertex{
    float x, y, z;
};

struct Face{
    int v1, v2, v3;
    float nx, ny, nz;
};

// obj file data
vector<Vertex> vertices;
vector<Face> faces;
float max_x = 0, max_y = 0, max_z = 0, min_x = 0, min_y = 0, min_z = 0;

// windows size
int winWidth = 1920, winHeight = 1080;

// trackball parameters initialization
float angle = 0.0, axis[3], trans[3];
float lastPos[3] = {0.0, 0.0, 0.0};

bool trackingMouse = false;
bool redrawContinue = false;
bool trackballMove = false;

// zoom for scaling
float zoom = 1.0f;

void init(int winWidth, int winHeight){
    // initialize
    glViewport(0, 0, winWidth, winHeight);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);

    // set modelview matrix
    glMatrixMode(GL_MODELVIEW);                                
	glLoadIdentity();
	gluLookAt(	0.0f, -1.5f, 0.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f);

    // set shading parameters
    float frontColor[] = {51.0 / 255.0, 153.0 / 255.0, 255.0 / 255.0, 1.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, frontColor);
    glMaterialf(GL_FRONT, GL_SHININESS, 100);

    float light0_pos[] = {0.0, -2.0, 0.0, 0.0};
    float ambient0[] = {0.1, 0.1, 0.1, 1.0};
    float diffuse0[] = {0.9, 0.9, 0.9, 1.0};
    float specular0[] = {1.0, 1.0, 1.0, 1.0};
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
}

void reshape(int w, int h){
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
    winWidth = w;
    winHeight = h;

    // Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

    // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, w * 1.0 / h, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void read_obj_file(const char* filename){
    ifstream inFile;
    inFile.open(filename);

    string line;
    while(getline(inFile, line, '\n')){
        if(line.substr(0, 2) == "v "){
            stringstream ss(line.substr(2));
            Vertex tmp_vertex; ss >> tmp_vertex.x; ss >> tmp_vertex.y; ss >> tmp_vertex.z;
            vertices.push_back(tmp_vertex);

            max_x = max(max_x, tmp_vertex.x); min_x = min(min_x, tmp_vertex.x);
            max_y = max(max_y, tmp_vertex.y); min_y = min(min_y, tmp_vertex.y);
            max_z = max(max_z, tmp_vertex.z); min_z = min(min_z, tmp_vertex.z);
        }
        else if(line.substr(0, 2) == "f "){
            stringstream ss(line.substr(2));
            Face tmp_face; ss >> tmp_face.v1; ss >> tmp_face.v2; ss >> tmp_face.v3;
            tmp_face.v1--, tmp_face.v2--, tmp_face.v3--;
            faces.push_back(tmp_face);
        }
    }
}

void load_obj(const char* filename){
    read_obj_file(filename);

    // compute face normal
    for(int i = 0; i < faces.size(); i++){
        float v1[] = {  vertices[faces[i].v2].x - vertices[faces[i].v1].x, 
                        vertices[faces[i].v2].y - vertices[faces[i].v1].y, 
                        vertices[faces[i].v2].z - vertices[faces[i].v1].z};
        float v2[] = {  vertices[faces[i].v3].x - vertices[faces[i].v1].x, 
                        vertices[faces[i].v3].y - vertices[faces[i].v1].y, 
                        vertices[faces[i].v3].z - vertices[faces[i].v1].z};

        faces[i].nx = v2[2] * v1[1] - v2[1] * v1[2];
        faces[i].ny = v2[0] * v1[2] - v2[2] * v1[0];
        faces[i].nz = v2[1] * v1[0] - v2[0] * v1[1];

        float vec_len = sqrt(faces[i].nx * faces[i].nx + faces[i].ny * faces[i].ny + faces[i].nz * faces[i].nz);

        faces[i].nx /= vec_len;
        faces[i].ny /= vec_len;
        faces[i].nz /= vec_len;
    }

    // normalize to unit size
    float shift[] = {(max_x + min_x) / 2, (max_y + min_y) / 2, (max_z + min_z) / 2};
    float scale = max(max((max_x - min_x), (max_y - min_y)), (max_z - min_z));

    for(int i = 0; i < vertices.size(); i++){
        vertices[i].x = (vertices[i].x - shift[0]) / scale;
        vertices[i].y = (vertices[i].y - shift[1]) / scale;
        vertices[i].z = (vertices[i].z - shift[2]) / scale;
    }
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // angle < 10: reject large angle changes, which might cause by a new clicking position
    if(trackballMove && angle < 10){
        glRotatef(angle, axis[0], axis[2], axis[1]);
    }

    for(int i = 0; i < faces.size(); i++){
        glBegin(GL_POLYGON);
            glNormal3f(faces[i].nx, faces[i].ny, faces[i].nz);
            glVertex3f(vertices[faces[i].v1].x, vertices[faces[i].v1].y, vertices[faces[i].v1].z);
            glVertex3f(vertices[faces[i].v2].x, vertices[faces[i].v2].y, vertices[faces[i].v2].z);
            glVertex3f(vertices[faces[i].v3].x, vertices[faces[i].v3].y, vertices[faces[i].v3].z);
        glEnd();
    }

    glFlush();
    glutSwapBuffers();
}

void pixel2worldcoor(int x, int y, int winWidth, int winHeight, float pos3d[]){
    // scale to range (-0.5, 0.5)
    pos3d[0] = ((float) x / (float) winWidth) - 0.5;
    pos3d[1] = ((float) (winHeight - y) / (float) winHeight) - 0.5;
    pos3d[2] = 1 - (pos3d[0] * pos3d[0] + pos3d[1] * pos3d[1]);
}

void mouseMotion(int x, int y){
    float curPos[3], dx, dy, dz;
    
    pixel2worldcoor(x, y, winWidth, winHeight, curPos);
    
    if(trackingMouse){
        dx = curPos[0] - lastPos[0];
        dy = curPos[1] - lastPos[1];
        dz = curPos[2] - lastPos[2];
        if(dx || dy || dz){
            angle = 360.0 * sqrt(dx*dx + dy*dy + dz*dz);
            axis[0] = curPos[2] * lastPos[1] - curPos[1] * lastPos[2];
            axis[1] = curPos[0] * lastPos[2] - curPos[2] * lastPos[0];
            axis[2] = curPos[1] * lastPos[0] - curPos[0] * lastPos[1];
        }
        lastPos[0] = curPos[0];
        lastPos[1] = curPos[1];
        lastPos[2] = curPos[2];
    }
    
    glutPostRedisplay();
}

void startMotion(int x, int y){
    pixel2worldcoor(x, y, winWidth, winHeight, lastPos);

    trackingMouse = true;
    redrawContinue = true;
    trackballMove = true;
}

void stopMotion(int x, int y){
    trackingMouse = false;
    redrawContinue = false;
    trackballMove = false;

    angle = 0.0;
    memset(lastPos, 0.0, sizeof(lastPos));
}

void mouseButton(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON){
        switch(state){
            case GLUT_DOWN:
                y = winHeight - y;
                startMotion(x, y);
                break;
            case GLUT_UP:
                x = min(x, winWidth), y = min(x, winHeight);
                stopMotion(x, y);
                break;
        }
    }
}

void idle(){
    if(redrawContinue)
        glutPostRedisplay();
}

void Specialkey(int key, int x, int y){
    switch(key){
        // zoom in
        case KEY_UP:
            glScalef(1.10f * zoom, 1.10f * zoom, 1.10f * zoom);
            glutPostRedisplay();
            break;
        
        // zoom out
        case KEY_DOWN:
            glScalef(0.90f * zoom, 0.90f * zoom, 0.90f * zoom);
            glutPostRedisplay();
            break;

        default:
            break;
    }
}

int main(int argc, char** argv){
    
    // load obj file
    load_obj("./Dino.obj");

    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Dinosaur");

    init(winWidth, winHeight);

    // register callbacks 
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    
    glutSpecialFunc(Specialkey);
    glutMotionFunc(mouseMotion);
    glutMouseFunc(mouseButton);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}