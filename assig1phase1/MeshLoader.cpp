// Copyright Disclaimer: This code (MeshLoader.cpp) is based on TA's code.

#include <iostream>
#include <cstring>
#include <map>
#include <vector>

using namespace std;

class Mesh {

	class Material {
	public:
		float Ka[4]; // ambient
		float Kd[4]; // diffuse
		float Ks[4]; // specular
		float Ns; // shininess
		float Tr; // transparent
		std::string map_Ka; // ambient texture
		std::string map_Kd; // diffuse texture
		std::string map_Ks; // specular texture

		Material() {
			for (int i = 0; i < 4; i++)
				Ka[i] = Kd[i] = Ks[i] = 1;
			Ns = Tr = 0;
		}
	};

	class Vertex {
	public:
		size_t v; // vertex index in vList
		size_t n; // normal index in nList
		size_t t; // texture index in tList

		Vertex() {
			v = n = t = 0;
		}

		Vertex(size_t v_index, size_t n_index, size_t t_index = 0) {
			v = v_index;
			n = n_index;
			t = t_index;
		}
	};

	class Vec3 {
	public:
		float ptr[3];

		Vec3 (float *v) {
			for (int i = 0; i < 3; i++)
				ptr[i] = v[i];
		}

		inline float& operator[](size_t index) {
			return ptr[index];
		}
	};

	class Face {
	public:
		Vertex v[3]; // each face has three vertices
		int m; // material index in mList

		Face (Vertex &v1, Vertex &v2, Vertex &v3, int m_index) {
			v[0] = v1;
			v[1] = v2;
			v[2] = v3;
			m = m_index;
		}

		inline Vertex& operator[](size_t index) {
			return v[index];
		}
	};

public:
	std::map<std::string, size_t> mMap; // { material name: material index } map
	std::vector<Material> mList; // material list
	std::vector<Vec3> vList; // vertex list
	std::vector<Vec3> nList; // normal list
	std::vector<Vec3> tList; // texture list
	std::vector<Face> fList; // face list
	size_t mTotal, vTotal, nTotal, tTotal, fTotal; // total materials, vertices, normals, textures, faces
	std::string mtlFile;
	std::string objFile;

	Mesh();
	Mesh(const char *obj_file);
	void init(const char *obj_file);
	void loadMesh(const char *obj_file);
	void loadMaterial(const char *mtl_file);

};

Mesh::Mesh() {
	mTotal = vTotal = nTotal = tTotal = fTotal = 0;
}

Mesh::Mesh(const char* obj_file) {
	mTotal = vTotal = nTotal = tTotal = fTotal = 0;
	init(obj_file);
}

void Mesh::init(const char *obj_file) {
	float default_value[3] = {1, 1, 1};
	Material default_material;

	vList.push_back(Vec3(default_value));
	nList.push_back(Vec3(default_value));
	tList.push_back(Vec3(default_value));
	mList.push_back(default_material);

	mTotal++; // mList[0] is default material

	mList[0].Ka[0] = 0.0f;
	mList[0].Ka[1] = 0.0f;
	mList[0].Ka[2] = 0.0f;
	mList[0].Ka[3] = 1.0f;
	mList[0].Kd[0] = 1.0f;
	mList[0].Kd[1] = 1.0f;
	mList[0].Kd[2] = 1.0f;
	mList[0].Kd[3] = 1.0f;
	mList[0].Ks[0] = 0.8f;
	mList[0].Ks[1] = 0.8f;
	mList[0].Ks[2] = 0.8f;
	mList[0].Ks[3] = 1.0f;
	mList[0].Ns = 32.0f;

	loadMesh(obj_file);
}

