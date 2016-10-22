#include <GL/glut.h>
#include "MeshLoader.cpp"

Mesh *mesh;
int windowSize[2];

void lighting();
void display();
void reshape(GLsizei, GLsizei);

int main(int argc, char** argv) {
	mesh = new Mesh("box.obj");
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Assignment 1 Phase 1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	delete mesh;
	return 0;
}

void lighting() {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat light_position[] = {150.0, 150.0, 150.0, 1.0};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void display() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLdouble) windowSize[0] / windowSize[1], 1, 1000);

	// viewing transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(250, 150, 150, 0, 0, 0, 0, 1, 0);

	// Always call lighting() after gluLookAt()
	lighting();

	// modeling transformation
	for (size_t i = 0; i < mesh->fTotal; ++i) {
		int material = -1;

		if (material != mesh->fList[i].m) {
			material = mesh->fList[i].m;
			glMaterialfv(GL_FRONT, GL_AMBIENT, mesh->mList[material].Ka);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mesh->mList[material].Kd);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mesh->mList[material].Ks);
			glMaterialf(GL_FRONT, GL_SHININESS, mesh->mList[material].Ns);
		}

		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < 3; ++j) {
			glNormal3fv(mesh->nList[mesh->fList[i][j].n].ptr);
			glVertex3fv(mesh->vList[mesh->fList[i][j].v].ptr);
		}
		glEnd();
	}

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	windowSize[0] = w;
	windowSize[1] = h;
}
