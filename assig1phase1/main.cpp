#include <GL/glut.h>
#include "MeshLoader.cpp"
#include "ViewLoader.cpp"
#include "LightLoader.cpp"

Mesh *mesh;
View *view;
Light *light;
int windowSize[2];

void lighting();
void display();
void reshape(GLsizei, GLsizei);

int main(int argc, char** argv) {
	mesh = new Mesh("box.obj");
	view = new View("view.view");
	light = new Light("light.light");

	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Assignment 1 Phase 1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	delete light;
	delete view;
	delete mesh;
	return 0;
}

void lighting() {
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT_MODEL_AMBIENT);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light->ambient);

	switch (light->terms.size()) {
		case 8:
			glEnable(GL_LIGHT7);
			glLightfv(GL_LIGHT7, GL_DIFFUSE, light->terms[7].diffuse);
			glLightfv(GL_LIGHT7, GL_SPECULAR, light->terms[7].specular);
			glLightfv(GL_LIGHT7, GL_AMBIENT, light->terms[7].ambient);
			glLightfv(GL_LIGHT7, GL_POSITION, light->terms[7].position);
		case 7:
			glEnable(GL_LIGHT6);
			glLightfv(GL_LIGHT6, GL_DIFFUSE, light->terms[6].diffuse);
			glLightfv(GL_LIGHT6, GL_SPECULAR, light->terms[6].specular);
			glLightfv(GL_LIGHT6, GL_AMBIENT, light->terms[6].ambient);
			glLightfv(GL_LIGHT6, GL_POSITION, light->terms[6].position);
		case 6:
			glEnable(GL_LIGHT5);
			glLightfv(GL_LIGHT5, GL_DIFFUSE, light->terms[5].diffuse);
			glLightfv(GL_LIGHT5, GL_SPECULAR, light->terms[5].specular);
			glLightfv(GL_LIGHT5, GL_AMBIENT, light->terms[5].ambient);
			glLightfv(GL_LIGHT5, GL_POSITION, light->terms[5].position);
		case 5:
			glEnable(GL_LIGHT4);
			glLightfv(GL_LIGHT4, GL_DIFFUSE, light->terms[4].diffuse);
			glLightfv(GL_LIGHT4, GL_SPECULAR, light->terms[4].specular);
			glLightfv(GL_LIGHT4, GL_AMBIENT, light->terms[4].ambient);
			glLightfv(GL_LIGHT4, GL_POSITION, light->terms[4].position);
		case 4:
			glEnable(GL_LIGHT3);
			glLightfv(GL_LIGHT3, GL_DIFFUSE, light->terms[3].diffuse);
			glLightfv(GL_LIGHT3, GL_SPECULAR, light->terms[3].specular);
			glLightfv(GL_LIGHT3, GL_AMBIENT, light->terms[3].ambient);
			glLightfv(GL_LIGHT3, GL_POSITION, light->terms[3].position);
		case 3:
			glEnable(GL_LIGHT2);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, light->terms[2].diffuse);
			glLightfv(GL_LIGHT2, GL_SPECULAR, light->terms[2].specular);
			glLightfv(GL_LIGHT2, GL_AMBIENT, light->terms[2].ambient);
			glLightfv(GL_LIGHT2, GL_POSITION, light->terms[2].position);
		case 2:
			glEnable(GL_LIGHT1);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, light->terms[1].diffuse);
			glLightfv(GL_LIGHT1, GL_SPECULAR, light->terms[1].specular);
			glLightfv(GL_LIGHT1, GL_AMBIENT, light->terms[1].ambient);
			glLightfv(GL_LIGHT1, GL_POSITION, light->terms[1].position);
		case 1:
			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light->terms[0].diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light->terms[0].specular);
			glLightfv(GL_LIGHT0, GL_AMBIENT, light->terms[0].ambient);
			glLightfv(GL_LIGHT0, GL_POSITION, light->terms[0].position);
		default:
			break;
	}
}

void display() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// viewport transformation
	glViewport(
		view->viewport[0],
		view->viewport[1],
		view->viewport[2],
		view->viewport[3]
	);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		view->perspective[0],
		(GLdouble) windowSize[0] / windowSize[1],
		view->perspective[1],
		view->perspective[2]
	);

	// viewing transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		view->lookAt[0],
		view->lookAt[1],
		view->lookAt[2],
		view->lookAt[3],
		view->lookAt[4],
		view->lookAt[5],
		view->lookAt[6],
		view->lookAt[7],
		view->lookAt[8]
	);

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
