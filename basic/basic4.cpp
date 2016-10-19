// OpenGL Documentation:
// https://www.opengl.org/sdk/docs/man2/
// GLUT Documentation:
// https://www.opengl.org/resources/libraries/glut/spec3/spec3.html

#include <GL/glut.h>

void display();
void reshape(GLsizei, GLsizei);
void lighting();

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Basic 4");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	lighting();
	glutSolidSphere(1, 256, 256);
	// glEnable(GL_NORMALIZE);
	// glNormal3f(0, 0, -1);
	// glVertex3f(2, 6, 2);
	glFlush();
}

void reshape(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.2, 1.2, -1.2, 1.2, -1.2, 1.2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void lighting() {
	float material_diffuse[] = {1, 0.2, 0.2, 1};
	float material_specular[] = {0.2, 1, 0.2, 1};
	float material_ambient[] = {0.2, 0.2, 1, 1};
	float material_emission[] = {0.1, 0.1, 0.1, 1};

	float light_diffuse[] = {1, 1, 1, 1};
	float light_specular[] = {1, 1, 1, 1};
	float light_ambient[] = {0.1, 0.1, 0.1, 1};
	float light_position[] = {4, 4, 10, 1};

	glShadeModel(GL_SMOOTH);
	// Available modes:
	//   GL_FLAT, GL_SMOOTH

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	// Available parameter names:
	//   GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR, GL_POSITION
	//   GL_SPOT_DIRECTION, GL_SPOT_EXPONENT, GL_SPOT_CUTOFF
	//   GL_CONSTANT_ATTENUATION, GL_LINEAR_ATTENUATION, GL_QUADRATIC_ATTENUATION
	// glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
	// glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2);
	// glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, 20);
	// Available faces:
	//   GL_FRONT, GL_BACK, GL_FRONT_AND_BACK
}
