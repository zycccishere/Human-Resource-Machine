#include "header.h"

Query load({
	{"Back"},

	"+-------------------------------------+\n"
	"|         Please choose your ID       |\n"
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
	if(p == 0)
		cout << convert(x.choice[p], BLACK, WHITE, FORE_BRIGHT | BACK_BRIGHT);
	else
		cout << convert(x.choice[p], BLACK, GREEN, FORE_BRIGHT | BACK_BRIGHT);
}
void init(Query &x) {
	system("cls");

	fs::path dir("./identity");
	fs::directory_iterator list(dir);
	load.choice.clear();
	load.choice.push_back("Back");
	for(auto &it : list) {
		load.choice.push_back(clear_suffix(it.path().filename().string()));
	}
	sort(load.choice.begin() + 1, load.choice.end());

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
	else {
		copy_file("./identity/" + load.choice[x] + ".id", "./temp/now.id");
		system("level_chooser.exe");
		copy_file("./temp/now.id", "./identity/" + load.choice[x] + ".id");
	}
	return false;
}

int main() {
	init(load);
	int cursor_row = load.strow;
	while(true) {
		if(kbhit() && (is_pressing(VK_DOWN) || is_pressing('S'))) {
			restore(load, cursor_row);

			cursor_row++;
			if(cursor_row >= load.strow + int(load.choice.size())) cursor_row = load.strow;
			highlight(load, cursor_row);

			while(is_pressing(VK_DOWN) || is_pressing('S'));
		}
		if(kbhit() && (is_pressing(VK_UP) || is_pressing('W'))) {
			restore(load, cursor_row);

			cursor_row--;
			if(cursor_row < load.strow) cursor_row = load.strow + load.choice.size() - 1;
			highlight(load, cursor_row);

			while(is_pressing(VK_UP) || is_pressing('W'));
		}
		if(kbhit() && is_pressing(VK_RETURN)) {
			Sleep(10);
			if(!is_pressing(VK_RETURN)) continue;
			bool quit = tackle(cursor_row - load.strow);
			if(quit) break;
			init(load);
			cursor_row = load.strow;
			while(kbhit() && is_pressing(VK_RETURN));
		}
	}
	system("cls");
	while(kbhit() && is_pressing(VK_RETURN));
	return 0;
}