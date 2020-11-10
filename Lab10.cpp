#include <GL/glut.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define SLICES 30 // Sphere 세로 방향의 선의 갯수(경도)
#define STACKS 30 // Sphere 가로 방향의 선의 갯수(위도)
#define CAMERA_LIGHT GL_LIGHT0
#define SUN_LIGHT GL_LIGHT1
#define EARTH_LIGHT GL_LIGHT2
#define MOON_LIGHT GL_LIGHT3
#define ARBITRARY_LIGHT GL_LIGHT4

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
Planet earth(1.7, 0.2, 0.0, 0.03, 0.0);      // 지구
Planet moon(0.5, 0.2, 0.0, 0.2, 0.0);        // 달

int current_light = CAMERA_LIGHT;

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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
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
	glEnable(GL_DEPTH_TEST);

	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(CAMERA_LIGHT, GL_AMBIENT, light_ambient);
	glLightfv(CAMERA_LIGHT, GL_DIFFUSE, light_diffuse);
	glLightfv(CAMERA_LIGHT, GL_SPECULAR, light_specular);
	glLightfv(SUN_LIGHT, GL_AMBIENT, light_ambient);
	glLightfv(SUN_LIGHT, GL_DIFFUSE, light_diffuse);
	glLightfv(SUN_LIGHT, GL_SPECULAR, light_specular);
	glLightfv(EARTH_LIGHT, GL_AMBIENT, light_ambient);
	glLightfv(EARTH_LIGHT, GL_DIFFUSE, light_diffuse);
	glLightfv(EARTH_LIGHT, GL_SPECULAR, light_specular);
	glLightfv(MOON_LIGHT, GL_AMBIENT, light_ambient);
	glLightfv(MOON_LIGHT, GL_DIFFUSE, light_diffuse);
	glLightfv(MOON_LIGHT, GL_SPECULAR, light_specular);
	glLightfv(ARBITRARY_LIGHT, GL_AMBIENT, light_ambient);
	glLightfv(ARBITRARY_LIGHT, GL_DIFFUSE, light_diffuse);
	glLightfv(ARBITRARY_LIGHT, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHTING);
	glEnable(CAMERA_LIGHT);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	GLfloat specular_material[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_material);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);
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

	gluLookAt(30, 30, 30, 0, 0, 0, 0, 1, 0);           // viewing transformation
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat emission[] = { 0.0, 0.0, 0.0, 1.0 };
	
	if (current_light == CAMERA_LIGHT) {
		light_position[0] = 30; light_position[1] = 30; light_position[2] = 30;
		glLightfv(CAMERA_LIGHT, GL_POSITION, light_position);
		light_position[0] = 0; light_position[1] = 0; light_position[2] = 0;
	}

	glPushMatrix();
	glRotatef(sun.rotationSpin, 0, 1, 0);              // sun rotation
	glLightfv(SUN_LIGHT, GL_POSITION, light_position);
	if (current_light == SUN_LIGHT) {
		emission[0] = 1.0; emission[1] = 0.0; emission[2] = 0.0;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(sun.radius, SLICES, STACKS);       // draw sun
	if (current_light == SUN_LIGHT) {
		emission[0] = 0.0;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}
	glPopMatrix();

	glPushMatrix();
	glRotatef(earth.orbitalSpin, 0, 1, 0);             // earth orbital
	glTranslatef(sun.radius + 12, 0, 0);
	glPushMatrix();
	glRotatef(earth.rotationSpin, 0, 1, 0);            // earth rotation
	glLightfv(EARTH_LIGHT, GL_POSITION, light_position);
	if (current_light == EARTH_LIGHT) {
		emission[0] = 0.0; emission[1] = 0.0; emission[2] = 1.0;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}
	glColor3f(0.0, 0.0, 1.0);
	glutSolidSphere(earth.radius, SLICES, STACKS);     // draw earth
	if (current_light == EARTH_LIGHT) {
		emission[2] = 0.0;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}
	glPopMatrix();

	glRotatef(moon.orbitalSpin, 0, 1, 0);              // moon orbital
	glTranslatef(earth.radius + 1, 0, 0);
	glRotatef(moon.rotationSpin, 0, 1, 0);             // moon rotation
	glLightfv(MOON_LIGHT, GL_POSITION, light_position);
	if (current_light == MOON_LIGHT) {
		emission[0] = 0.5; emission[1] = 0.5; emission[2] = 0.5;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}
	glColor3f(0.5, 0.5, 0.5);
	glutSolidSphere(moon.radius, SLICES, STACKS);      //draw moon
	if (current_light == MOON_LIGHT) {
		emission[0] = 0.0; emission[1] = 0.0; emission[2] = 0.0;
		glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	}
	glPopMatrix();

	if (current_light == ARBITRARY_LIGHT) {
		light_position[2] = 30;
		glLightfv(ARBITRARY_LIGHT, GL_POSITION, light_position);
		light_position[2] = 0;
	}

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
	switch (key) {
	case '0':
		current_light = CAMERA_LIGHT;
		glDisable(SUN_LIGHT); glDisable(EARTH_LIGHT);
		glDisable(MOON_LIGHT); glDisable(ARBITRARY_LIGHT);
		glEnable(CAMERA_LIGHT);
		break;
	case '1':
		current_light = SUN_LIGHT;
		glDisable(CAMERA_LIGHT); glDisable(EARTH_LIGHT);
		glDisable(MOON_LIGHT); glDisable(ARBITRARY_LIGHT);
		glEnable(SUN_LIGHT);
		break;
	case '2':
		current_light = EARTH_LIGHT;
		glDisable(CAMERA_LIGHT); glDisable(SUN_LIGHT);
		glDisable(MOON_LIGHT); glDisable(ARBITRARY_LIGHT);
		glEnable(EARTH_LIGHT);
		break;
	case '3':
		current_light = MOON_LIGHT;
		glDisable(CAMERA_LIGHT); glDisable(SUN_LIGHT);
		glDisable(EARTH_LIGHT); glDisable(ARBITRARY_LIGHT);
		glEnable(MOON_LIGHT);
		break;
	case '4':
		current_light = ARBITRARY_LIGHT;
		glDisable(CAMERA_LIGHT); glDisable(SUN_LIGHT);
		glDisable(EARTH_LIGHT); glDisable(MOON_LIGHT);
		glEnable(ARBITRARY_LIGHT);
		break;
	}
	glutPostRedisplay();
}


void idle() {
	sun.rotationSpin += sun.rotationSpeed;
	if (sun.rotationSpin > 360) sun.rotationSpin -= 360;

	earth.rotationSpin += earth.rotationSpeed;
	if (earth.rotationSpin > 360) earth.rotationSpin -= 360;
	earth.orbitalSpin += earth.orbitalSpeed;
	if (earth.orbitalSpin > 360) earth.orbitalSpin -= 360;

	moon.rotationSpin += moon.rotationSpeed;
	if (moon.rotationSpin > 360) moon.rotationSpin -= 360;
	moon.orbitalSpin += moon.orbitalSpeed;
	if (moon.orbitalSpin > 360) moon.orbitalSpin -= 360;

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