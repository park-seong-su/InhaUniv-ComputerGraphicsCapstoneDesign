#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

bool alphaBlendingOn = false;
bool antiAliasingOn = false;

struct Point {
	int x, y, mode;
	void setPoint(int _x, int _y, int m) {
		x = _x; y = _y; mode = m;
	}
	void clear() {
		mode = 0;
	}
};
const int pSize = 1000;
Point p[pSize]; int pIndex = 0;

/* 0(don't execute), 1(draw_point), 2(draw_line), 3(draw_triangle) */
int drawMode = 0;

void init();
void draw();
void draw_point();
void draw_lines();
void draw_triangles();
void draw_string(void*, const char*, int, int);
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void idle();
void draw_mode_menu_function(int);
void main_menu_function(int);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("12151556 박성수");
	init();

	int drawModeMenu = glutCreateMenu(draw_mode_menu_function);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Triangle", 3);

	glutCreateMenu(main_menu_function);
	glutAddSubMenu("DrawMode", drawModeMenu);
	glutAddMenuEntry("Clear", 998);
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
	/*화면의 기본색으로 검은색 설정*/
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/*화면 좌표 정보 설정*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.f); //argument left, right, bottom, top

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}


void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	draw_point();
	draw_lines();
	draw_triangles();
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24,
		"inha university", 10, 10);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24,
		"12151556 parkseongsu", 270, 10);

	glFlush();
}


void draw_point() {
	glColor4f(1.0f, 0.0f, 0.0f, 0.75f);
	glPointSize(8.0f);
	glBegin(GL_POINTS);

	for (int i = 0; i < pIndex; i++) {
		if (p[i].mode == 1) glVertex2i(p[i].x, p[i].y);
	}

	glEnd();
}


void draw_lines() {
	glLineWidth(1.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 0.75f);
	glBegin(GL_LINES);

	for (int i = 0; i < pIndex; i++) {
		if (p[i].mode == 2) glVertex2i(p[i].x, p[i].y);
	}

	glEnd();
}


void draw_triangles() {
	glColor4f(1.0f, 1.0f, 0.0f, 0.75f);
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < pIndex; i++) {
		if (p[i].mode == 3) glVertex2i(p[i].x, p[i].y);
	}

	glEnd();
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
	if (button == 0) { //mouse right botton
		switch (drawMode) {
		case 1:
			if (state == 0) p[pIndex++].setPoint(x, y, drawMode);
			break;
		case 2:
			p[pIndex++].setPoint(x, y, drawMode);
			break;
		case 3:
			if (state == 0) p[pIndex++].setPoint(x, y, drawMode);
			break;
		}
	}
}


void motion(int x, int y) {
	printf("Mouse is moving! (%d, %d)\n", x, y);
}


void keyboard(unsigned char key, int x, int y) {
	printf("You pressed %c\n", key);
	switch (key)
	{
	case 'a':
		if (!antiAliasingOn) {
			antiAliasingOn = true;
			glEnable(GL_POINT_SMOOTH);
			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_POLYGON_SMOOTH);
			printf("ANTI ALIASING ON\n");
		}
		else {
			antiAliasingOn = false;
			glDisable(GL_POINT_SMOOTH);
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_POLYGON_SMOOTH);
			printf("ANTI ALIASING OFF\n");
		}
		glutPostRedisplay();
		break;
	case 'b':
		if (!alphaBlendingOn) {
			alphaBlendingOn = true;
			glEnable(GL_BLEND);
			printf("ALPHA BLENDING ON\n");
		}
		else {
			alphaBlendingOn = false;
			glDisable(GL_BLEND);
			printf("ALPHA BLENDING OFF\n");
		}
		glutPostRedisplay();
		break;
	}
}


void idle() {
	printf("%d second has elapsed since the system was started\n",
		GetTickCount() / 1000);
}


void draw_mode_menu_function(int option) {
	printf("DrawMode menu %d has been selected\n", option);
	switch (option) {
	case 1:
		drawMode = 1;
		break;
	case 2:
		drawMode = 2;
		break;
	case 3:
		drawMode = 3;
		break;
	}
}


void main_menu_function(int option) {
	printf("Main menu %d has been selected\n", option);
	switch (option) {
	case 998:
		for (int i = 0; i < pIndex; i++) {
			p[i].clear();
		}
		pIndex = 0;
		drawMode = 0;
		glutPostRedisplay();
		break;
	case 999:
		exit(0);
		break;
	}
}
