#include <GL/glut.h>

void display();
void reshape(GLsizei, GLsizei);

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Basic 2");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

void display() {
	glClearColor(0, 0, 0, 0);
	// glClearDepth(0);
	glClear(GL_COLOR_BUFFER_BIT);
	// Available masks:
	//   GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT
	//   GL_ACCUM_BUFFER_BIT, GL_STENCIL_BUFFER_BIT
	glBegin(GL_POLYGON);
	// Available modes:
	//   GL_POINTS, GL_LINES, GL_TRIANGLES
	//   GL_QUADS, GL_POLYGON, etc.

	// white point at lower left corner
	glColor3f(1, 1, 1);
	glVertex3f(-3, -3, 0);
	// red point at lower right corner
	glColor3f(1, 0, 0);
	glVertex3f(3, -3, 0);
	// green point at upper right corner
	glColor3f(0, 1, 0);
	glVertex3f(3, 3, 0);
	// blue point at upper left corner
	glColor3f(0, 0, 1);
	glVertex3f(-3, 3, 0);
	// black point at middle
	glColor3f(0, 0, 0);
	glVertex3f(0, 0, 0);
	// Suffixes:
	//   b: byte, s: short, i: int
	//   f: float, d: double, u: unsigned

	glEnd();
	// close glBegin()
	glFlush();
	// glFinish();
}

void reshape(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5, 5, -5, 5, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
