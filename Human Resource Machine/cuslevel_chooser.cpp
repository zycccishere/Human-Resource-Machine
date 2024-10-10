#include "header.h"

Query level({
	{
		"Back"
	},

	"+-------------------------------------+\n"
	"|           CHOOSE A LEVEL!           |\n"
	"+-------------------------------------+\n",

	4, 3
});

void restore(Query &x, int row) {
	set_xy(x.stcol, row);
	int p = row - x.strow;
	cout << convert(x.choice[p], WHITE, FORE_NORMAL);
}
void highlight(Query &x, int row) {
	set_xy(x.stcol, row);
	int p = row - x.strow;
	if(p == 0) // Back
		cout << convert(x.choice[p], BLACK, GREEN, FORE_BRIGHT | BACK_BRIGHT);
	else
		cout << convert(x.choice[p], BLACK, WHITE, FORE_BRIGHT | BACK_BRIGHT);
}
void init(Query &x) {
	hide_cursor();
	system("cls");
	cout << x.title;
	cout << "press W(or UP) and S(or DOWN) to choose, and press Enter to confirm." << endl;

	for(int row = x.strow; row <= x.strow + int(x.choice.size()) - 1; row++) {
		restore(x, row);
	}
	set_xy(x.stcol, x.strow);
	highlight(x, x.strow);
}

Code cd[3];
bool tackle(int x) {
	while(kbhit() && is_pressing(VK_RETURN));
	clear_buffer();
	system("cls");
	if(x == 0) { // Back
		return true;
	}
	else { // level x
		system("cls");
		cout << "Please enter your input file name(dir acceptable). If you would like to input in console, press enter." << endl;
		string str;
		show_cursor();
		getline(cin, str);
		if((int)str.length() > 0) { // input exists
			while(!exists(fs::path(str))) {
				system("cls");
				cout << "This file does not exist. Please try again (press enter to input in console):" << endl;
				getline(cin, str);
				if(str.length() == 0) break;
			}
		}
		// prepare for level x
		create_file("temp/level.dat");
		copy_file("customize_level/" + level.choice[x] + ".dat", "temp/level.dat");
		if(str.length() > 0) { // load from given file, and overwrite panel 1
			ifstream fin(str, ios::in);
			cd[0].cmds.resize(0);
			while(fin) {
				string str;
				getline(fin, str);
				if(str.length() == 0) continue;
				operation tmp = is_valid_command(str);
				if(tmp.label == -1) continue;
				cd[0].cmds.push_back((operation){tmp.label, tmp.num, file_syntax(tmp.content)});
			}
			cd[0].lines = cd[0].cmds.size();
		}
		ofstream fout("./temp/code.cd", ios::out);
		for(int i = 0; i < 3; i++) {
			Code &tmp = cd[i];
			fout << tmp.lines << endl;
			for(const auto &j : tmp.cmds) {
				fout << j.label;
				if(j.label == -1) { // wrong input
					fout << ' ' << j.content << endl;
				}
				else if(j.label >= 2) { // with opnum
					fout << ' ' << j.num << endl;
				}
				else { // without opnum
					fout << endl;
				}
			}
			fout << endl;
		}
		fout.close();
		hide_cursor();
		system("main.exe");
	}
	return false;
}

int main() {
	fs::path dir("./customize_level");
	fs::directory_iterator list(dir);
	for(auto &it : list) {
		level.choice.push_back(clear_suffix(it.path().filename().string()));
	}
	init(level);
	int cursor_row = level.strow;
	while(true) {
		if(kbhit() && (is_pressing(VK_DOWN) || is_pressing('S'))) {
			restore(level, cursor_row);

			cursor_row++;
			if(cursor_row >= level.strow + int(level.choice.size())) cursor_row = level.strow;
			highlight(level, cursor_row);

			while(is_pressing(VK_DOWN) || is_pressing('S'));
		}
		if(kbhit() && (is_pressing(VK_UP) || is_pressing('W'))) {
			restore(level, cursor_row);

			cursor_row--;
			if(cursor_row < level.strow) cursor_row = level.strow + level.choice.size() - 1;
			highlight(level, cursor_row);

			while(is_pressing(VK_UP) || is_pressing('W'));
		}
		if(kbhit() && is_pressing(VK_RETURN)) {
			Sleep(10);
			if(!is_pressing(VK_RETURN)) continue;
			bool quit = tackle(cursor_row - level.strow);
			hide_cursor();
			if(quit) break;
			init(level);
			cursor_row = level.strow;
			while(kbhit() && is_pressing(VK_RETURN));
		}
	}
	system("cls");
	while(kbhit() && is_pressing(VK_RETURN));
	return 0;
}