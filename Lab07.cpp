#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define SLICES 30 // Sphere 세로 방향의 선의 갯수(경도)
#define STACKS 30 // Sphere 가로 방향의 선의 갯수(위도)

class Star {
public:
	Star() : radius(0.0), rotationSpeed(0.0), rotationSpin(0.0) {}
	Star(double r, double speed, double spin) 
		: radius(r), rotationSpeed(speed), rotationSpin(spin) {}
	double radius;           // 반지름
	double rotationSpeed;    // 자전속도
	double rotationSpin;     // 자전각도
};

class Planet : public Star {
public:
	Planet() : Star(0.0, 0.0, 0.0), orbitalSpeed(0.0), orbitalSpin(0.0) {}
	Planet(double r, double rSpeed, double rSpin, double oSpeed, double oSpin) 
		: Star(r, rSpeed, rSpin), orbitalSpeed(oSpeed), orbitalSpin(oSpin) {}
	double orbitalSpeed;     // 공전속도
	double orbitalSpin;      // 공전각도
};

Star sun(5.0, 0.1, 0.0);                     // 태양
Planet mercury(1.0, 0.02, 0.0, 0.09, 0.0);   // 수성
Planet venus(1.5, 0.01, 0.0, 0.05, 0.0);     // 금성
Planet earth(1.7, 0.5, 0.0, 0.03, 0.0);      // 지구
Planet moon(0.5, 0.5, 0.0, 0.6, 0.0);        // 달
Planet mars(1.4, 0.7, 0.0, 0.02, 0.0);       // 화성

void init();
void resize(int, int);
void draw();
void draw_axis();
void draw_string(void*, const char*, int, int);
void mouse(int, int, int, int);
void motion(int, int);
void keyboard(unsigned char, int, int);
void idle();
void draw_mode_menu_function(int);
void main_menu_function(int);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("12151556 박성수");
	init();

	glutCreateMenu(main_menu_function);
	glutAddMenuEntry("Quit", 999);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
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
	gluLookAt(0, 60, 0, 0, 0, 0, 0, 0, -1);            // viewing transformation
	draw_axis(); 
	
	glPushMatrix();
	glRotatef(sun.rotationSpin, 0, 1, 0);              // sun rotation
	glColor3f(1, 0, 0);
	glutWireSphere(sun.radius, SLICES, STACKS);        // draw sun
	glPopMatrix();

	glPushMatrix();
	glRotatef(mercury.orbitalSpin, 0, 1, 0);           // mercury orbital
	glTranslatef(sun.radius + 3, 0, 0);
	glRotatef(mercury.rotationSpin, 0, 1, 0);          // mercury rotation
	glColor3f(0.6, 0.6, 0.6);
	glutWireSphere(mercury.radius, SLICES, STACKS);    // draw mercury
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(venus.orbitalSpin, 0, 1, 0);             // venus orbital
	glTranslatef(sun.radius + 7, 0, 0);
	glRotatef(venus.rotationSpin, 0, 1, 0);            // venus rotation
	glColor3f(0.8, 0.6, 0);
	glutWireSphere(venus.radius, SLICES, STACKS);      // draw venus
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(earth.orbitalSpin, 0, 1, 0);             // earth orbital
	glTranslatef(sun.radius + 12, 0, 0);
	glPushMatrix();
	glRotatef(earth.rotationSpin, 0, 1, 0);            // earth rotation
	glColor3f(0, 0, 1);
	glutWireSphere(earth.radius, SLICES, STACKS);      // draw earth
	glPopMatrix();

	glRotatef(moon.orbitalSpin, 0, 1, 0);              // moon orbital
	glTranslatef(earth.radius + 1, 0, 0);
	glRotatef(moon.rotationSpin, 0, 1, 0);             // moon rotation
	glColor3f(0.8, 0.8, 0.8);
	glutWireSphere(moon.radius, SLICES, STACKS);       //draw moon
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(mars.orbitalSpin, 0, 1, 0);              // mars orbital
	glTranslatef(sun.radius + 18, 0, 0);
	glRotatef(mars.rotationSpin, 0, 1, 0);             // mars rotation
	glColor3f(0.8, 0.3, 0);
	glutWireSphere(mars.radius, SLICES, STACKS);       // mars venus
	glPopMatrix();
	
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
	glutPostRedisplay();
}


void idle() {
	sun.rotationSpin += sun.rotationSpeed;
	if (sun.rotationSpin > 360) sun.rotationSpin -= 360;

	mercury.rotationSpin += mercury.rotationSpeed;
	if (mercury.rotationSpin > 360) mercury.rotationSpin -= 360;
	mercury.orbitalSpin += mercury.orbitalSpeed;
	if (mercury.orbitalSpin > 360) mercury.orbitalSpin -= 360;

	venus.rotationSpin += venus.rotationSpeed;
	if (venus.rotationSpin > 360) venus.rotationSpin -= 360;
	venus.orbitalSpin += venus.orbitalSpeed;
	if (venus.orbitalSpin > 360) venus.orbitalSpin -= 360;

	earth.rotationSpin += earth.rotationSpeed;
	if (earth.rotationSpin > 360) earth.rotationSpin -= 360;
	earth.orbitalSpin += earth.orbitalSpeed;
	if (earth.orbitalSpin > 360) earth.orbitalSpin -= 360;

	moon.rotationSpin += moon.rotationSpeed;
	if (moon.rotationSpin > 360) moon.rotationSpin -= 360;
	moon.orbitalSpin += moon.orbitalSpeed;
	if (moon.orbitalSpin > 360) moon.orbitalSpin -= 360;

	mars.rotationSpin += mars.rotationSpeed;
	if (mars.rotationSpin > 360) mars.rotationSpin -= 360;
	mars.orbitalSpin += mars.orbitalSpeed;
	if (mars.orbitalSpin > 360) mars.orbitalSpin -= 360;

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