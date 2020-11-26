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
void skybox_texture_mapping();
void draw_skybox();
void draw_sphere();
int inverse_matrix(vector<vector<GLfloat>>&, vector<vector<GLfloat>>&);

struct Point {
	double x, y, z;
	Point() : x(0), y(0), z(0) {}
	Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

double radius = 10;
double theta = 45, phi = 45;
Point camera;
Point center(0, 0, 0);
Point up(0, 1, 0);

GLuint g_nCubeTex;


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

	skybox_texture_mapping();

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
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

	glColor3f(1.0, 1.0, 1.0);
	draw_skybox();
	draw_sphere();

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


void skybox_texture_mapping() {
	glGenTextures(1, &g_nCubeTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	int width, height, channels;
	uchar* skybox;
	skybox = readImageData("skybox/left.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox);
	skybox = readImageData("skybox/right.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width,height, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox);
	skybox = readImageData("skybox/top.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox);
	skybox = readImageData("skybox/bottom.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox);
	skybox = readImageData("skybox/front.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox);
	skybox = readImageData("skybox/back.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
}


void draw_skybox() {
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	float g_nSkySize = 50.0f;
	glBegin(GL_QUADS);
	// positive x
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
	// negative x
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	// positive y
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
	// negative y
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	// positive z
	glTexCoord3d(1.0, -1.0, 1.0); glVertex3f(g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, -1.0, 1.0); glVertex3f(-g_nSkySize, -g_nSkySize, g_nSkySize);
	glTexCoord3d(-1.0, 1.0, 1.0); glVertex3f(-g_nSkySize, g_nSkySize, g_nSkySize);
	glTexCoord3d(1.0, 1.0, 1.0); glVertex3f(g_nSkySize, g_nSkySize, g_nSkySize);
	// negative z
	glTexCoord3d(1.0, -1.0, -1.0); glVertex3f(g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, -1.0, -1.0); glVertex3f(-g_nSkySize, -g_nSkySize, -g_nSkySize);
	glTexCoord3d(-1.0, 1.0, -1.0); glVertex3f(-g_nSkySize, g_nSkySize, -g_nSkySize);
	glTexCoord3d(1.0, 1.0, -1.0); glVertex3f(g_nSkySize, g_nSkySize, -g_nSkySize);
	glEnd();
}


void draw_sphere() {
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	
	GLfloat camera_matrix[16];
	GLfloat camera_inverse_matrix[16];
	vector<vector<GLfloat>> camera_matrix_2d(4, vector<GLfloat>(4, 0.0));
	vector<vector<GLfloat>> camera_inverse_matrix_2d(4, vector<GLfloat>(4, 0.0));

	glGetFloatv(GL_MODELVIEW_MATRIX, camera_matrix);
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			camera_matrix_2d[r][c] = camera_matrix[4 * r + c];

	if (inverse_matrix(camera_matrix_2d, camera_inverse_matrix_2d) == 0)
		printf("determinant 0 errer!\n");

	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			camera_inverse_matrix[4 * r + c] = camera_inverse_matrix_2d[r][c];

	glMultMatrixf(camera_inverse_matrix);
	
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_nCubeTex);
	glutSolidSphere(1.0f, 40, 40);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


int inverse_matrix(vector<vector<GLfloat>>& mat, vector<vector<GLfloat>>& inv) {
	inv[0][0] = mat[1][1] * mat[2][2] * mat[3][3]
				- mat[1][1] * mat[2][3] * mat[3][2]
				- mat[2][1] * mat[1][2] * mat[3][3]
				+ mat[2][1] * mat[1][3] * mat[3][2]
				+ mat[3][1] * mat[1][2] * mat[2][3]
				- mat[3][1] * mat[1][3] * mat[2][2];
	inv[0][1] =	- mat[0][1] * mat[2][2] * mat[3][3]
				+ mat[0][1] * mat[2][3] * mat[3][2]
				+ mat[2][1] * mat[0][2] * mat[3][3]
				- mat[2][1] * mat[0][3] * mat[3][2]
				- mat[3][1] * mat[0][2] * mat[2][3]
				+ mat[3][1] * mat[0][3] * mat[2][2];
	inv[0][2] = mat[0][1] * mat[1][2] * mat[3][3]
				- mat[0][1] * mat[1][3] * mat[3][2]
				- mat[1][1] * mat[0][2] * mat[3][3]
				+ mat[1][1] * mat[0][3] * mat[3][2]
				+ mat[3][1] * mat[0][2] * mat[1][3]
				- mat[3][1] * mat[0][3] * mat[1][2];
	inv[0][3] = - mat[0][1] * mat[1][2] * mat[2][3]
				+ mat[0][1] * mat[1][3] * mat[2][2]
				+ mat[1][1] * mat[0][2] * mat[2][3]
				- mat[1][1] * mat[0][3] * mat[2][2]
				- mat[2][1] * mat[0][2] * mat[1][3]
				+ mat[2][1] * mat[0][3] * mat[1][2];
	inv[1][0] = - mat[1][0] * mat[2][2] * mat[3][3]
				+ mat[1][0] * mat[2][3] * mat[3][2]
				+ mat[2][0] * mat[1][2] * mat[3][3]
				- mat[2][0] * mat[1][3] * mat[3][2]
				- mat[3][0] * mat[1][2] * mat[2][3]
				+ mat[3][0] * mat[1][3] * mat[2][2];
	inv[1][1] = mat[0][0] * mat[2][2] * mat[3][3]
				- mat[0][0] * mat[2][3] * mat[3][2]
				- mat[2][0] * mat[0][2] * mat[3][3]
				+ mat[2][0] * mat[0][3] * mat[3][2]
				+ mat[3][0] * mat[0][2] * mat[2][3]
				- mat[3][0] * mat[0][3] * mat[2][2];
	inv[1][2] = - mat[0][0] * mat[1][2] * mat[3][3]
				+ mat[0][0] * mat[1][3] * mat[3][2]
				+ mat[1][0] * mat[0][2] * mat[3][3]
				- mat[1][0] * mat[0][3] * mat[3][2]
				- mat[3][0] * mat[0][2] * mat[1][3]
				+ mat[3][0] * mat[0][3] * mat[1][2];
	inv[1][3] = mat[0][0] * mat[1][2] * mat[2][3]
				- mat[0][0] * mat[1][3] * mat[2][2]
				- mat[1][0] * mat[0][2] * mat[2][3]
				+ mat[1][0] * mat[0][3] * mat[2][2]
				+ mat[2][0] * mat[0][2] * mat[1][3]
				- mat[2][0] * mat[0][3] * mat[1][2];
	inv[2][0] = mat[1][0] * mat[2][1] * mat[3][3]
				- mat[1][0] * mat[2][3] * mat[3][1]
				- mat[2][0] * mat[1][1] * mat[3][3]
				+ mat[2][0] * mat[1][3] * mat[3][1]
				+ mat[3][0] * mat[1][1] * mat[2][3]
				- mat[3][0] * mat[1][3] * mat[2][1];
	inv[2][1] = - mat[0][0] * mat[2][1] * mat[3][3]
				+ mat[0][0] * mat[2][3] * mat[3][1]
				+ mat[2][0] * mat[0][1] * mat[3][3]
				- mat[2][0] * mat[0][3] * mat[3][1]
				- mat[3][0] * mat[0][1] * mat[2][3]
				+ mat[3][0] * mat[0][3] * mat[2][1];
	inv[2][2] = mat[0][0] * mat[1][1] * mat[3][3]
				- mat[0][0] * mat[1][3] * mat[3][1]
				- mat[1][0] * mat[0][1] * mat[3][3]
				+ mat[1][0] * mat[0][3] * mat[3][1]
				+ mat[3][0] * mat[0][1] * mat[1][3]
				- mat[3][0] * mat[0][3] * mat[1][1];
	inv[2][3] = - mat[0][0] * mat[1][1] * mat[2][3]
				+ mat[0][0] * mat[1][3] * mat[2][1]
				+ mat[1][0] * mat[0][1] * mat[2][3]
				- mat[1][0] * mat[0][3] * mat[2][1]
				- mat[2][0] * mat[0][1] * mat[1][3]
				+ mat[2][0] * mat[0][3] * mat[1][1];
	inv[3][0] = - mat[1][0] * mat[2][1] * mat[3][2]
				+ mat[1][0] * mat[2][2] * mat[3][1]
				+ mat[2][0] * mat[1][1] * mat[3][2]
				- mat[2][0] * mat[1][2] * mat[3][1]
				- mat[3][0] * mat[1][1] * mat[2][2]
				+ mat[3][0] * mat[1][2] * mat[2][1];
	inv[3][1] = mat[0][0] * mat[2][1] * mat[3][2]
				- mat[0][0] * mat[2][2] * mat[3][1]
				- mat[2][0] * mat[0][1] * mat[3][2]
				+ mat[2][0] * mat[0][2] * mat[3][1]
				+ mat[3][0] * mat[0][1] * mat[2][2]
				- mat[3][0] * mat[0][2] * mat[2][1];
	inv[3][2] = - mat[0][0] * mat[1][1] * mat[3][2]
				+ mat[0][0] * mat[1][2] * mat[3][1]
				+ mat[1][0] * mat[0][1] * mat[3][2]
				- mat[1][0] * mat[0][2] * mat[3][1]
				- mat[3][0] * mat[0][1] * mat[1][2]
				+ mat[3][0] * mat[0][2] * mat[1][1];
	inv[3][3] =	mat[0][0] * mat[1][1] * mat[2][2]
				- mat[0][0] * mat[1][2] * mat[2][1]
				- mat[1][0] * mat[0][1] * mat[2][2]
				+ mat[1][0] * mat[0][2] * mat[2][1]
				+ mat[2][0] * mat[0][1] * mat[1][2]
				- mat[2][0] * mat[0][2] * mat[1][1];

	GLfloat det = mat[0][0] * inv[0][0] + mat[0][1] * inv[1][0] +
		mat[0][2] * inv[2][0] + mat[0][3] * inv[3][0];

	if (det == 0.0) return 0;
	det = 1.0 / det;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			inv[i][j] *= det;
	return 1;
}