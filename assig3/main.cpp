#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <vector>
#include <cmath>
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
std::string targetObject = "None";

float transmittance = 0.5;
float reflectance = 0.5;
GLuint hairSimProg;
GLuint phongShadProg;

void loadTextures();
void lighting();
void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void motion(int, int);
void mouse(int, int, int, int);
void setShaders();
std::string textFileRead(const char *);
bool handleError(const char *, GLuint);

int main(int argc, char** argv) {
	meshes.push_back(new Mesh("Scalp.obj"));
	meshes.push_back(new Mesh("Sphere.obj"));
	view = new View("Peter.view");
	light = new Light("Peter.light");
	scene = new Scene("Peter.scene");

	glutInit(&argc, argv);
	glutInitWindowSize(
		view->viewport[0] + view->viewport[2],
		view->viewport[1] + view->viewport[3]
	);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ACCUM | GLUT_STENCIL);
	glutCreateWindow("Assignment 3");
	glewInit();
	FreeImage_Initialise();
	glGenTextures(128, textures);
	loadTextures();
	FreeImage_DeInitialise();
	setShaders();
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

			if (map[5].empty()) {
				glBindTexture(GL_TEXTURE_2D, textures[idx++]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);
				glTexImage2D(
					GL_TEXTURE_2D, 0, GL_RGBA8,
					FreeImage_GetWidth(bitmap2),
					FreeImage_GetHeight(bitmap2),
					0, GL_BGRA, GL_UNSIGNED_BYTE,
					FreeImage_GetBits(bitmap2)
				);
				glGenerateMipmap(GL_TEXTURE_2D);
				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			}

			else {
				switch (i) {
					case 0:
						glBindTexture(GL_TEXTURE_CUBE_MAP, textures[idx]);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
						                GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
						                GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
						glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 1000);
						glTexImage2D(
							GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8,
							FreeImage_GetWidth(bitmap2),
							FreeImage_GetHeight(bitmap2),
							0, GL_BGRA, GL_UNSIGNED_BYTE,
							FreeImage_GetBits(bitmap2)
						);
						break;
					case 1:
						glTexImage2D(
							GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8,
							FreeImage_GetWidth(bitmap2),
							FreeImage_GetHeight(bitmap2),
							0, GL_BGRA, GL_UNSIGNED_BYTE,
							FreeImage_GetBits(bitmap2)
						);
						break;
					case 2:
						glTexImage2D(
							GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8,
							FreeImage_GetWidth(bitmap2),
							FreeImage_GetHeight(bitmap2),
							0, GL_BGRA, GL_UNSIGNED_BYTE,
							FreeImage_GetBits(bitmap2)
						);
						break;
					case 3:
						glTexImage2D(
							GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8,
							FreeImage_GetWidth(bitmap2),
							FreeImage_GetHeight(bitmap2),
							0, GL_BGRA, GL_UNSIGNED_BYTE,
							FreeImage_GetBits(bitmap2)
						);
						break;
					case 4:
						glTexImage2D(
							GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8,
							FreeImage_GetWidth(bitmap2),
							FreeImage_GetHeight(bitmap2),
							0, GL_BGRA, GL_UNSIGNED_BYTE,
							FreeImage_GetBits(bitmap2)
						);
						break;
					case 5:
						glTexImage2D(
							GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8,
							FreeImage_GetWidth(bitmap2),
							FreeImage_GetHeight(bitmap2),
							0, GL_BGRA, GL_UNSIGNED_BYTE,
							FreeImage_GetBits(bitmap2)
						);
						glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
						glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
						idx++;
						break;
					default:
						break;
				}
			}

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
	for (int step = 0; step < 2; ++step) {
		auto mesh = meshes[step];

		switch (step) {
			case 0:
				glUseProgram(hairSimProg);
				break;

			case 1:
				glUseProgram(phongShadProg);
				glUniform1i(
					glGetUniformLocation(phongShadProg, "lightCount"),
					(GLint) light->terms.size()
				);
				break;

			default:
				glUseProgram(0);
				break;
		}

		for (size_t i = 0; i < mesh->fTotal; ++i) {
			int material = -1;
			auto range = scene->models.equal_range(mesh->objFile);

			if (material != mesh->fList[i].m) {
				material = mesh->fList[i].m;
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mesh->mList[material].Ka);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mesh->mList[material].Kd);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mesh->mList[material].Ks);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mesh->mList[material].Ns);
			}

			for (auto model = range.first; model != range.second; ++model) {
				int mapNum = -1;

				if (model->second[10] >= 0) {
					int idx = (int) model->second[10], tempIdx;
					mapNum = scene->maps[idx][5].empty() ? scene->maps[idx][1].empty() ? 1 : 2 : 6;

					switch (mapNum) {
						case 1:
							tempIdx = stoi(scene->maps[idx][0]);
							glEnable(GL_TEXTURE_2D);
							glBindTexture(GL_TEXTURE_2D, textures[tempIdx]);
							glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
							glDisable(GL_CULL_FACE);
							glEnable(GL_ALPHA_TEST);
							glAlphaFunc(GL_GREATER, 0.5);
							break;

						case 2:
							tempIdx = stoi(scene->maps[idx][0]);
							glActiveTexture(GL_TEXTURE0);
							glEnable(GL_TEXTURE_2D);
							glBindTexture(GL_TEXTURE_2D, textures[tempIdx]);
							glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
							glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

							tempIdx = stoi(scene->maps[idx][1]);
							glActiveTexture(GL_TEXTURE1);
							glEnable(GL_TEXTURE_2D);
							glBindTexture(GL_TEXTURE_2D, textures[tempIdx]);
							glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
							glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
							break;

						case 6:
							tempIdx = stoi(scene->maps[idx][0]);
							glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
							glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
							glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
							glEnable(GL_TEXTURE_GEN_S);
							glEnable(GL_TEXTURE_GEN_T);
							glEnable(GL_TEXTURE_GEN_R);
							glEnable(GL_TEXTURE_CUBE_MAP);
							glBindTexture(GL_TEXTURE_CUBE_MAP, textures[tempIdx]);
							glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
							break;

						default:
							break;
					}
				}

				glPushMatrix();

				glTranslatef(model->second[7], model->second[8], model->second[9]);
				glRotatef(model->second[3], model->second[4], model->second[5], model->second[6]);
				glScalef(model->second[0], model->second[1], model->second[2]);

				glBegin(GL_TRIANGLES);
				for (size_t j = 0; j < 3; ++j) {
					if (mapNum == 1)
						glTexCoord2fv(mesh->tList[mesh->fList[i][j].t].ptr);
					else if (mapNum == 2) {
						glMultiTexCoord2fv(GL_TEXTURE0, mesh->tList[mesh->fList[i][j].t].ptr);
						glMultiTexCoord2fv(GL_TEXTURE1, mesh->tList[mesh->fList[i][j].t].ptr);
					}
					glNormal3fv(mesh->nList[mesh->fList[i][j].n].ptr);
					glVertex3fv(mesh->vList[mesh->fList[i][j].v].ptr);
				}
				glEnd();

				glPopMatrix();

				if (model->second[10] >= 0) {
					switch (mapNum) {
						case 1:
							glBindTexture(GL_TEXTURE_2D, 0);
							glDisable(GL_TEXTURE_2D);
							break;
						case 2:
							glActiveTexture(GL_TEXTURE1);
							glBindTexture(GL_TEXTURE_2D, 0);
							glDisable(GL_TEXTURE_2D);
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, 0);
							glDisable(GL_TEXTURE_2D);
							break;
						case 6:
							glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
							glDisable(GL_TEXTURE_CUBE_MAP);
							glDisable(GL_TEXTURE_GEN_S);
							glDisable(GL_TEXTURE_GEN_T);
							glDisable(GL_TEXTURE_GEN_R);
							break;
						default:
							break;
					}
				}
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

	double dx = view->lookAt[0] - view->lookAt[3];
	double dy = view->lookAt[1] - view->lookAt[4];
	double dz = view->lookAt[2] - view->lookAt[5];
	double theta = std::atan2(dz, dx);
	double radius = std::hypot(dx, dz);

	double leftVec[3] = {
		radius * std::cos(theta + 0.1309) + view->lookAt[3],
		0 + view->lookAt[4],
		radius * std::sin(theta + 0.1309) + view->lookAt[5]
	};

	double rightVec[3] = {
		radius * std::cos(theta - 0.1309) + view->lookAt[3],
		0 + view->lookAt[4],
		radius * std::sin(theta - 0.1309) + view->lookAt[5]
	};

	switch (key) {
		case 'w':
			view->lookAt[0] -= dx * 0.1;
			view->lookAt[1] -= dy * 0.1;
			view->lookAt[2] -= dz * 0.1;
			glutPostRedisplay();
			break;
		case 's':
			view->lookAt[0] += dx * 0.1;
			view->lookAt[1] += dy * 0.1;
			view->lookAt[2] += dz * 0.1;
			glutPostRedisplay();
			break;
		case 'a':
			view->lookAt[0] = leftVec[0];
			view->lookAt[1] = leftVec[1];
			view->lookAt[2] = leftVec[2];
			glutPostRedisplay();
			break;
		case 'd':
			view->lookAt[0] = rightVec[0];
			view->lookAt[1] = rightVec[1];
			view->lookAt[2] = rightVec[2];
			glutPostRedisplay();
			break;
		case 'r':
			reflectance = reflectance >= 1.0f ? 1.0f : reflectance + 0.1f;
			glutPostRedisplay();
			break;
		case 'f':
			reflectance = reflectance <= 0.0f ? 0.0f : reflectance - 0.1f;
			glutPostRedisplay();
			break;
		case 't':
			transmittance = transmittance >= 1.0f ? 1.0f : transmittance + 0.1f;
			glutPostRedisplay();
			break;
		case 'g':
			transmittance = transmittance <= 0.0f ? 0.0f : transmittance - 0.1f;
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

void setShaders() {
	// information about current GL connection
	std::cout << "GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GL_SHADING_LANGUAGE_VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// hair simulation program
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint geomShader = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertSourze = textFileRead("hairSim.vert");
	std::string geomSourze = textFileRead("hairSim.geom");
	std::string fragSourze = textFileRead("hairSim.frag");

	const char *vertSource = vertSourze.c_str();
	const char *geomSource = geomSourze.c_str();
	const char *fragSource = fragSourze.c_str();

	glShaderSource(vertShader, 1, &vertSource, nullptr);
	glShaderSource(geomShader, 1, &geomSource, nullptr);
	glShaderSource(fragShader, 1, &fragSource, nullptr);

	glCompileShader(vertShader);
	handleError("hairSim.vert", vertShader);
	glCompileShader(geomShader);
	handleError("hairSim.geom", geomShader);
	glCompileShader(fragShader);
	handleError("hairSim.frag", fragShader);

	hairSimProg = glCreateProgram();

	glAttachShader(hairSimProg, vertShader);
	glAttachShader(hairSimProg, geomShader);
	glAttachShader(hairSimProg, fragShader);

	glLinkProgram(hairSimProg);

	// phong shading program
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	vertSourze = textFileRead("phongShad.vert");
	fragSourze = textFileRead("phongShad.frag");

	vertSource = vertSourze.c_str();
	fragSource = fragSourze.c_str();

	glShaderSource(vertShader, 1, &vertSource, nullptr);
	glShaderSource(fragShader, 1, &fragSource, nullptr);

	glCompileShader(vertShader);
	handleError("phongShad.vert", vertShader);
	glCompileShader(fragShader);
	handleError("phongShad.frag", fragShader);

	phongShadProg = glCreateProgram();

	glAttachShader(phongShadProg, vertShader);
	glAttachShader(phongShadProg, fragShader);

	glLinkProgram(phongShadProg);
}

std::string textFileRead(const char *filename) {
	std::ifstream fs(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (fs.is_open()) {
		std::ifstream::pos_type size = fs.tellg();
		fs.seekg(0, std::ios::beg);
		std::vector<char> buf((unsigned) size);
		fs.read(&buf[0], size);
		fs.close();
		return std::string(&buf[0], (unsigned) size);
	}
	else {
		std::cout << "cannot open " << filename << std::endl;
		return "";
	}
}

bool handleError(const char *filename, GLuint shader) {
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog((unsigned) maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		if (maxLength)
			std::cout << filename << ": " << &errorLog[0] << std::endl;
		else
			std::cout << filename << ": failed" << std::endl;
		glDeleteShader(shader);

		return false;
	}

	std::cout << filename << ": success" << std::endl;
	return true;
}
