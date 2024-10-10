//
//  header.h
//  mainprogram
//
//  Created by zhangyc on 2023/12/2.
//  Modified by ChenYumo on 2023/12/21
//

#pragma once
#ifndef header_h
#define header_h

#include <conio.h>
#include <list>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cctype>
#include <windows.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

#define cur_up(n) "\033[" #n "A"
#define cur_down(n) "\033[" #n "B"
#define cur_forward(n) "\033[" #n "C"
#define cur_backward(n) "\033[" #n "D"
#define cur_position(x,y) "\033[" #y ";" #x "H"
#define xinterval 5
#define yinterval 3
#define ANSI_BACKGROUND_RED     "\x1b[41m"
#define ANSI_BACKGROUND_GREEN   "\x1b[42m"
#define ANSI_BACKGROUND_YELLOW  "\x1b[43m"
#define ANSI_BACKGROUND_BLUE    "\x1b[44m"
#define ANSI_BACKGROUND_MAGENTA "\x1b[45m"
#define ANSI_BACKGROUND_CYAN    "\x1b[46m"
#define ANSI_BACKGROUND_RESET   "\x1b[49m"

#define LONG_WAIT (1500)
#define NORMAL_WAIT (400)
#define SHORT_WAIT (100)

#define END_NORMAL -1
#define CONTINUE -2
#define ENDLESS_LOOP -3

#define MAX_LEVEL 4
#define MAX_SIDEBAR 6
#define MAX_PANEL 3

#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37
#define FORE_BRIGHT (0b01)
#define FORE_NORMAL (0b00)
#define BACK_BRIGHT (0b10)
#define BACK_NORMAL (0b00)

//robot movement
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3


//robot status
#define Norm 0
#define Hold_block 1
#define Interact_space 2
#define Fetch_inbox 3
#define Put_outbox 4

struct pos{int x; int y;};
struct operation {
	int label;
	int num;
	string content;
};
struct Code {
	int lines;
	vector <operation> cmds;
};
struct Query {
	vector <string> choice;
	string title;
	int strow, stcol;
};
struct ID {
	string name;
	vector <bool> level_info;
};
struct Console_Cmd {
	int op;
	int x1, x2, x3;
	string content;
};

const pos init_pos={7, 27};
const pos in_pos={12, 13};
const pos out_pos={12, 41};

vector <pos> space_loc;

const int robot_lines[5] = {
	8, 10, 8, 8, 8
};
const string handsome_robot[5][10]={
	{ // 3
		"  ? ? ?  ",
		"  |@ @|  ",
		"  \\_-_/  ",
		"  --|--  ",
		" /  |  \\ ",
		"0  [#]  0",
		"   . .   ",
		"  *   *  "
	},
	{ // 1
		"  +---+  ",
		"[>| n |<]",
		"| +---+ |",
		"| |@ @| |",
		"\\ \\_~_/ /",
		" +--|--+ ",
		"    |    ",
		"   [#]   ",
		"  /   \\  ",
		" *#   #* "
	},
	{ // 2
		"  ? ? ?  ",
		"  |@ @|  ",
		"  \\_~_/  ",
		"  --|--  ",
		" /  |  \\ ",
		" {+---+} ",
		"  | n |  ",
		"  +---+  "
	},
	{ // 4
		"  + $ +  ",
		"   @  |  ",
		"  [___/  ",
		"}---|    ",
		"    |    ",
		"   [*]   ",
		"  '   '  ",
		"  %   %  "
	},
	{
		"  + $ +  ",
		"  |  @   ",
		"  \\___]  ",
		"    |---{",
		"    |    ",
		"   [*]   ",
		"  '   '  ",
		"  %   %  "
	}
};

