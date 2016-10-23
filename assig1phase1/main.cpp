#include <GL/glut.h>
#include <vector>
#include "MeshLoader.cpp"
#include "ViewLoader.cpp"
#include "LightLoader.cpp"
#include "SceneLoader.cpp"

std::vector<Mesh *> meshes;
View *view;
Light *light;
Scene *scene;

int windowSize[2] = {-1, -1};
int mouseLocation[2] = {-1, -1};

void lighting();
void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void motion(int, int);
void mouse(int, int, int, int);

int main(int argc, char** argv) {
	meshes.push_back(new Mesh("redbox.obj")); // box.obj
	meshes.push_back(new Mesh("yellowbox.obj")); // venus.obj
	meshes.push_back(new Mesh("bluebox.obj")); // bunny.obj
	view = new View("scene2.view"); // view.view
	light = new Light("scene2.light"); // light.light
	scene = new Scene("scene2.scene"); // scene.scene

	glutInit(&argc, argv);
	glutInitWindowSize(
		view->viewport[0] + view->viewport[2],
		view->viewport[1] + view->viewport[3]
	);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Assignment 1 Phase 1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	glutMainLoop();

	delete scene;
	delete light;
	delete view;
	for (auto mesh: meshes)
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
	glEnable(GL_NORMALIZE);
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
		(GLdouble) view->viewport[2] / view->viewport[3],
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
	for (auto mesh: meshes) {
		for (size_t i = 0; i < mesh->fTotal; ++i) {
			int material = -1;
			auto range = scene->models.equal_range(mesh->objFile);

			if (material != mesh->fList[i].m) {
				material = mesh->fList[i].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT, mesh->mList[material].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mesh->mList[material].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR, mesh->mList[material].Ks);
				glMaterialf(GL_FRONT, GL_SHININESS, mesh->mList[material].Ns);
			}

			for (auto model = range.first; model != range.second; ++model) {
				glPushMatrix();

				glTranslatef(model->second[7], model->second[8], model->second[9]);
				glRotatef(model->second[3], model->second[4], model->second[5], model->second[6]);
				glScalef(model->second[0], model->second[1], model->second[2]);

				glBegin(GL_TRIANGLES);
				for (size_t j = 0; j < 3; ++j) {
					glNormal3fv(mesh->nList[mesh->fList[i][j].n].ptr);
					glVertex3fv(mesh->vList[mesh->fList[i][j].v].ptr);
				}
				glEnd();

				glPopMatrix();
			}
		}
	}

	glutSwapBuffers();
}

void reshape(int width, int height) {
	windowSize[0] = width;
	windowSize[1] = height;
}

void keyboard(unsigned char key, int x, int y) {
	mouseLocation[0] = x;
	mouseLocation[1] = y;

	double sightVec[3] = {
		view->lookAt[0] - view->lookAt[3],
		view->lookAt[1] - view->lookAt[4],
		view->lookAt[2] - view->lookAt[5]
	};
	double upVec[3] = {
		view->lookAt[6],
		view->lookAt[7],
		view->lookAt[8]
	};
	double rightVec[3] = {
		(sightVec[1] * upVec[2] - sightVec[2] * upVec[1]) / 100,
		(sightVec[2] * upVec[0] - sightVec[0] * upVec[2]) / 100,
		(sightVec[0] * upVec[1] - sightVec[1] * upVec[0]) / 100
	};

	switch (key) {
		case 'w':
			view->lookAt[0] -= 0.5;
			view->lookAt[1] -= 0.5;
			view->lookAt[2] -= 0.5;
			glutPostRedisplay();
			break;
		case 's':
			view->lookAt[0] += 0.5;
			view->lookAt[1] += 0.5;
			view->lookAt[2] += 0.5;
			glutPostRedisplay();
			break;
		case 'a':
			view->lookAt[0] += rightVec[0];
			view->lookAt[1] += rightVec[1];
			view->lookAt[2] += rightVec[2];
			view->lookAt[3] += rightVec[0];
			view->lookAt[4] += rightVec[1];
			view->lookAt[5] += rightVec[2];
			glutPostRedisplay();
			break;
		case 'd':
			view->lookAt[0] -= rightVec[0];
			view->lookAt[1] -= rightVec[1];
			view->lookAt[2] -= rightVec[2];
			view->lookAt[3] -= rightVec[0];
			view->lookAt[4] -= rightVec[1];
			view->lookAt[5] -= rightVec[2];
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

void motion(int x, int y) {
	// dx = x - mouseLocation[0]
	// dy = y - mouseLocation[1]
	// TODO
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouseLocation[0] = x;
		mouseLocation[1] = y;
	}
}
