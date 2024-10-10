#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

const string sources[8] = {
	"launcher",
	"game_loader",
	"level_chooser",
	"level_remover",
	"main",
	"customize",
	"cuslevel_chooser",
	"ID_remover"
};

int main() {
	for(int i = 0; i < 8; i++) {
		string cmd = "g++ -std=c++17 " + sources[i] + ".cpp -o " + sources[i] + " -static";
		system(cmd.c_str());
	}
	return 0;
}