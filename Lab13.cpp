#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bmpfuncs.h"

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
void draw_teapot();
void draw_disk();
void draw_can();
void draw_text();

int current_width, current_height;
GLUquadricObj* qobj = gluNewQuadric();

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
}


void resize(int width, int height) {
	current_width = width; current_height = height;
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

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	draw_teapot();
	draw_disk();
	draw_can();
	draw_text();

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


void draw_teapot() {
	glViewport(current_width / 2, 0, current_width / 2, current_height / 2);
	draw_axis();
	glColor3f(1.0, 1.0, 0.0);
	glutWireTeapot(3.0);
	glFlush();
}


void draw_disk() {
	glViewport(current_width / 2, current_height / 2, current_width / 2, current_height / 2);
	draw_axis();
	glColor3f(0.0, 0.0, 1.0);
	gluPartialDisk(qobj, 1, 3, 30, 2, 0, 300);
	glFlush();
}


void draw_can() {
	glViewport(0, 0, current_width / 2, current_height / 2);
	draw_axis();
	glColor3f(0.5, 0.5, 0.5);
	glPushMatrix();
	glTranslated(0.0, 0.0, 4.0);
	gluDisk(qobj, 0.0, 1.0, 16, 4);
	glPopMatrix();
	glColor3f(0.1, 0.5, 0.5);
	gluCylinder(qobj, 1.0f, 1.0f, 4.0f, 16, 4);
	glColor3f(0.5, 0.5, 0.5);
	gluDisk(qobj, 0.0, 1.0, 16, 4);
	glFlush();
}


void draw_text() {
	glViewport(0, current_height / 2, current_width / 2, current_height / 2);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Graphics are cool!!", -4, 2);
	draw_string(GLUT_BITMAP_HELVETICA_18, "Multi-Viewport Example!!", -4, 0);
	glFlush();
}