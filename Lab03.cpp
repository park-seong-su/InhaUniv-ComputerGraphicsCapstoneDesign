#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>

bool lineStippleOn = false;
bool cullFaceOn = false;

void init();
void draw();
void draw_point();
void draw_lines();
void draw_line_strip();
void draw_line_loop();
void draw_triangles();
void draw_triangle_strip();
void draw_triangle_fan();
void draw_quads();
void draw_quad_strip();
void draw_ploygon();
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void idle();
void line_menu_function(int);
void ploygon_menu_function(int);
void main_menu_function(int);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("12151556 �ڼ���");
	init();
	
	int lineMenu = glutCreateMenu(line_menu_function);
	glutAddMenuEntry("Line", 1);
	glutAddMenuEntry("strip Line", 2);
	glutAddMenuEntry("Loop Line", 3);

	int ploygonMenu = glutCreateMenu(ploygon_menu_function);
	glutAddMenuEntry("Triangles", 1);
	glutAddMenuEntry("Triangle strip", 2);
	glutAddMenuEntry("Triangle fan", 3);
	glutAddMenuEntry("Quads", 4);
	glutAddMenuEntry("Quad strip", 5);
	glutAddMenuEntry("Polygon", 6);

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Point", 1);
	glutAddSubMenu("Line", lineMenu);
	glutAddSubMenu("Ploygon", ploygonMenu);
	glutAddMenuEntry("Quit", 999);
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
	/*ȭ���� �⺻������ blue ����*/
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

	/*ȭ�� ��ǥ ���� ����*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.f); //argument left, right, bottom, top
}


void draw() {
	/*ȭ���� �����ϰ� �����*/
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}


void draw_point() {
	/*���� ������ �ʷϻ�����*/
	glColor3f(0.0f, 1.0f, 0.0f);
	/*���� ũ�� (�ʱⰪ�� 1.0)*/
	glPointSize(4.0f);
	glBegin(GL_POINTS);

	glVertex2i(100, 100); //���ϴܺ��� 0,0
	glVertex2i(200, 400);
	glVertex2i(450, 300);
	
	glEnd();
}


void draw_lines() {
	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);

	glVertex2i(50, 50);
	glVertex2i(250, 450);
	glVertex2i(300, 450);
	glVertex2i(400, 100);

	glEnd();
}


void draw_line_strip() {
	glLineWidth(5.0f);
	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	
	glVertex2i(50, 50);
	glVertex2i(250, 450);
	glVertex2i(300, 450);
	glVertex2i(400, 100);

	glEnd();
}


void draw_line_loop() {
	glLineWidth(10.0f);
	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);

	glVertex2i(50, 50);
	glVertex2i(250, 450);
	glVertex2i(300, 450);
	glVertex2i(400, 100);
	
	glEnd();
}


void draw_triangles() {
	glPolygonMode(GL_FRONT, GL_FILL); //ccw����(�ո�)�� ä���
	glPolygonMode(GL_BACK, GL_LINE); //cw����(�޸�)�� ������ ǥ��
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);

	/*CW*/
	glVertex2i(100, 50);
	glVertex2i(100, 300);
	glVertex2i(170, 120);
	/*CCW*/
	glVertex2i(400, 200);
	glVertex2i(300, 400);
	glVertex2i(200, 200);

	glEnd();
}


void draw_triangle_strip() {
	glPolygonMode(GL_FRONT, GL_FILL); //ccw����(�ո�)�� ä���
	glPolygonMode(GL_BACK, GL_LINE); //cw����(�޸�)�� ������ ǥ��
	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);

	/*CW*/
	glVertex2i(100, 50);
	glVertex2i(100, 300);
	glVertex2i(170, 120);
	/*CCW*/
	glVertex2i(400, 200);
	glVertex2i(300, 400);
	glVertex2i(200, 200);

	glEnd();
}


