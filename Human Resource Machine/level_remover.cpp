#include "header.h"

Query Remove({
	{
		"Back"
	},

	"Choose a customize level to delete.\n",

	2, 3
});

void restore(Query &x, int row) {
	set_xy(x.stcol, row);
	int p = row - x.strow;
	cout << convert(x.choice[p], WHITE, FORE_NORMAL);
}
void highlight(Query &x, int row) {
	set_xy(x.stcol, row);
	int p = row - x.strow;
	cout << convert(x.choice[p], (p ? WHITE : BLACK), (p ? RED : WHITE), FORE_BRIGHT | BACK_BRIGHT);
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
	else {
		cout << "Are you sure you want to delete \"" << Remove.choice[x] << "\"? (Y/N)" << endl;
		string str;
		while(true) {
			getline(cin, str);
			if(str.length() != 1 || (str[0] != 'Y' && str[0] != 'N')) {
				system("cls");
				cout << "Please enter Y/N:" << endl;
				continue;
			}
			break;
		}
		if(str[0] == 'Y') {
			fs::path pth("./customize_level/" + Remove.choice[x] + ".dat");
			fs::remove(pth);
			Remove.choice.erase(Remove.choice.begin() + x);
		}
	}
	return false;
}

int main() {
	fs::path dir("./customize_level");
	fs::directory_iterator list(dir);
	for(auto &it : list) {
		Remove.choice.push_back(clear_suffix(it.path().filename().string()));
	}
	init(Remove);
	int cursor_row = Remove.strow;
	while(true) {
		if(kbhit() && (is_pressing(VK_DOWN) || is_pressing('S'))) {
			restore(Remove, cursor_row);

			cursor_row++;
			if(cursor_row >= Remove.strow + int(Remove.choice.size())) cursor_row = Remove.strow;
			highlight(Remove, cursor_row);

			while(is_pressing(VK_DOWN) || is_pressing('S'));
		}
		if(kbhit() && (is_pressing(VK_UP) || is_pressing('W'))) {
			restore(Remove, cursor_row);

			cursor_row--;
			if(cursor_row < Remove.strow) cursor_row = Remove.strow + Remove.choice.size() - 1;
			highlight(Remove, cursor_row);

			while(is_pressing(VK_UP) || is_pressing('W'));
		}
		if(kbhit() && is_pressing(VK_RETURN)) {
			Sleep(10);
			if(!is_pressing(VK_RETURN)) continue;
			bool quit = tackle(cursor_row - Remove.strow);
			if(quit) break;
			init(Remove);
			cursor_row = Remove.strow;
			while(kbhit() && is_pressing(VK_RETURN));
		}
	}
	system("cls");
	while(kbhit() && is_pressing(VK_RETURN));
	return 0;
}