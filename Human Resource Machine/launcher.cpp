#include "header.h"

const string About = 
"+------------------------------------------------------------------------------------+\n"
"|                               HUMAN RESOURCE MACHINE                               |\n"
"|************************************************************************************|\n"
"|*   Created by: ChenYumo  ZhangYicheng (sorted by lexicographic order)             *|\n"
"|*                                                                                  *|\n"
"|*      -Laucher:       ChenYumo                                                    *|\n"
"|*      -Main Program:  ZhangYicheng                                                *|\n"
"|*      -Merge:         ChenYumo                                                    *|\n"
"|*      -Auto Judge:    ZhangYicheng                                                *|\n"
"|*      -ArtWork:       ZhangYicheng                                                *|\n"
"|************************************************************************************|\n"
"|                                                                                    |\n"
"|************************************************************************************|\n"
"|*                                 Acknowledgement                                  *|\n"
"|*                                                                                  *|\n"
"|*   DuRuohe & Tangmingliang                                                        *|\n"
"|*            --- For their enlightening ideas and powerful technical support       *|\n"
"|*                                                                                  *|\n"
"|*   Our professors and assistants                                                  *|\n"
"|*            --- For their hardwork and patience                                   *|\n"
"|*                                                                                  *|\n"
"|*   Our friends and family members                                                 *|\n"
"|*            --- For their support and help in debugging                           *|\n"
"|************************************************************************************|\n"
"|                                                                                    |\n"
"|                                                                       2023.12      |\n"
"+------------------------------------------------------------------------------------+\n"
;

Query launch({
	{
		"1.Start Game",
		"2.Create ID",
		"3.Delete ID",
		"4.Customize Game",
		"5.Help",
		"6.About",
		"7.Quit"
	},

	"+-------------------------------------+\n"
	"|                                     |\n"
	"|   Human Resource Machine Laucher    |\n"
	"|                        ver. 1.0.0   |\n"
	"|                                     |\n"
	"+-------------------------------------+\n",

	7, 3
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

bool tackle(int x, bool spcreate) {
	while(kbhit() && is_pressing(VK_RETURN));
	if(!spcreate) clear_buffer();
	if(!spcreate && x != 6) system("cls");
	if(x == 0) {		// Start Game
		fs::path dir("./identity");
		while(fs::is_empty(dir)) {
			cout << "Oops! You have no ID to load! Please create one before you start game." << endl;
			bool flag = tackle(1, true);
			if(!flag) return false;
		}
		system("game_loader.exe");
	}
	else if(x == 1) {	// Create ID
		if(!spcreate) set_xy(0, 0);
		else set_xy(0, 1);
		cout << "All IDs are shown below:" << endl;
		int startrow = show_files("./identity") + 1 + (spcreate ? 1 : 0);

		show_cursor();
		cout << "Please input your new identity name(Input 'Q' to go back):" << endl;
		
		string str;
		while(true) {
			getline(cin, str);
			if(not_a_filename(str)) {
				set_xy(0, startrow); cout << "                                                          ";
				set_xy(0, startrow + 1); for(int i = 1; i <= int(str.length()); i++) cout << ' ';
				set_xy(0, startrow);
				cout << "Inappropriate ID! Please try again:" << endl;
			}
			else if(str[0] == 'Q' && str.length() == 1) return false;
			else if(exists(fs::path("./identity/" + str + ".id"))) {
				set_xy(0, startrow); cout << "                                                          ";
				set_xy(0, startrow + 1); for(int i = 1; i <= int(str.length()); i++) cout << ' ';
				set_xy(0, startrow);
				cout << "You have already created this identity, try again:" << endl;
			}
			else break;
		}
		hide_cursor();
		system("cls");
		create_file("./identity/" + str + ".id");
		ofstream fout("./identity/" + str + ".id", ios::out);
		fout << 1 << endl; // Start from level 1
		for(int i = 1; i <= MAX_LEVEL; i++) {
			fout << 0 << endl << endl << 0 << endl << endl << 0 << endl << endl; // blank space at first
		}
		fout.close();
		cout << '\"' << str << '\"' << " has been successfully created as your new identity!" << endl;
		if(spcreate) {
			Sleep(500);
			return true;
		}
		cout << endl << "Press any key to go back!" << endl;
		while(kbhit());
		while(!kbhit());
		clear_buffer();
	}
	else if(x == 2) {	// Delete ID
		system("ID_remover.exe");
	}
	else if(x == 3) {	// Customize Game
		system("customize.exe");
	}
	else if(x == 4) {	// Help
		system("cls");
		system("HELP.pdf");
		cout << "Hope this can help you. If nothing opens, check \"HELP.pdf\" or \"HELP.md\"" << endl;
		system("pause");
	}
	else if(x == 5) {	// About
		system("cls");
		cout << About << endl;
		system("pause");
	}
	else if(x == 6) {	// Quit
		return true;
	}
	hide_cursor();
	return false;
}

int main() {
	full_screen();
	init(launch);
	int cursor_row = launch.strow;
	while(true) {
		if(kbhit() && (is_pressing(VK_DOWN) || is_pressing('S'))) {
			restore(launch, cursor_row);

			cursor_row++;
			if(cursor_row >= launch.strow + int(launch.choice.size())) cursor_row = launch.strow;
			highlight(launch, cursor_row);

			while(is_pressing(VK_DOWN) || is_pressing('S'));
		}
		if(kbhit() && (is_pressing(VK_UP) || is_pressing('W'))) {
			restore(launch, cursor_row);

			cursor_row--;
			if(cursor_row < launch.strow) cursor_row = launch.strow + launch.choice.size() - 1;
			highlight(launch, cursor_row);

			while(is_pressing(VK_UP) || is_pressing('W'));
		}
		if(kbhit() && is_pressing(VK_RETURN)) {
			Sleep(10);
			if(!is_pressing(VK_RETURN)) continue;
			bool quit = tackle(cursor_row - launch.strow, false);
			if(quit) break;
			init(launch);
			cursor_row = launch.strow;
			while(kbhit() && is_pressing(VK_RETURN));
		}
	}
	system("cls");
	return 0;
}