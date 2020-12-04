#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include "GL/glext.h"
#include "bmpfuncs.h"
#define PI 3.1415926535897932384626433832795
using namespace std;

void init();
void resize(int, int);
void draw();
void draw_axis();
void draw_string(void*, const char*, int, int);
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void idle();
void main_menu_function(int);
double degreeToRadian(double degree) { return degree * (PI / 180); }
void MakeCheckerBoard(int);
void MakeSincSurface(int);

struct Point {
	double x, y, z;
	Point() : x(0), y(0), z(0) {}
	Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

double radius = 50;
double theta = 45, phi = 45;
Point camera;
Point center(0, 0, 0);
Point up(0, 1, 0);

int DisplayListStartIndex;


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("12151556 ¹Ú¼º¼ö");
	init();

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}


void init() {
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);

	DisplayListStartIndex = glGenLists(2);
	MakeCheckerBoard(DisplayListStartIndex);
	MakeSincSurface(DisplayListStartIndex + 1);
}


void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}


void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double thetaRadian = degreeToRadian(theta);
	double phiRadian = degreeToRadian(phi);
	camera.z = radius * sin(thetaRadian) * cos(phiRadian);
	camera.x = radius * sin(thetaRadian) * sin(phiRadian);
	camera.y = radius * cos(thetaRadian);

	if (theta > 180) up.y = -1;
	else up.y = 1;

	gluLookAt(camera.x, camera.y, camera.z,
		center.x, center.y, center.z,
		up.x, up.y, up.z);

	float light_position[] = { 0,50,0,1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	for (int i = -3; i <= 3; i++) {
		for (int j = -3; j <= 3; j++) {
			glPushMatrix();
			glTranslatef(10 * i, 0, 10 * j);
			glCallList(DisplayListStartIndex);
			glPopMatrix();
		}
	}

	glTranslatef(0, 3, 0);
	glCallList(DisplayListStartIndex + 1);

	glFlush();
	glutSwapBuffers(); //back buffer to front buffer
}


void draw_axis() {
	glLineWidth(3);
	glBegin(GL_LINES); //x: red, y: green, z: blue
	glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(4, 0, 0);
	glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 4, 0);
	glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 4);
	glEnd();
	glLineWidth(1);
}


void draw_string(void* font, const char* str, int x, int y) {
	glColor3f(1.0f, 1.0f, 1.0f);
	unsigned int i;
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
}


void mouse(int button, int state, int x, int y) {
	y = 500 - y - 1; //convert window coordinate to glut coordinate
	printf("Mouse button is clicked! (%d, %d, %d, %d) \n",
		button, state, x, y);
}


void motion(int x, int y) {
	printf("Mouse is moving! (%d, %d)\n", x, y);
}


void keyboard(unsigned char key, int x, int y) {
	glutPostRedisplay();
}


void specialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		phi -= 5;
		if (phi < 0) phi += 360;
		break;
	case GLUT_KEY_RIGHT:
		phi += 5;
		if (phi >= 360) phi -= 360;
		break;
	case GLUT_KEY_UP:
		theta -= 5;
		if (theta < 0) theta += 360;
		break;
	case GLUT_KEY_DOWN:
		theta += 5;
		if (theta >= 360) theta -= 360;
		break;
	case GLUT_KEY_PAGE_UP:
		radius -= 1;
		if (radius == 0) radius = 1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		radius += 1;
		break;
	}
	glutPostRedisplay();
}


void idle() {
	glutPostRedisplay();
}


void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);
	switch (option) {
	case 999:
		exit(0);
		break;
	}
}


void MakeCheckerBoard(int ListIndex) {
	glNewList(ListIndex, GL_COMPILE);
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	for (int i = -5; i < 5; i++) {
		for (int j = -5; j < 5; j++) {
			glPushMatrix();
			glTranslatef(i, 0, j);
			if ((i + j + 100) % 2 == 1) glColor3f(1.0, 1.0, 0.0);
			else glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_QUADS);
			glVertex3f(0.5f, 0, 0.5f);
			glVertex3f(-0.5f, 0, 0.5f);
			glVertex3f(-0.5f, 0, -0.5f);
			glVertex3f(0.5f, 0, -0.5f);
			glEnd();
			glPopMatrix();
		}
	}
	glPopAttrib();
	glEndList();
}


void MakeSincSurface(int ListIndex) {
	float x, z, d1, d2, d3, d4, y1, y2, y3, y4;
	float step = 0.2;
	float freq = 1.5;
	float peak = 10;

	glNewList(ListIndex, GL_COMPILE);
	glColor3f(0.0, 1.0, 0.0);
	for (x = -10; x < 10; x += step) {
		for (z = -10; z < 10; z += step) {
			d1 = sqrt(x * x + z * z);
			d2 = sqrt((x + step) * (x + step) + z * z);
			d3 = sqrt((x + step) * (x + step) + (z + step) * (z + step));
			d4 = sqrt(x * x + (z + step) * (z + step));
			if (d1 == 0) y1 = peak;
			else y1 = peak * sin(freq * d1) / (freq * d1);
			if (d2 == 0) y2 = peak;
			else y2 = peak * sin(freq * d2) / (freq * d2);
			if (d3 == 0) y3 = peak;
			else y3 = peak * sin(freq * d3) / (freq * d3);
			if (d4 == 0) y4 = peak;
			else y4 = peak * sin(freq * d4) / (freq * d4);
			glBegin(GL_TRIANGLES);
			glNormal3f(step, step, y3 - y1);
			glVertex3f(x, y1, z);
			glVertex3f(x + step, y2, z);
			glVertex3f(x + step, y3, z + step);
			glVertex3f(x + step, y3, z + step);
			glVertex3f(x, y4, z + step);
			glVertex3f(x, y1, z);
			glEnd();
		}
	}
	glEndList();
}