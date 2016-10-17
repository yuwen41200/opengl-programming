#include <GL/glut.h>

void display();
void reshape(GLsizei, GLsizei);

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	// Available modes:
	//   GLUT_SINGLE, GLUT_DOUBLE
	//   GLUT_RGBA, GLUT_RGB, GLUT_INDEX
	//   GLUT_DEPTH, GLUT_STENCIL, GLUT_ACCUM
	// glutInitWindowSize(500, 500);
	// glutInitWindowPosition(100, 100);
	glutCreateWindow("Basic 1");
	glutDisplayFunc(display);
	// glutPostRedisplay();
	// glutSwapBuffers();
	glutReshapeFunc(reshape);
	// glutKeyboardFunc(callback);
	// glutMouseFunc(callback);
	// glutMotionFunc(callback);
	// glutPassiveMotionFunc(callback);
	// glutIdleFunc(callback);
	glutMainLoop();
	return 0;
}

void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glutSolidTeapot(1);
	// glutWireTeapot(1);
	// glutSolidSphere(1, 1, 1);
	// glutSolidCube(1);
	// glutSolidCone(1, 1, 1, 1);
	// glutSolidTorus(1, 1, 1, 1);
	glFlush();
}

void reshape(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.5, 0.5, -0.5, 0.5, 1, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}
