#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "bmpfuncs.h"
#define PI 3.1415926535897932384626433832795
#define CAMERA_LIGHT GL_LIGHT0
#define CUBE_LIGHT GL_LIGHT1
#define CYLINDER_LIGHT GL_LIGHT2
#define SPHERE_LIGHT GL_LIGHT3

void init();
void resize(int, int);
void draw();
void draw_axis();
void draw_string(void*, const char*, float, float, float, float, float);
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void print_keyboard_navigation();
void idle();
void draw_mode_menu_function(int);
void main_menu_function(int);
double degreeToRadian(double degree) { return degree * (PI / 180); }
void light_default();
void cube_texture_mapping();
void cylinder_texture_mapping();
void sphere_texture_mapping();
void draw_cube();
void draw_cylinder();
void draw_sphere();
void draw_keyboard_navigation();

struct Point {
	double x, y, z;
	Point() : x(0), y(0), z(0) {}
	Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

double radius = 15;
double theta = 45, phi = 45;
Point camera;
Point center(0, 0, 0);
Point up(0, 1, 0);

int current_light = CAMERA_LIGHT;
GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat emission[] = { 0.0, 0.0, 0.0, 1.0 };

GLuint CubeTexture[6];
GLuint CylinderTexture[3];
GLuint* SphereTexture = new GLuint();
GLUquadricObj* qobj = gluNewQuadric();
float navi_camera_rgb[3] = { 1.0, 1.0, 0.0 }, navi_cube_rgb[3] = { 1.0, 1.0, 1.0 },
navi_cylinder_rgb[3] = { 1.0, 1.0, 1.0 }, navi_sphere_rgb[3] = { 1.0, 1.0, 1.0 };

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

	light_default();

	print_keyboard_navigation();

	cube_texture_mapping();
	cylinder_texture_mapping();
	sphere_texture_mapping();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricTexture(qobj, GL_TRUE);

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

	if (current_light == CAMERA_LIGHT) {
		light_position[0] = camera.x; light_position[1] = camera.y; light_position[2] = camera.z;
	}
	else {
		light_position[0] = 0; light_position[1] = 0; light_position[2] = 0;
	}

	glLightfv(CAMERA_LIGHT, GL_POSITION, light_position);

	glColor3f(1.0, 1.0, 1.0);
	draw_cube();
	draw_cylinder();
	draw_sphere();
	draw_keyboard_navigation();

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
	y = 500 - y - 1; //convert window coordinate to glut coordinate
	printf("Mouse button is clicked! (%d, %d, %d, %d) \n",
		button, state, x, y);
}


void motion(int x, int y) {
	printf("Mouse is moving! (%d, %d)\n", x, y);
}


void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'i':
		radius = 15; theta = 45; phi = 45;
		current_light = CAMERA_LIGHT;
		glDisable(CUBE_LIGHT); glDisable(CYLINDER_LIGHT); glDisable(SPHERE_LIGHT);
		glEnable(CAMERA_LIGHT);
		navi_cube_rgb[2] = 1.0; navi_cylinder_rgb[2] = 1.0; navi_sphere_rgb[2] = 1.0;
		navi_camera_rgb[2] = 0.0;
		break;
	case 27:
		exit(0);
		break;
	case '0':
		current_light = CAMERA_LIGHT;
		glDisable(CUBE_LIGHT); glDisable(CYLINDER_LIGHT); glDisable(SPHERE_LIGHT);
		glEnable(CAMERA_LIGHT);
		navi_cube_rgb[2] = 1.0; navi_cylinder_rgb[2] = 1.0; navi_sphere_rgb[2] = 1.0;
		navi_camera_rgb[2] = 0.0;
		break;
	case '1':
		current_light = CUBE_LIGHT;
		glDisable(CAMERA_LIGHT); glDisable(CYLINDER_LIGHT); glDisable(SPHERE_LIGHT);
		glEnable(CUBE_LIGHT);
		navi_camera_rgb[2] = 1.0; navi_cylinder_rgb[2] = 1.0; navi_sphere_rgb[2] = 1.0;
		navi_cube_rgb[2] = 0.0;
		break;
	case '2':
		current_light = CYLINDER_LIGHT;
		glDisable(CUBE_LIGHT); glDisable(CAMERA_LIGHT); glDisable(SPHERE_LIGHT);
		glEnable(CYLINDER_LIGHT);
		navi_cube_rgb[2] = 1.0; navi_camera_rgb[2] = 1.0; navi_sphere_rgb[2] = 1.0;
		navi_cylinder_rgb[2] = 0.0;
		break;
	case '3':
		current_light = SPHERE_LIGHT;
		glDisable(CUBE_LIGHT); glDisable(CYLINDER_LIGHT); glDisable(CAMERA_LIGHT);
		glEnable(SPHERE_LIGHT);
		navi_cube_rgb[2] = 1.0; navi_cylinder_rgb[2] = 1.0; navi_camera_rgb[2] = 1.0;
		navi_sphere_rgb[2] = 0.0;
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


