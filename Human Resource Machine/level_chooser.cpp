#include "header.h"

Query level({
	{
		"Back",
		"Level 1",
		"Level 2",
		"Level 3",
		"Level 4"
	},

	"+-------------------------------------+\n"
	"|           CHOOSE A LEVEL!           |\n"
	"+-------------------------------------+\n"
	"\033[92mLevel x\033[0m means that it is completed, while \033[91mLevel x\033[0m means that it is not completed yet.\n"
	"\033[94mLevel x\033[0m is your current progress.\n",

	6, 3
});

int progress;
Code cd[MAX_LEVEL + 1 + 10][3];

void restore(Query &x, int row) {
	set_xy(x.stcol, row);
	int p = row - x.strow;
	if(p == 0) // Back
		cout << convert(x.choice[p], WHITE, FORE_NORMAL);
	else if(p == progress)
		cout << convert(x.choice[p], BLUE, FORE_BRIGHT);
	else
		cout << convert(x.choice[p], (p < progress ? GREEN : RED), FORE_BRIGHT);
}
void highlight(Query &x, int row) {
	set_xy(x.stcol, row);
	int p = row - x.strow;
	if(p == 0) // Back
		cout << convert(x.choice[p], BLACK, GREEN, FORE_BRIGHT | BACK_BRIGHT);
	else if(p == progress)
		cout << convert(x.choice[p], BLUE, WHITE, FORE_BRIGHT | BACK_BRIGHT);
	else
		cout << convert(x.choice[p], (p < progress ? GREEN : RED), WHITE, FORE_BRIGHT | BACK_BRIGHT);
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

bool tackle(int x) {
	while(kbhit() && is_pressing(VK_RETURN));
	clear_buffer();
	system("cls");
	if(x == 0) { // Back
		return true;
	}
	else { // level x
		if(x <= progress) {
			system("cls");
			cout << "Please enter your input file name(dir acceptable). If you would like to input in console, press enter." << 
			endl << convert("Warning: Entering from file will overwrite your previous code in code panel 1!", RED, FORE_BRIGHT) << endl;
			string str;
			show_cursor();
			getline(cin, str);
			hide_cursor();
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
			copy_file("level_info/level" + to_string(x) + ".dat", "temp/level.dat");

			if(str.length() > 0) { // load from given file, and overwrite panel 1
				ifstream fin(str, ios::in);
				cd[x][0].cmds.resize(0);
				while(fin) {
					string str;
					getline(fin, str);
					if(str.length() == 0) continue;
					operation tmp = is_valid_command(str);
					if(tmp.label == -1) continue;
					int lb = -1;
					if(tmp.label == -2) lb = -1;
					else lb = tmp.label;
					cd[x][0].cmds.push_back((operation){lb, tmp.num, file_syntax(tmp.content)});
				}
				cd[x][0].lines = cd[x][0].cmds.size();
			}
			ofstream fout("./temp/code.cd", ios::out);
			for(int i = 0; i < 3; i++) {
				Code &tmp = cd[x][i];
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
			system("main.exe");
			// update modified code
			ifstream fin("./temp/code.cd");
			int pass; fin >> pass;
			progress = max(progress, pass + 1);
			for(int i = 0; i < MAX_PANEL; i++) {
				fin >> cd[x][i].lines;
				vector <operation> &tmp = cd[x][i].cmds;
				tmp.resize(0);
				for(int j = 0; j < cd[x][i].lines; j++) {
					tmp.push_back((operation){});
					fin >> tmp[j].label;
					if(tmp[j].label == -1) fin >> tmp[j].content;
					else if(tmp[j].label >= 2) fin >> tmp[j].num;
				}
			}
			fin.close();
		}
		else if(x <= MAX_LEVEL){
			system("cls");
			cout << "You cannot enter Level " << x << " before you pass Level " << progress << " !" << endl;
			system("pause");
		}
	}
	return false;
}

int main() {
	ifstream fin("./temp/now.id");
	fin >> progress;
	for(int i = 1; i <= progress; i++) { // read info about code
		for(int j = 0; j < MAX_PANEL; j++) {
			fin >> cd[i][j].lines;
			for(int k = 1; k <= cd[i][j].lines; k++) {
				int op, x = -1; string tmp = "";
				fin >> op;
				if(op >= 2) // with an operating number
					fin >> x;
				else if(op == -1) { // wrong but acceptable code
					fin >> tmp;
				}
				cd[i][j].cmds.push_back((operation){op, x, tmp});
			}
		}
	}
	fin.close();
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
			if(quit) break;
			init(level);
			hide_cursor();
			cursor_row = level.strow;
			while(kbhit() && is_pressing(VK_RETURN));
		}
	}
	system("cls");
	while(kbhit() && is_pressing(VK_RETURN));

	ofstream fout("./temp/now.id");
	fout << progress << endl;
	for(int i = 1; i <= progress; i++) {
		for(int j = 0; j < MAX_PANEL; j++) {
			fout << cd[i][j].lines << endl;
			for(int k = 0; k < cd[i][j].lines; k++) {
				fout << cd[i][j].cmds[k].label;
				int op = cd[i][j].cmds[k].label;
				if(op >= 2) // with an operating number
					fout << ' ' << cd[i][j].cmds[k].num << endl;
				else if(op == -1)
					fout << ' ' << cd[i][j].cmds[k].content << endl;
				else
					fout << endl;
			}
			fout << endl;
		}
		fout << endl;
	}
	fout.close();
	return 0;
}