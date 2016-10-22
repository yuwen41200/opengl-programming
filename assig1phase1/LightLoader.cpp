#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Light {

	class LightTerm {
	public:
		float diffuse[4] = {0, 0, 0, 1};
		float specular[4] = {0, 0, 0, 1};
		float ambient[4] = {0, 0, 0, 1};
		float position[4] = {0, 0, 0, 1};
	};

public:
	Light(const char *);
	vector<LightTerm> terms;
	float ambient[4] = {0, 0, 0, 1};

};

Light::Light(const char *light_file) {
	ifstream fs(light_file);
	string command = "";

	if (!fs.good()) {
		cout << "cannot open light file \"" << light_file << "\"" << endl;
		return;
	}

	cout << light_file << endl;

	while (fs >> command) {
		if (command == "light") {
			LightTerm new_term;
			fs >> new_term.position[0] >> new_term.position[1] >> new_term.position[2];
			fs >> new_term.ambient[0] >> new_term.ambient[1] >> new_term.ambient[2];
			fs >> new_term.diffuse[0] >> new_term.diffuse[1] >> new_term.diffuse[2];
			fs >> new_term.specular[0] >> new_term.specular[1] >> new_term.specular[2];
			terms.push_back(new_term);
		}

		else if (command == "ambient")
			fs >> ambient[0] >> ambient[1] >> ambient[2];
	}
}
