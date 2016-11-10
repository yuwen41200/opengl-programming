#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <vector>
#include "MeshLoader.hpp"
#include "ViewLoader.hpp"
#include "LightLoader.hpp"
#include "SceneLoader.hpp"

std::vector<Mesh *> meshes;
View *view;
Light *light;
Scene *scene;

unsigned textures[128];
int windowSize[2] = {-1, -1};
int mouseLocation[2] = {-1, -1};
string targetObject = "None";

void loadTextures();
void lighting();
void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void motion(int, int);
void mouse(int, int, int, int);

int main(int argc, char** argv) {
	meshes.push_back(new Mesh("bush.obj"));
	meshes.push_back(new Mesh("gem.obj"));
	meshes.push_back(new Mesh("groundv2.obj"));
	meshes.push_back(new Mesh("hedge.obj"));
	meshes.push_back(new Mesh("leaves.obj"));
	meshes.push_back(new Mesh("LittleFountain.obj"));
	meshes.push_back(new Mesh("skybox.obj"));
	meshes.push_back(new Mesh("trunk.obj"));
	meshes.push_back(new Mesh("water.obj"));
	view = new View("park.view");
	light = new Light("park.light");
	scene = new Scene("park.scene");

	glutInit(&argc, argv);
	glutInitWindowSize(
		view->viewport[0] + view->viewport[2],
		view->viewport[1] + view->viewport[3]
	);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("Assignment 1 Phase 2");
	glewInit();
	FreeImage_Initialise();
	glGenTextures(128, textures);
	loadTextures();
	FreeImage_DeInitialise();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();

	delete scene;
	delete light;
	delete view;
	for (auto mesh: meshes)
		delete mesh;
	return 0;
}

void loadTextures() {
	int idx = 0;
	for (auto &map: scene->maps) {
		for (int i = 0; i < 6; ++i) {
			if (map[i].empty())
				break;

			FIBITMAP *bitmap1 = FreeImage_Load(
				FreeImage_GetFileType(map[i].c_str()),
				map[i].c_str()
			);
			FIBITMAP *bitmap2 = FreeImage_ConvertTo32Bits(bitmap1);
			map[i] = to_string(idx);

			glBindTexture(GL_TEXTURE_2D, textures[idx++]);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 31);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGBA8,
				FreeImage_GetWidth(bitmap2),
				FreeImage_GetHeight(bitmap2),
				0, GL_BGRA, GL_UNSIGNED_BYTE,
				FreeImage_GetBits(bitmap2)
			);
			glGenerateMipmap(GL_TEXTURE_2D);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

			FreeImage_Unload(bitmap2);
			FreeImage_Unload(bitmap1);
		}
	}
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

			if (range.first->second[10] >= 0) {
				int idx1 = (int) range.first->second[10];
				int idx2 = stoi(scene->maps[idx1][0]);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, textures[idx2]);
			}

			for (auto model = range.first; model != range.second; ++model) {
				glPushMatrix();

				glTranslatef(model->second[7], model->second[8], model->second[9]);
				glRotatef(model->second[3], model->second[4], model->second[5], model->second[6]);
				glScalef(model->second[0], model->second[1], model->second[2]);

				glBegin(GL_TRIANGLES);
				for (size_t j = 0; j < 3; ++j) {
					glTexCoord2fv(mesh->tList[mesh->fList[i][j].t].ptr);
					glNormal3fv(mesh->nList[mesh->fList[i][j].n].ptr);
					glVertex3fv(mesh->vList[mesh->fList[i][j].v].ptr);
				}
				glEnd();

				glPopMatrix();
			}

			if (range.first->second[10] >= 0) {
				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);
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
			if ('1' <= key && key <= '9') {
				try {
					auto mesh = meshes.at(key - '1');
					targetObject = mesh->objFile;
				}
				catch (std::out_of_range e) {
					targetObject = "None";
				}
			}
			break;
	}
}

void motion(int dx, int dy) {
	std::cout << "motion(" << dx << ", " << dy << ")" << std::endl;
	auto range = scene->models.equal_range(targetObject);

	for (auto model = range.first; model != range.second; ++model) {
		if (dx > 25)
			model->second[7]++;
		if (dx < 25)
			model->second[7]--;
		if (dy > 25)
			model->second[8]--;
		if (dy < 25)
			model->second[8]++;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseLocation[0] = x;
			mouseLocation[1] = y;
		}
		else if (state == GLUT_UP) {
			int dx = x - mouseLocation[0];
			int dy = y - mouseLocation[1];
			motion(dx, dy);
		}
	}
}
