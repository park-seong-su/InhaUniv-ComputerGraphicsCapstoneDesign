#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>


void init();
void draw();
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void idle();
void sub_menu_function(int);
void main_menu_function(int);

int main(int argc, char** argv) {
	int submenu1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("12151556 ¹Ú¼º¼ö");
	init();

	submenu1 = glutCreateMenu(sub_menu_function);
	glutAddMenuEntry("backgroundRed", 1);
	glutAddMenuEntry("backgroundGreen", 2);
	glutAddMenuEntry("backgroundBlue", 3);

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAddMenuEntry("Go!", 11);
	glutAddSubMenu("Sub Menu", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	//glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}


void init() {
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
}


void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireTeapot(0.5f);
	glFlush();
}


void mouse(int button, int state, int x, int y) {
	printf("Mouse button is clicked! (%d, %d, %d, %d) \n",
		button, state, x, y);
}


void motion(int x, int y) {
	printf("Mouse is moving! (%d, %d)\n", x, y);
}


void keyboard(unsigned char key, int x, int y) {
	printf("You pressed %c\n", key);
}


void idle() {
	printf("%d second has elapsed since the system was started\n",
		GetTickCount() / 1000);
}


void sub_menu_function(int option) {
	printf("Sub menu %d has been selected\n", option);
	glClear(GL_COLOR_BUFFER_BIT);
	if (option == 1) glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
	if (option == 2) glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
	if (option == 3) glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireTeapot(0.5f);
	glFlush();
}


void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);
	if (option == 999) {
		exit(0);
	}
}