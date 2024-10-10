#include "header.h"

Query custom({
	{
		"Back",
		"New Level",
		"Delete Level",
		"Start Game"
	},

	"+-------------------------------------+\n"
	"|           Customize Level           |\n"
	"+-------------------------------------+\n"
	"Check customize_tutorial.txt before creating!\n",

	5, 3
});

void restore(Query &x, int row) {
	set_xy(x.stcol, row);
	int p = row - x.strow;
	cout << convert(x.choice[p], WHITE, FORE_NORMAL);
}
void highlight(Query &x, int row) {
	set_xy(x.stcol, row);
	int p = row - x.strow;
	cout << convert(x.choice[p], BLACK, WHITE, FORE_BRIGHT | BACK_BRIGHT);
}
void init(Query &x) {
	system("cls");

	cout << x.title;
	cout << "press W(or UP) and S(or DOWN) to choose, and press Enter to confirm." << endl;
	for(int row = x.strow; row <= x.strow + int(x.choice.size()) - 1; row++) {
		restore(x, row);
	}
	set_xy(x.stcol, x.strow);
	highlight(x, x.strow);
	hide_cursor();
}

bool tackle(int x) {
	while(kbhit() && is_pressing(VK_RETURN));
	clear_buffer();
	system("cls");
	if(x == 0) { // Back
		return true;
	}
	else if(x == 1) { // New Level
		system("cls");
		cout << "Please Enter the DIR of a level config file. (\'Q\'to exit)" << endl;
		show_cursor();
		string str, name;
		while(true) {
			getline(cin, str);
			if(str.length() == 1 && str[0] == 'Q') { // exit
				return false;
			}
			if(!exists(fs::path(str))) {
				system("cls");
				cout << "This file does not exist. Please try again. (\'Q\'to exit)" << endl;
				continue;
			}
			else {
				ifstream fin(str);
				getline(fin, name);
				fin.close();
				if(exists(fs::path("./customize_level/" + name + ".dat"))) {
					system("cls");
					cout << "You have already created this level! Please try again:" << endl;
					continue;
				}
				break;
			}
		}
		hide_cursor();
		create_file("./customize_level/" + name + ".dat");
		ifstream fin(str);
		ofstream fout("./customize_level/" + name + ".dat");
		
		fout << -1 << endl; // customize number
		getline(fin, name);
		fout << name << endl; // level name
		string note;
		getline(fin, note);
		fout << note << endl;

		int innum; fin >> innum; fout << innum << ' ';
		for(int i = 1; i <= innum; i++) {
			int x;
			fin >> x;
			fout << x << ' ';
		}
		fout << endl;

		int outnum; fin >> outnum; fout << outnum << ' ';
		for(int i = 1; i <= outnum; i++) {
			int x;
			fin >> x;
			fout << x << ' ';
		}
		fout << endl;

		int spacenum; fin >> spacenum; fout << spacenum << ' ';
		for(int i = 1; i <= spacenum; i++) {
			string x;
			fin >> x;
			if(x == "blank") fout << 114514 << ' ';
			else fout << atoi(x.c_str()) << ' ';
		}
		fout << endl;

		int cmdnum; fin >> cmdnum; fout << cmdnum << ' ';
		for(int i = 1; i <= cmdnum; i++) {
			string cmd;
			fin >> cmd;
			fout << is_cmdname(cmd) << ' ';
		}
		fout << endl;

		int line_limit, compute_limit;
		fin >> line_limit >> compute_limit;
		fout << line_limit << ' ' << compute_limit << endl;
		fin.close(); fout.close();
		cout << '\"' << convert(name, WHITE, BLUE, BACK_BRIGHT | FORE_BRIGHT) << '\"' << " has been successfully created as a new level!" << endl;
		system("pause");
		return false;
	}
	else if(x == 2) { // Delete Level
		system("level_remover.exe");
	}
	else if(x == 3) { // Start Game
		system("cuslevel_chooser.exe");
	}
	return false;
}

int main() {
	hide_cursor();
	init(custom);
	int cursor_row = custom.strow;
	while(true) {
		if(kbhit() && (is_pressing(VK_DOWN) || is_pressing('S'))) {
			restore(custom, cursor_row);

			cursor_row++;
			if(cursor_row >= custom.strow + int(custom.choice.size())) cursor_row = custom.strow;
			highlight(custom, cursor_row);

			while(is_pressing(VK_DOWN) || is_pressing('S'));
		}
		if(kbhit() && (is_pressing(VK_UP) || is_pressing('W'))) {
			restore(custom, cursor_row);

			cursor_row--;
			if(cursor_row < custom.strow) cursor_row = custom.strow + custom.choice.size() - 1;
			highlight(custom, cursor_row);

			while(is_pressing(VK_UP) || is_pressing('W'));
		}
		if(kbhit() && is_pressing(VK_RETURN)) {
			Sleep(10);
			if(!is_pressing(VK_RETURN)) continue;
			bool quit = tackle(cursor_row - custom.strow);
			hide_cursor();
			if(quit) break;
			init(custom);
			cursor_row = custom.strow;
			while(kbhit() && is_pressing(VK_RETURN));
		}
	}
	system("cls");
	while(kbhit() && is_pressing(VK_RETURN));
	return 0;
}