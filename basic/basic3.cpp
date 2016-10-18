#include <ctime>
#include <GL/glut.h>

void display();
void reshape(GLsizei, GLsizei);
void idle();
void keyboard(unsigned char, int, int);

int width, height;
float year = 0, day = 0;
double time_prev, time_curr;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Basic 3");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

void display() {
	// OpenGL Documentation:
	// https://www.opengl.org/sdk/docs/man2/

	// initialize buffers
	// apply hidden surface removal
	glClearColor(0.258, 0.523, 0.953, 0);
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// viewing transformation
	// matrix modes: GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 2, 6, 0, 0, 0, 0, 1, 0);
	// glLoadMatrixf(array_of_16_values);
	// glMultMatrixf(array_of_16_values);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (float) width / height, 1, 20);
	// glFrustum(-1, 1, -1, 1, 1, 20);
	// glOrtho(-5, 5, -5, 5, -5, 5);
	// gluOrtho2D(-5, 5, -5, 5);

	// viewport transformation
	// relative to the lower left corner
	glViewport(0, 0, width, height);

	// modeling transformation
	// (a) draw centre sphere
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1, 1, 0.5);
	glPushMatrix();
	glRotatef(year * 0.5f, 0, 0, 1);
	glutWireSphere(1, 24, 24);
	glPopMatrix();

	// (b) draw outer sphere
	glColor3f(1, 1, 1);
	glPushMatrix();
	glRotatef(year, 0, 1, 0);
	glTranslatef(3.2, 0, 0);
	glRotatef(day, 1, 1, 0);
	glutWireSphere(0.5, 16, 16);
	glPopMatrix();

	// (c) draw inner sphere
	// execution order is inverted
	glPushMatrix();
	glRotatef(year * 2, 0, 1, 0);
	glTranslatef(1.6, 0, 0);
	glRotatef(day * 2, 1, 1, 0);
	glScalef(1, 1, 1.2);
	glutWireSphere(0.2, 8, 8);
	glPopMatrix();

	// glFlush() will not work
	glutSwapBuffers();
}

void reshape(GLsizei _width, GLsizei _height) {
	width = _width;
	height = _height;
}

void idle() {
	time_curr = clock();
	day += 10000 * (time_curr - time_prev) / CLOCKS_PER_SEC;
	day = day > 360 ? day - 360 : day;
	year += 1000 * (time_curr - time_prev) / CLOCKS_PER_SEC;
	year = year > 360 ? year - 360 : year;
	time_prev = time_curr;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'a':
			day += 20;
			day = day > 360 ? day - 360 : day;
			glutPostRedisplay();
			break;
		case 's':
			year += 2;
			year = year > 360 ? year - 360 : year;
			glutPostRedisplay();
			break;
		case 'd':
			time_curr = clock();
			glutIdleFunc(idle);
			break;
		case 'f':
			glutIdleFunc(nullptr);
			break;
		case 'q':
			exit(0);
		default:
			break;
	}
}
