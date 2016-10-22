#include <iostream>
#include <fstream>
#include <map>
#include <array>

using namespace std;

class Scene {

public:
	Scene(const char *);
	map<string, array<float, 10>> models;

};

Scene::Scene(const char *scene_file) {
	ifstream fs(scene_file);
	string command = "";

	if (!fs.good()) {
		cout << "cannot open scene file \"" << scene_file << "\"" << endl;
		return;
	}

	cout << scene_file << endl;

	while (fs >> command) {
		if (command == "model") {
			string key;
			array<float, 10> value;

			fs >> key >> value[0] >> value[1] >> value[2];
			fs >> value[3] >> value[4] >> value[5] >> value[6];
			fs >> value[7] >> value[8] >> value[9];

			models[key] = value;
		}
	}
}