void Mesh::loadMesh(const char *obj_file) {
	size_t curr_mtl = 0, n_vertex, n_texture, n_normal;

	FILE *scene = fopen(obj_file, "r");
	objFile = obj_file;

	if (!scene) {
		cout << "cannot open object file \"" << obj_file << "\"" << endl;
		return;
	}

	cout << obj_file << endl;

	while (!feof(scene)) {
		char token[100], buf[100], mtl_file[256], v[5][100];
		float vec[3] = {0, 0, 0};

		token[0] = 0;
		fscanf(scene, "%s", token);

		if (!strcmp(token, "g")) {
			fscanf(scene, "%s", buf);
		}

		else if (!strcmp(token, "mtllib")) {
  			fscanf(scene, "%s", mtl_file);
			loadMaterial(mtl_file);
		}

		else if (!strcmp(token, "usemtl")) {
			fscanf(scene, "%s", buf);
			curr_mtl = mMap[objFile + "_" + buf];
		}

		else if (!strcmp(token, "v")) {
			fscanf(scene, "%f %f %f", &vec[0], &vec[1], &vec[2]);
			vList.push_back(Vec3(vec));
		}

		else if (!strcmp(token, "vn")) {
			fscanf(scene, "%f %f %f", &vec[0], &vec[1], &vec[2]);
			nList.push_back(Vec3(vec));
		}

		else if (!strcmp(token, "vt")) {
			fscanf(scene, "%f %f", &vec[0], &vec[1]);
			tList.push_back(Vec3(vec));
		}

		else if (!strcmp(token, "f")) {
			Vertex tmp_vertex[3];

			for (size_t i = 0; i < 3; i++)
				fscanf(scene, "%s", v[i]);

			for (size_t i = 0; i < 3; i++) {
				char str[20], ch;
				size_t base, offset;
				base = offset = 0;

				// vertex index in vList
				while ((ch = v[i][base+offset]) != '/' && (ch = v[i][base+offset]) != '\0') {
					str[offset] = ch;
					offset++;
				}

				str[offset] = '\0';
				n_vertex = (size_t) atoi(str);
				base += ch == '\0' ? offset : offset + 1;
				offset = 0;

				// texture index in tList
				while ((ch = v[i][base+offset]) != '/' && (ch = v[i][base+offset]) != '\0') {
					str[offset] = ch;
					offset++;
				}

				str[offset] = '\0';
				n_texture = (size_t) atoi(str);
				base += ch == '\0' ? offset : offset+1;
				offset = 0;

				// normal index in nList
				while ((ch = v[i][base+offset]) != '\0') {
					str[offset] = ch;
					offset++;
				}

				str[offset] = '\0';
				n_normal = (size_t) atoi(str);

				tmp_vertex[i].v = n_vertex;
				tmp_vertex[i].t = n_texture;
				tmp_vertex[i].n = n_normal;
			}

			fList.push_back(Face(tmp_vertex[0], tmp_vertex[1], tmp_vertex[2], (int) curr_mtl));
		}

		else if (!strcmp(token, "#"))
			fgets(buf, 100, scene);
	}

	fclose(scene);

	vTotal = vList.size();
	nTotal = nList.size();
	tTotal = tList.size();
	fTotal = fList.size();
	printf("%zu vertices, %zu normals, %zu textures, %zu faces\n", vTotal, nTotal, tTotal, fTotal);
}

void Mesh::loadMaterial(const char *mtl_file) {
	char token[100], buf[100];
	float r, g, b;
	size_t curr_mtl = 0;

	FILE *fp_mtl = fopen(mtl_file, "r");
	mtlFile = mtl_file;

	if (!fp_mtl) {
		cout << "cannot open material file \"" << mtl_file << "\"" << endl;
		return;
	}

	cout << mtl_file << endl;

	while (!feof(fp_mtl)) {
		token[0] = 0;
		fscanf(fp_mtl, "%s", token);

		if (!strcmp(token, "newmtl")) {
			fscanf(fp_mtl, "%s", buf);
			Material new_mtl;
			mList.push_back(new_mtl);
			curr_mtl = mTotal++;
			mMap[objFile + "_" + buf] = curr_mtl;
		}

		else if (!strcmp(token, "Ka")) {
			fscanf(fp_mtl, "%f %f %f", &r, &g, &b);
			mList[curr_mtl].Ka[0] = r;
			mList[curr_mtl].Ka[1] = g;
			mList[curr_mtl].Ka[2] = b;
			mList[curr_mtl].Ka[3] = 1;
		}

		else if (!strcmp(token, "Kd")) {
			fscanf(fp_mtl, "%f %f %f", &r, &g, &b);
			mList[curr_mtl].Kd[0] = r;
			mList[curr_mtl].Kd[1] = g;
			mList[curr_mtl].Kd[2] = b;
			mList[curr_mtl].Kd[3] = 1;
		}

		else if (!strcmp(token, "Ks")) {
			fscanf(fp_mtl, "%f %f %f", &r, &g, &b);
			mList[curr_mtl].Ks[0] = r;
			mList[curr_mtl].Ks[1] = g;
			mList[curr_mtl].Ks[2] = b;
			mList[curr_mtl].Ks[3] = 1;
		}

		else if (!strcmp(token, "Ns")) {
			fscanf(fp_mtl, "%f", &r);
			mList[curr_mtl].Ns = r;
		}

		else if (!strcmp(token, "Tr")) {
			fscanf(fp_mtl, "%f", &r);
			mList[curr_mtl].Tr = r;
		}

		else if (!strcmp(token, "d")) {
			fscanf(fp_mtl, "%f", &r);
			mList[curr_mtl].Tr = r;
		}

		else if (!strcmp(token, "map_Kd")) {
			fscanf(fp_mtl, "%s", buf);
			mList[curr_mtl].map_Kd = string(buf);
		}

		else if (!strcmp(token, "map_Ks")) {
			fscanf(fp_mtl, "%s", buf);
			mList[curr_mtl].map_Ks = string(buf);
		}

		else if (!strcmp(token, "map_Ka")) {
			fscanf(fp_mtl, "%s", buf);
			mList[curr_mtl].map_Ka = string(buf);
		}

		else if (!strcmp(token, "#"))
			fgets(buf, 100, fp_mtl);
	}

	fclose(fp_mtl);

	printf("%ld materials\n", mMap.size());
}
