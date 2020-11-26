#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

float g_pPosition[6] = { 0.0f, 144.0f, -100.0f, -100.0f, 100.0f, -100.0f };

int g_nX, g_nY;
int g_nSelect = 0;

int g_nGLWidth = 500, g_nGLHeight = 500;

GLUquadricObj* qobj = gluNewQuadric();

void init();
void resize(int, int);
void draw();
void draw_axis();
void draw_string(void*, const char*, float, float, float, float, float);
void mouse(int, int, int, int);
void motion(int, int);
void draw_mode_menu_function(int);
void main_menu_function(int);
void draw_background();
void draw_sphere();
void draw_sphere_cube_disk();
void picking(int, int);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(g_nGLWidth, g_nGLHeight);
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

	glutMainLoop();

	return 0;
}


void init() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f,
			g_nGLHeight / 2.0f, -100, 100);
}


void resize(int width, int height) {
	g_nGLWidth = width;
	g_nGLHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f,
		    -100, 100);
	glMatrixMode(GL_MODELVIEW);
}


void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	draw_background();
	//draw_sphere();
	draw_sphere_cube_disk();

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


void draw_string(void* font, const char* str, float x, float y, float r, float g, float b) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-5, 5, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(r, g, b);
	glRasterPos3f(x, y, 0);
	for (unsigned int i = 0; i < strlen(str); i++)
		glutBitmapCharacter(font, str[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}


void mouse(int button, int state, int x, int y) {
	y = g_nGLHeight - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		picking(x, y);
		g_nX = x;
		g_nY = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		g_nSelect = 0;
	}
}


void motion(int x, int y) {
	y = g_nGLHeight - y;
	if (g_nSelect > 0) {
		g_pPosition[(g_nSelect - 1) * 2 + 0] += x - g_nX;
		g_pPosition[(g_nSelect - 1) * 2 + 1] += y - g_nY;
		g_nX = x;
		g_nY = y;
		glutPostRedisplay();
	}
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


void draw_background() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	int i;
	int offset = 50;
	glBegin(GL_LINES);
	for (i = -g_nGLWidth / 2.0f; i < g_nGLWidth / 2.0f; i += offset)
	{
		glVertex3f(i, -g_nGLHeight / 2.0f, 0.0f);
		glVertex3f(i, g_nGLHeight / 2.0f, 0.0f);
	}
	for (i = -g_nGLHeight / 2.0f; i < g_nGLHeight / 2.0f; i += offset)
	{
		glVertex3f(-g_nGLWidth / 2.0f, i, 0.0f);
		glVertex3f(g_nGLWidth / 2.0f, i, 0.0f);
	}
	glEnd();
	glPopAttrib();
}


void draw_sphere() {
	glLoadName(1);
	glPushMatrix();
	glTranslatef(g_pPosition[0], g_pPosition[1], 0.0f);
	glColor3f(1, 0, 0); // red
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();

	glLoadName(2);
	glPushMatrix();
	glTranslatef(g_pPosition[2], g_pPosition[3], 1.0f);
	glColor3f(0, 1, 0); // green
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();

	glLoadName(3);
	glPushMatrix();
	glTranslatef(g_pPosition[4], g_pPosition[5], 2.0f);
	glColor3f(0, 0, 1); // blue
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();
}


void draw_sphere_cube_disk() {
	glLoadName(1);
	glPushMatrix();
	glTranslatef(g_pPosition[0], g_pPosition[1], 0.0f);
	glColor3f(1, 0, 0); // red
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();

	glLoadName(2);
	glPushMatrix();
	glTranslatef(g_pPosition[2], g_pPosition[3], 25.0f);
	glColor3f(0, 1, 0); // green
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0); // -x axis
	glVertex3f(-25.0, 25.0, -25.0); glVertex3f(-25.0, -25.0, -25.0);
	glVertex3f(-25.0, -25.0, 25.0); glVertex3f(-25.0, 25.0, 25.0);
	glNormal3f(1.0, 0.0, 0.0); // +x axis
	glVertex3f(25.0, 25.0, 25.0); glVertex3f(25.0, -25.0, 25.0);
	glVertex3f(25.0, -25.0, -25.0); glVertex3f(25.0, 25.0, -25.0);
	glNormal3f(0.0, -1.0, 0.0); // -y axis
	glVertex3f(-25.0, -25.0, -25.0); glVertex3f(25.0, -25.0, -25.0);
	glVertex3f(25.0, -25.0, 25.0);	glVertex3f(-25.0, -25.0, 25.0);
	glNormal3f(0.0, 1.0, 0.0); // +y axis
	glVertex3f(-25.0, 25.0, 25.0);	glVertex3f(25.0, 25.0, 25.0);
	glVertex3f(25.0, 25.0, -25.0);	glVertex3f(-25.0, 25.0, -25.0);
	glNormal3f(0.0, 0.0, -1.0); // -z axis
	glVertex3f(25.0, 25.0, -25.0);	glVertex3f(-25.0, 25.0, -25.0);
	glVertex3f(-25.0, -25.0, -25.0); glVertex3f(25.0, -25.0, -25.0);
	glNormal3f(0.0, 0.0, 1.0); // +z axis
	glVertex3f(25.0, 25.0, 25.0); glVertex3f(-25.0, 25.0, 25.0);
	glVertex3f(-25.0, -25.0, 25.0); glVertex3f(25.0, -25.0, 25.0);
	glEnd();
	glPopMatrix();

	glLoadName(3);
	glPushMatrix();
	glTranslatef(g_pPosition[4], g_pPosition[5], 30.0f);
	glColor3f(0, 0, 1); // blue
	gluPartialDisk(qobj, 0, 25, 30, 2, 0, 300);
	glPopMatrix();
}


void picking(int x, int y) {
	GLuint selectBuf[256];
	glSelectBuffer(256, selectBuf);
	glRenderMode(GL_SELECT);
	glMatrixMode(GL_PROJECTION);
	glInitNames();
	glPushName(-1);

	glPushMatrix();
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	gluPickMatrix(x, y, 0.1, 0.1, viewport);
	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f,
			g_nGLHeight / 2.0f, -100, 100);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//draw_sphere();
	draw_sphere_cube_disk();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits = glRenderMode(GL_RENDER);
	if (hits <= 0) return;
	
	int stack = selectBuf[0];
	unsigned int zMin = selectBuf[1];
	unsigned int zMax = selectBuf[2];
	g_nSelect = selectBuf[3];

	printf("hits: %d\n", hits);
	for (int i = 0; i < hits; i++) {
		printf("%u %u %u %u\n", 
			selectBuf[4 * i], selectBuf[4 * i + 1], 
			selectBuf[4 * i + 2], selectBuf[4 * i + 3]);
	}
	
	int index = 3 + stack;
	int i;
	for (i = 1; i < hits; i++) {
		stack = selectBuf[index];
		if (zMin > selectBuf[index + 1]) {
			zMin = selectBuf[index + 1];
			g_nSelect = selectBuf[index + 3];
		}
		index += 3 + stack;
	}
	printf("g_nSelect: %d\n\n", g_nSelect);
}