void print_keyboard_navigation() {
	printf("------------------------Keyboard Naviagtion------------------------\n");
	printf("page up : zoom in\n");
	printf("page down : zoom out\n");
	printf("arrow : camera position\n");
	printf("number 0 : camera light\n");
	printf("number 1 : cube light\n");
	printf("number 2 : cylinder light\n");
	printf("number 3 : sphere light\n");
	printf("i : init\n");
	printf("esc : exit\n");
	printf("-------------------------------------------------------------------\n\n");
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


void light_default() {
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(CAMERA_LIGHT, GL_AMBIENT, light_ambient);
	glLightfv(CAMERA_LIGHT, GL_DIFFUSE, light_diffuse);
	glLightfv(CAMERA_LIGHT, GL_SPECULAR, light_specular);
	glLightfv(CUBE_LIGHT, GL_AMBIENT, light_ambient);
	glLightfv(CUBE_LIGHT, GL_DIFFUSE, light_diffuse);
	glLightfv(CUBE_LIGHT, GL_SPECULAR, light_specular);
	glLightfv(CYLINDER_LIGHT, GL_AMBIENT, light_ambient);
	glLightfv(CYLINDER_LIGHT, GL_DIFFUSE, light_diffuse);
	glLightfv(CYLINDER_LIGHT, GL_SPECULAR, light_specular);
	glLightfv(SPHERE_LIGHT, GL_AMBIENT, light_ambient);
	glLightfv(SPHERE_LIGHT, GL_DIFFUSE, light_diffuse);
	glLightfv(SPHERE_LIGHT, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHTING);
	glEnable(CAMERA_LIGHT);
}


void cube_texture_mapping() {
	glGenTextures(6, CubeTexture);
	int imgWidth, imgHeight, channels;
	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, CubeTexture[i]);
		char buf[100];
		sprintf(buf, "img/TexImage%d.bmp", i);
		buf[strlen(buf)] = 0;
		uchar* img = readImageData(buf, &imgWidth, &imgHeight, &channels);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}


void cylinder_texture_mapping() {
	glGenTextures(3, CylinderTexture);

	glBindTexture(GL_TEXTURE_2D, CylinderTexture[0]);
	int width, height, channels;
	uchar* img = readImageData("img/CIDER_T.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, CylinderTexture[1]);
	img = readImageData("img/coke.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, CylinderTexture[2]);
	img = readImageData("img/CIDER_B.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}


void sphere_texture_mapping() {
	glGenTextures(1, SphereTexture);

	glBindTexture(GL_TEXTURE_2D, *SphereTexture);
	int width, height, channels;
	uchar* img = readImageData("img/EARTH.bmp", &width, &height, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}


void draw_cube() {
	glPushMatrix();
	glTranslatef(1.0, 0.0, 1.0);
	glLightfv(CUBE_LIGHT, GL_POSITION, light_position);

	glBindTexture(GL_TEXTURE_2D, CubeTexture[0]);
	glBegin(GL_QUADS);
	glNormal3f(-1.0, 0.0, 0.0); // -x axis
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, CubeTexture[1]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0); // +x axis
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, CubeTexture[2]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0); // -y axis
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, CubeTexture[3]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); // +y axis
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, CubeTexture[4]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0); // -z axis
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, CubeTexture[5]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0); // +z axis
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -1.0, 1.0);
	glEnd();

	glPopMatrix();
}


void draw_cylinder() {
	glPushMatrix();
	glTranslatef(-5.0, -2.0, 0.5);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glLightfv(CYLINDER_LIGHT, GL_POSITION, light_position);

	//draw top
	glPushMatrix();
	glTranslated(0.0, 0.0, 4.0);
	glBindTexture(GL_TEXTURE_2D, CylinderTexture[0]);
	gluDisk(qobj, 0.0, 1.0, 16, 4); // (*quad, inner, outer, slices, stacks)
	glPopMatrix();
	//draw middle
	glBindTexture(GL_TEXTURE_2D, CylinderTexture[1]);
	gluCylinder(qobj, 1.0f, 1.0f, 4.0f, 16, 4); // (*quad, base, top, height, slices, stacks)
	//draw bottom
	glBindTexture(GL_TEXTURE_2D, CylinderTexture[2]);
	gluDisk(qobj, 0.0, 1.0, 16, 4);

	glPopMatrix();
}


void draw_sphere() {
	glPushMatrix();
	glTranslatef(2.0, 0.0, -5.0);
	glLightfv(SPHERE_LIGHT, GL_POSITION, light_position);

	glBindTexture(GL_TEXTURE_2D, *SphereTexture);
	gluSphere(qobj, 1, 30, 30);

	glPopMatrix();
}


void draw_keyboard_navigation() {
	glBindTexture(GL_TEXTURE_2D, 0); //unbind
	draw_string(GLUT_BITMAP_9_BY_15, "page up : zoom in", -4.9, 4.8, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_9_BY_15, "page down : zoom out", -4.9, 4.6, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_9_BY_15, "arrow : camera position", -4.9, 4.4, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_9_BY_15, "number 0 : camera light", -4.9, 4.2, navi_camera_rgb[0], navi_camera_rgb[1], navi_camera_rgb[2]);
	draw_string(GLUT_BITMAP_9_BY_15, "number 1 : cube light", -4.9, 4.0, navi_cube_rgb[0], navi_cube_rgb[1], navi_cube_rgb[2]);
	draw_string(GLUT_BITMAP_9_BY_15, "number 2 : cylinder light", -4.9, 3.8, navi_cylinder_rgb[0], navi_cylinder_rgb[1], navi_cylinder_rgb[2]);
	draw_string(GLUT_BITMAP_9_BY_15, "number 3 : sphere light", -4.9, 3.6, navi_sphere_rgb[0], navi_sphere_rgb[1], navi_sphere_rgb[2]);
	draw_string(GLUT_BITMAP_9_BY_15, "i : init", -4.9, 3.4, 1.0, 1.0, 1.0);
	draw_string(GLUT_BITMAP_9_BY_15, "esc : exit", -4.9, 3.2, 1.0, 1.0, 1.0);
}