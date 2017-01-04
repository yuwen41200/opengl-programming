#include <iostream>
#include <fstream>

using namespace std;

class View {

public:
	View(const char *);
	double lookAt[9];
	double perspective[3];
	int viewport[4];

};

View::View(const char *view_file) {
	ifstream fs(view_file);
	string command = "";
	// Ugly Windows
	char tempBuf[250];

	if (!fs.good()) {
		cout << "cannot open view file \"" << view_file << "\"" << endl;
		return;
	}

	cout << view_file << endl;

	while (fs >> tempBuf) {
		// Ugly Windows
		command = tempBuf;
		if (command == "eye")
			fs >> lookAt[0] >> lookAt[1] >> lookAt[2];
		else if (command == "vat")
			fs >> lookAt[3] >> lookAt[4] >> lookAt[5];
		else if (command == "vup")
			fs >> lookAt[6] >> lookAt[7] >> lookAt[8];

		else if (command == "fovy")
			fs >> perspective[0];
		else if (command == "dnear")
			fs >> perspective[1];
		else if (command == "dfar")
			fs >> perspective[2];

		else if (command == "viewport")
			fs >> viewport[0] >> viewport[1] >> viewport[2] >> viewport[3];
	}

	fs.close();
}