void draw_triangle_fan() {
	glPolygonMode(GL_FRONT, GL_FILL); //ccw����(�ո�)�� ä���
	glPolygonMode(GL_BACK, GL_LINE); //cw����(�޸�)�� ������ ǥ��
	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLE_FAN);

	/*CW*/
	glVertex2i(100, 50);
	glVertex2i(100, 300);
	glVertex2i(170, 120);
	/*CCW*/
	glVertex2i(400, 200);
	glVertex2i(300, 400);
	glVertex2i(200, 200);

	glEnd();
}


void draw_quads() {
	glPolygonMode(GL_FRONT, GL_FILL); //ccw����(�ո�)�� ä���
	glPolygonMode(GL_BACK, GL_LINE); //cw����(�޸�)�� ������ ǥ��
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);

	/*CW*/
	glVertex2i(50, 50);
	glVertex2i(70, 300);
	glVertex2i(100, 300);
	glVertex2i(150, 200);
	/*CCW*/
	glVertex2i(400, 100);
	glVertex2i(400, 400);
	glVertex2i(250, 400);
	glVertex2i(200, 100);

	glEnd();
}


void draw_quad_strip() {
	glPolygonMode(GL_FRONT, GL_FILL); //ccw����(�ո�)�� ä���
	glPolygonMode(GL_BACK, GL_LINE); //cw����(�޸�)�� ������ ǥ��
	glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_QUAD_STRIP);

	/*CW*/
	glVertex2i(50, 50);
	glVertex2i(70, 300);
	glVertex2i(100, 300);
	glVertex2i(150, 200);
	/*CCW*/
	glVertex2i(400, 100);
	glVertex2i(400, 400);
	glVertex2i(250, 400);
	glVertex2i(200, 100);

	glEnd();
}


void draw_ploygon() {
	glPolygonMode(GL_FRONT, GL_FILL); //ccw����(�ո�)�� ä���
	glPolygonMode(GL_BACK, GL_LINE); //cw����(�޸�)�� ������ ǥ��
	glColor3f(1.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);

	/*CW*/
	glVertex2i(50, 50);
	glVertex2i(200, 300);
	glVertex2i(300, 400);
	glVertex2i(400, 300);
	glVertex2i(450, 100);

	glEnd();
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
	switch (key)
	{
	case 'l':
		if (!lineStippleOn) {
			lineStippleOn = true;
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(4, 0xAAAA);
			printf("GL_LINE_STIPPLE ON\n");
		}
		else {
			lineStippleOn = false;
			glDisable(GL_LINE_STIPPLE);
			printf("GL_LINE_STIPPLE OFF\n");
		}
		break;
	case 'c':
		if (!cullFaceOn) {
			cullFaceOn = true;
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK); //�޸� ����
			printf("GL_CULL_FACE ON\n");
		}
		else {
			cullFaceOn = false;
			glDisable(GL_CULL_FACE);
			printf("GL_CULL_FACE OFF\n");
		}
		break;
	}
}


void idle() {
	printf("%d second has elapsed since the system was started\n",
		GetTickCount() / 1000);
}


void line_menu_function(int option) {
	printf("Line menu %d has been selected\n", option);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (option) {
	case 1:
		draw_lines();
		break;
	case 2:
		draw_line_strip();
		break;
	case 3:
		draw_line_loop();
		break;
	}
	glFlush();
}


void ploygon_menu_function(int option) {
	printf("Ploygon menu %d has been selected\n", option);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (option) {
	case 1:
		draw_triangles();
		break;
	case 2:
		draw_triangle_strip();
		break;
	case 3:
		draw_triangle_fan();
		break;
	case 4:
		draw_quads();
		break;
	case 5:
		draw_quad_strip();
		break;
	case 6:
		draw_ploygon();
		break;
	}
	glFlush();
}


void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (option) {
	case 1:
		draw_point();
		break;
	case 999:
		exit(0);
		break;
	}
	glFlush();
}