const pos levpos={2,10}; // Level
const pos nampos={2,38}; // Name
const pos valpos={6,87}; // Valid Operations
const pos spapos={22,29}; // Space
const pos youpos={6,65}; // Your Code
const pos newoppos={28,1}; // New Operations
const pos inputbegin={10,6}; // Begin of Input
const pos outputbegin={10,52}; // Begin of Output
const pos panel = {2, 82}; // Code Panel
const pos term_pos = {28, 0}; // Positon under the outline
const pos spd_pos = {29, 83};
string op[]={"inbox","outbox","add","sub","copyto","copyfrom","jump","jumpifzero"};
/*
inbox         0
outbox        1
add        x  2
sub        x  3
copyto     x  4
copyfrom   x  5
jump       x  6
jumpifzero x  7
*/
const int NOTE_WIDTH = 101, OUTLINE_WIDTH = 105;
string outline =
"+----------------------------+----------------------------+   +-----------------------------------------+\n"
"|                            |                            |   |                                         |\n"
"|   Level                    |                            |   |  Command Panel  (   / 3 )               |\n"//2,10;2,12;2,68
"|                            |                            |   |                                         |\n"
"+----------------------------+----------------------------+   +--------------------+--------------------+\n"
"|                                                         |   |     Your Code      |  Valid Operations  |\n"
"|                                                         |   |                    |                    |\n"
"|   ^IN^                                          ^OUT^   |   |                    |                    |\n"
"|  +-----+                                       +-----+  |   |                    |                    |\n"//8,83
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |                                       |     |  |   |                    |                    |\n"
"|  |     |      +-------------------------+      |     |  |   |                    |                    |\n"
"|  |     |      |                         |      |     |  |   |                    |                    |\n"
"|  |     |      |                         |      |     |  |   |                    |                    |\n"
"|  |     |      |                         |      |     |  |   |                    |                    |\n"
"|  |     |      +-------------------------+      |     |  |   |                    |                    |\n"
"|  |     |                 SPACE                 |     |  |   |                    |                    |\n"
"|  |* * *|                                       |* * *|  |   |                    |                    |\n"
"+---------------------------------------------------------+   +--------------------+--------------------+\n";

/* OS and console related functions */
string posit(string str,int x,int y){
	string out = "\033[" + to_string(x + 1) + ";" + to_string(y + 1) + "H" + str + "\n";
	return out;
}
void set_xy(int x, int y) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){(short)x, (short)y});
}
void full_screen() {
	HWND hwnd = GetForegroundWindow();
	int cx = GetSystemMetrics(SM_CXSCREEN); /* 屏幕宽度 像素 */
	int cy = GetSystemMetrics(SM_CYSCREEN); /* 屏幕高度 像素 */
	LONG l_WinStyle = GetWindowLong(hwnd,GWL_STYLE); /* 获取窗口信息 */
	/* 设置窗口信息 最大化 取消标题栏及边框 */
	SetWindowLong(hwnd,GWL_STYLE,(l_WinStyle | WS_POPUP | WS_MAXIMIZE));
	SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);
}
void hide_cursor()
{
	HANDLE h_GAME =GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(h_GAME, &cursor_info);
	cursor_info.bVisible=false;
	SetConsoleCursorInfo(h_GAME, &cursor_info);
}
void show_cursor() 
{
	HANDLE h_GAME =GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(h_GAME, &cursor_info);
	cursor_info.bVisible=true;
	SetConsoleCursorInfo(h_GAME, &cursor_info);
}
bool is_pressing(int vkval) {
	return bool(GetAsyncKeyState(vkval));
}
void clear_buffer() {
	cin.ignore(numeric_limits <streamsize>::max(), '\n');
}

/* functions used to convert outputs */
string convert(string str, int foreground_color, int background_color, int is_bright) {
	string cmd = 	"\033[" + to_string(foreground_color + 60 * (is_bright & 0b01)) + "m" +
						"\033[" + to_string(background_color + 60 * (is_bright & 0b10 ? 1 : 0) + 10) + "m" +
						str + 
						"\033[0m";
	return cmd;
}
string convert(string str, int color, int is_bright) {
	string cmd = 	"\033[" + to_string(color + 60 * is_bright) + "m" +
						str + 
						"\033[0m";
	return cmd;
}
string file_syntax(string x) { // convert x, s.t. all spaces in x turn into '$'
	for(int i = 0; i < (int)x.size(); i++)
		if(x[i] == ' ') x[i] = '$';
	return x;
}
string console_syntax(string x) { // the reverse process of file_syntax()
	for(int i = 0; i < (int)x.size(); i++)
		if(x[i] == '$') x[i] = ' ';
	return x;
}
string clear_suffix(string file_name) {
	for(int i = file_name.length() - 1; i >= 0; i--) {
		if(file_name[i] == '.') {
			return file_name.substr(0, i);
		}
	}
	return "Fatal Error!";
}
int sgn(int x){
	if(x < 0) return -1;
	if(x > 0) return 1;
	return 0;
}

