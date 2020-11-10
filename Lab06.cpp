#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define PI 3.1415926535897932384626433832795

struct Point {
	double x, y, z;
	Point() : x(0), y(0), z(0) {}
	Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

double radius = 20;
double theta = 45, phi = 45;
Point camera;
Point center(0, 0, 0);
Point up(0, 1, 0);

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
void draw_mode_menu_function(int);
void main_menu_function(int);
double degreeToRadian(double);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
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
	//glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}


void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}


void draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double thetaRadian = degreeToRadian(theta);
	double phiRadian = degreeToRadian(phi);
	camera.z = radius * sin(thetaRadian) * cos(phiRadian);
	camera.x = radius * sin(thetaRadian) * sin(phiRadian);
	camera.y = radius * cos(thetaRadian);

	gluLookAt(camera.x, camera.y, camera.z,
			  center.x, center.y, center.z,
			  up.x, up.y, up.z);

	glColor3f(1, 1, 0);
	glutWireTeapot(4);
	draw_axis();

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
	printf("You pressed %c\n", key);
	switch (key) {
	case 'i':
		printf("init radious, theta, phi\n");
		radius = 20; theta = 45; phi = 45;
		break;
	}
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
		if (theta < 0) {
			theta += 10;
			phi += 180;
			if (phi >= 360) phi -= 360;
		}
		break;
	case GLUT_KEY_DOWN:
		theta += 5;
		if (theta > 180) {
			theta -= 10;
			phi += 180;
			if (phi >= 360) phi -= 360;
		}
		break;
	case GLUT_KEY_PAGE_UP:
		radius -= 1;
		if (radius == 0) radius = 1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		radius += 1;
		break;
	}
	printf("r: %f theta: %f phi : %f\n", radius, theta, phi);
	glutPostRedisplay();
}


void idle() {
	glutPostRedisplay();
}


void draw_mode_menu_function(int option) {
	printf("DrawMode menu %d has been selected\n", option);
}


void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);
	switch (option) {
	case 999:
		exit(0);
		break;
	}
}


double degreeToRadian(double degree) {
	return degree * (PI / 180);
}