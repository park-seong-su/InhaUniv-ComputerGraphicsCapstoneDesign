#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

const int tranXSize = 5;
const int tranYSize = 5;

int spinState;               // 0: stop, 1: ccw spin, 2: cw spin
double spin;                 // rotation value
int tranX;                   // X translation value
int tranY;                   // Y translation value
double scale = 1.0;          // scaling value
char pivotPointStr[100];     // pivot point string
GLfloat currentMatrix[16];   // current matrix array

struct Point {
	int x, y;
	Point() : x(0), y(0) {}
	void setPoint(int _x, int _y) { x = _x; y = _y; }
};

void init();
void draw();
void draw_triangle();
void points_over_window_exeception(Point[]);
Point& calc_after_transformation(Point&);
void draw_string(void*, const char*, int, int);
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void idle();
void draw_mode_menu_function(int);
void main_menu_function(int);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("12151556 박성수");
	init();

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

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
	/* 화면의 기본색으로 검정색 설정 */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* 화면 좌표 정보 설정 */
	glMatrixMode(GL_PROJECTION); //3차원을 2차원으로 투영
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.f); //argument left, right, bottom, top
}


void draw() {
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(250 + tranX, 250 + tranY, 0);
	glRotatef(spin, 0, 0, 1);
	glScalef(scale, scale, scale);
	glTranslatef(-250, -250, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, currentMatrix);

	draw_triangle();

	glLoadIdentity();
	sprintf_s(pivotPointStr, "pivot point: (%d, %d)", 250 + tranX, 250 + tranY);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24,
		pivotPointStr, 10, 10);
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24,
		"12151556 parkseongsu", 270, 10);

	glFlush();
	glutSwapBuffers(); //back buffer to front buffer
}


void draw_triangle() {
	glColor4f(1.0f, 1.0f, 0.0f, 0.75f);
	Point p[3];
	p[0].setPoint(220, 230); p[1].setPoint(250, 290); p[2].setPoint(280, 230);
	points_over_window_exeception(p);

	glBegin(GL_TRIANGLES);

	glVertex2i(p[0].x, p[0].y);
	glVertex2i(p[1].x, p[1].y);
	glVertex2i(p[2].x, p[2].y);

	glEnd();
}


void points_over_window_exeception(Point p[]) {
	// 1: over left in window, 2: over right in window
	// 3: over top in window, 4: over bottom in winodw

	int exceptionState = 0;
	Point pAfterTrans[3];
	for (int i = 0; i < 3; i++) pAfterTrans[i] = calc_after_transformation(p[i]);

	for (int i = 0; i < 3; i++) {
		if (pAfterTrans[i].x < 0) { exceptionState = 1; break; }
		else if (pAfterTrans[i].x > 500) { exceptionState = 2; break; }
		else if (pAfterTrans[i].y < 0) { exceptionState = 3; break; }
		else if (pAfterTrans[i].y > 500) { exceptionState = 4; break; }
	}

	switch (exceptionState) {
	case 1:
		for (int i = 0; i < 3; i++) p[i].x += tranXSize;
		tranX += tranXSize;
		break;
	case 2:
		for (int i = 0; i < 3; i++) p[i].x -= tranXSize;
		tranX -= tranXSize;
		break;
	case 3:
		for (int i = 0; i < 3; i++) p[i].y += tranYSize;
		tranY += tranYSize;
		break;
	case 4:
		for (int i = 0; i < 3; i++) p[i].y -= tranYSize;
		tranY -= tranYSize;
		break;
	}
}


Point& calc_after_transformation(Point& p) {
	int x = currentMatrix[0] * p.x + currentMatrix[4] * p.y
		+ currentMatrix[8] * 0 + currentMatrix[12] * 1;
	int y = currentMatrix[1] * p.x + currentMatrix[5] * p.y
		+ currentMatrix[9] * 0 + currentMatrix[13] * 1;
	int z = currentMatrix[2] * p.x + currentMatrix[6] * p.y
		+ currentMatrix[10] * 0 + currentMatrix[14] * 1;
	int h = currentMatrix[3] * p.x + currentMatrix[7] * p.y
		+ currentMatrix[11] * 0 + currentMatrix[15] * 1;
	if (h != 0) x /= h; y /= h; z /= h;
	Point ret;
	ret.setPoint(x, y);
	return ret;
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
	case 's':
		switch (spinState) {
		case 0:
			spinState = 1;
			printf("CCW SPIN ON\n");
			break;
		case 1:
			spinState = 2;
			printf("CCW SPIN OFF\n");
			printf("CW SPIN ON\n");
			break;
		case 2:
			spinState = 0;
			printf("SPIN OFF\n");
			break;
		}
		break;
	case '1': scale = 1.0; printf("scale 1\n"); break;
	case '2': scale = 2.0; printf("scale 2\n"); break;
	case '3': scale = 3.0; printf("scale 3\n"); break;
	case '4': scale = 4.0; printf("scale 4\n"); break;
	case '5': scale = 5.0; printf("scale 5\n"); break;
	}
	glutPostRedisplay();
}


void specialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		printf("You pressed left\n");
		tranX -= tranXSize;
		break;
	case GLUT_KEY_RIGHT:
		printf("You pressed right\n");
		tranX += tranXSize;
		break;
	case GLUT_KEY_UP:
		printf("You pressed up\n");
		tranY += tranYSize;
		break;
	case GLUT_KEY_DOWN:
		printf("You pressed down\n");
		tranY -= tranYSize;
		break;
	}
	glutPostRedisplay();
}


void idle() {
	switch (spinState) {
	case 1:
		spin = spin + 0.1;
		if (spin > 360) spin -= 360;
		break;
	case 2:
		spin = spin - 0.1;
		if (spin < 0) spin += 360;
		break;
	}
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