/* functions for safe input */
int is_cmdname(string &x) { // find the index of command x in op[], if x is illegal, return -1
	for(int i = 0; i < 8; i++) {
		if(x == op[i]) return i;
	}
	return -1;
}
bool is_opnum(string &x) { // check if x is composed of digits
	if(x.length() == 0) return false;
	for(int i = 0; i < (int)x.length(); i++) {
		if(!isdigit(x[i])) return false;
	}
	return true;
}
Console_Cmd analyze_console_input(string &str) {
	const Console_Cmd FAIL = (Console_Cmd){-1, -1, -1, -1, ""}; // unacceptable input
	for(int i = 0; i < (int)str.size(); i++) if(str[i] == '$') return FAIL;
	stringstream sstream(str);
	vector <string> v;
	while(sstream) {
		v.push_back("");
		sstream >> v[v.size() - 1];
	}
	if(v.size() <= 2) return FAIL;
	string y = "";
	for(int i = 2; i < (int)v.size(); i++) {
		y += v[i];
		if(i != (int)v.size() - 1) y += ' ';
	}
	string x = v[1] + ' ' + y;
	if(v[0] != "new" && v[0] != "ins" && v[0] != "del") return FAIL;
	if(v[0] == "new" && (v.size() != 4 && v.size() != 3)) return (Console_Cmd){0, -2, -2, -1, x};
	if(v[0] == "ins" && !is_opnum(v[1])) return FAIL;
	if(v[0] == "del" && v.size() != 3) return FAIL;
	if(v[0] == "del") {
		if(!is_opnum(v[1])) return FAIL;
		return (Console_Cmd){2, atoi(v[1].c_str()), -1, -1, x};
	}
	else {
		if(v[0] == "new") {
			int op = is_cmdname(v[1]);
			if(op == -1 || (op >= 2 && !is_opnum(v[2])) || (op < 2 && v[2].size() != 0)) return (Console_Cmd){0, -2, -2, -1, x};
			return (Console_Cmd){0, op, (v[2].size() == 0 ? -1 : atoi(v[2].c_str())), -1, x};
		}
		if(v[0] == "ins") {
			int line = atoi(v[1].c_str());
			int op = is_cmdname(v[2]);
			if(op == -1 || (op >= 2 && !is_opnum(v[3])) || (op < 2 && v[3].size() != 0)) return (Console_Cmd){1, line, -2, -1, y};
			return (Console_Cmd){1, line, op, (v[3].size() == 0 ? -1 : atoi(v[3].c_str())), y};
		}
	}
	return FAIL;
}
operation is_valid_command(string x) { // 使用getline读入x并调用这个函数
	const operation FAIL = (operation){-1, -1, ""}; // 不可接受的输入
	const operation ILLE = (operation){-2, -1, x};
	if(x.length() == 0 || x.length() >= 16) return FAIL;
	stringstream sstream(x);
	vector <string> v;
	while(sstream) {
		v.push_back("");//给vector增加一个长度
		sstream >> v[v.size() - 1];
		if(v.size() > 3) return ILLE;
	} //这一段可以完成按空格分割的字符串读取
	string &cmd = v[0];
	int op = is_cmdname(cmd);
	if(op == -1) return ILLE;
	if(op < 2) {
		if(v.size() == 3) return ILLE;
		return (operation){op, -1, x};
	}
	// op >= 2
	if(v.size() == 2) return ILLE;
	string &num = v[1];
	if(!is_opnum(num)) return ILLE;
	return (operation){op, atoi(num.c_str()), x};
}
bool not_a_filename(string file_name) { // check whether file_name is a file name
	if(file_name.length() == 0) return true;
	bool all_space = true;
	for(int i = 0; i < (int)file_name.length(); i++) {
		char &c = file_name[i];
		if(c == '*' || c == '/' || c == '\\' || c == '<'
			|| c == '>' || c == '|' || c == '?' ||
			c == '\"' || c == ':') return true;
		if(c != ' ') all_space = false;
	}
	if(all_space) return true;
	return false;
}

