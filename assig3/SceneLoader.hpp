#include <iostream>
#include <fstream>
#include <map>
#include <array>

using namespace std;

class Scene {

public:
	Scene(const char *);
	multimap<string, array<float, 11>> models;
	vector<array<string, 6>> maps;

};

Scene::Scene(const char *scene_file) {
	ifstream fs(scene_file);
	string command = "";
	float idx = -1;
	// Ugly Windows
	char tempBuf[250];

	if (!fs.good()) {
		cout << "cannot open scene file \"" << scene_file << "\"" << endl;
		return;
	}

	cout << scene_file << endl;

	while (fs >> tempBuf) {
		// Ugly Windows
		command = tempBuf;
		if (command == "model") {
			string key;
			array<float, 11> value;

			// Ugly Windows
			fs >> tempBuf;
			key = tempBuf;
			fs >> value[0] >> value[1] >> value[2];;
			// Linux
			//fs >> key >> value[0] >> value[1] >> value[2];
			fs >> value[3] >> value[4] >> value[5] >> value[6];
			fs >> value[7] >> value[8] >> value[9];

			value[10] = idx;
			models.insert(pair<string, array<float, 11>>(key, value));
		}

		else if (command == "no-texture")
			idx = -1;

		else if (command == "single-texture") {
			array<string, 6> value;
			// Ugly Windows
			fs >> tempBuf;
			value[0] = tempBuf;
			// Linux
			//fs >> value[0];
			value[1] = value[2] = value[3] = value[4] = value[5] = "";
			maps.push_back(value);
			idx = maps.size() - 1;
		}

		else if (command == "multi-texture") {
			array<string, 6> value;
			// Ugly Windows
			fs >> tempBuf;
			value[0] = tempBuf;
			fs >> tempBuf;
			value[1] = tempBuf;
			// Linux
			//fs >> value[0] >> value[1];
			value[2] = value[3] = value[4] = value[5] = "";
			maps.push_back(value);
			idx = maps.size() - 1;
		}

		else if (command == "cube-map") {
			array<string, 6> value;
			// Ugly Windows
			fs >> tempBuf;
			value[0] = tempBuf;
			fs >> tempBuf;
			value[1] = tempBuf;
			fs >> tempBuf;
			value[2] = tempBuf;
			fs >> tempBuf;
			value[3] = tempBuf;
			fs >> tempBuf;
			value[4] = tempBuf;
			fs >> tempBuf;
			value[5] = tempBuf;
			// Linux
			//fs >> value[0] >> value[1] >> value[2];
			//fs >> value[3] >> value[4] >> value[5];
			maps.push_back(value);
			idx = maps.size() - 1;
		}
	}

	fs.close();
}