/* functions frequently used */
void getfileop(Code *x){ // read from code.cd, store the information in Code[]
	ifstream fin;
	fin.open("./temp/code.cd");
	for(int i = 0; i < 3; i++) {
		x[i].cmds.resize(0);
		fin >> x[i].lines;
		for(int j = 0; j < x[i].lines; j++) {
			x[i].cmds.push_back((operation){});
			fin >> x[i].cmds[j].label; int &op = x[i].cmds[j].label;
			if(op == -1) {
				x[i].cmds[j].label = -1;
				fin >> x[i].cmds[j].content;
			}
			else if(op >= 2) {
				fin >> x[i].cmds[j].num;
			}
			else x[i].cmds[j].num = -1;
		}
	}
	fin.close();
}
void clear_op(pos position, vector <operation> &fop, int shiftx) { // clear all commands in "Your Code"
	int size = fop.size();
	for(int i = 0; i <= size; i++) {
		set_xy(position.y, position.x + i + shiftx);
		cout << "                  ";
	}
}
void show_file_op(int index, pos position, vector <operation> &fop, int shiftx){ // show given operations
	clear_op(position, fop, shiftx);
	int size = fop.size();
	for(int i = 0; i < size; i++){

		if(i == index){
			if(fop[i].label == -1) {
				cout << posit(ANSI_BACKGROUND_RED + to_string(i + 1) + ". " + console_syntax(fop[i].content)
				 + ANSI_BACKGROUND_RESET, position.x + i + shiftx, position.y);
				continue;
			}
			if(fop[i].num == -1) {
				cout << posit(ANSI_BACKGROUND_RED + to_string(i + 1) + ". " + op[fop[i].label] + ANSI_BACKGROUND_RESET, position.x + i + shiftx, position.y);
			}
			else {
				cout << posit(ANSI_BACKGROUND_RED + to_string(i + 1) + ". " + op[fop[i].label] + ' ' + to_string(fop[i].num) + ANSI_BACKGROUND_RESET, position.x + i + shiftx, position.y);
			}
		}
		else {
			if(fop[i].label == -1) {
				cout << posit(to_string(i + 1) + ". " + console_syntax(fop[i].content), position.x + i + shiftx, position.y);
				continue;
			}
			if(fop[i].num == -1) {
				cout << posit(to_string(i + 1) + ". " + op[fop[i].label], position.x + i + shiftx, position.y);
			}
			else {
				cout << posit(to_string(i + 1) + ". " + op[fop[i].label] + ' ' + to_string(fop[i].num), position.x + i + shiftx, position.y);
			}
		}
	}
}
bool copy_file(string from, string to) {
	ifstream fin(from);
	if(!fin) return false;
	ofstream fout(to);
	if(!fout) return false;
	while(true) {
		char c = fin.get();
		if(c == EOF) {
			fin.close();
			fout.close();
			break;
		}
		fout << c;
	}
	return true;
}
int show_files(string str) {
	fs::path dir(str);
	fs::directory_iterator list(dir);
	int lines = 0;
	if(fs::is_empty(dir)) {
		cout << "Obviously, nothing has been created yet." << endl;
		return 1;
	}
	else {
		cout << endl;
		lines++;
		for(auto &it : list) {
			cout << clear_suffix(it.path().filename().string()) << endl;
			lines++;
		}
	}
	cout << endl;
	return lines + 1;
}
void clear_file(string dir) {
	ofstream fout;
	fout.open(dir, ios::trunc);
	if(!fout) return;
	fout.close();
}
void create_file(string dir) {
	ofstream fout;
	fout.open(dir, ios::trunc);
	fout.close();
}
void show_spd(int spd, int shiftx) {
	set_xy(spd_pos.y, spd_pos.x + shiftx);
	cout << "                           ";
	set_xy(spd_pos.y, spd_pos.x + shiftx);
	if(spd != 0)
		cout << convert("Speed: " + to_string(spd) + 'X', BLUE, FORE_BRIGHT);
	else
		cout << convert("Speed: Single Step Mode", YELLOW, FORE_BRIGHT);
}
#endif /* header_h */
