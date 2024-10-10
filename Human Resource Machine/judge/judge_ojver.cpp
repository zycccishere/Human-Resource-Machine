/*
 * 程序正确性检验器，调用时需要 judge.exe level_num，并且需要在temp文件夹下生成好code.cd
 */

#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <iostream>
using namespace std;

#define END (-3)
#define FAIL (-1)
#define TLE (-2)

#define EMPTY (-2000000000)

int status;
void fail() {
	status = FAIL;
}
void error_on(int x) {
	status = x;
}

struct command {
	int op, x;
};
vector <command> v;

int level;
const int MAX_DATA = 1e3 + 10, MAX_SPACES = 3, MAX_WAIT = 1000;
const int spaces_lim[] = {-1, 0, 3, 3};
int indata[MAX_DATA], ans[MAX_DATA], outdata[MAX_DATA];
int inp, outp, indata_n, outdata_n, spaces[MAX_SPACES];
bool flag[MAX_SPACES]; // 空地上是否有数

int hand;

int lenin[3] = {2, 8, 8};
int lenout[3] = {2, 8, 2};
const int Data[][20] = {
	{1, 2},
	{3, 9, 5, 1, -2, -2, 9, -9},
	{6, 2, 7, 7, -9, 3, -3, -3}
};
const int Datao[][20] = {
	{1, 2},
	{-6, 6, 4, -4, 0, 0, 18, -18},
	{7, -3}
};

void generate_data(int lev) {
	memset(flag, false, sizeof(flag));
	indata_n = lenin[lev - 1];
	for(int i = 1; i <= indata_n; i++)
		indata[i] = Data[lev - 1][i - 1];
	outdata_n = lenout[lev - 1];
	for(int i = 1; i <= outdata_n; i++)
		outdata[i] = Datao[lev - 1][i - 1];
}

bool illegal_sp(int x) {
	if(x < 0 || x > spaces_lim[level]) return true;
	return false;
}

void execute(int &r) {
	//system("pause");
	command &c = v[r];
	if(level == 1 && c.op >= 2) {
		error_on(r);
		return;
	}
	if(c.op == 0) { // inbox
		if(inp > indata_n) {
			//cout << indata_n << " " << inp << endl;
			status = END;
			return;
		}
		hand = indata[inp++];
	}
	else if(c.op == 1) { // outbox
		if(hand == EMPTY) {
			error_on(r);
			return;
		}
		ans[outp++] = hand;
		hand = EMPTY;
	}
	else if(c.op == 2) { // add
		if(hand == EMPTY || illegal_sp(c.x) || flag[c.x] == false) {
			error_on(r);
			return;
		}
		hand += spaces[c.x];
	}
	else if(c.op == 3) { // sub
		if(hand == EMPTY || illegal_sp(c.x) || flag[c.x] == false) {
			error_on(r);
			return;
		}
		hand -= spaces[c.x];
	}
	else if(c.op == 4) { // copyto
		if(hand == EMPTY || illegal_sp(c.x)) {
			error_on(r);
			return;
		}
		spaces[c.x] = hand; flag[c.x] = true;
	}
	else if(c.op == 5) { // copyfrom
		if(illegal_sp(c.x) || flag[c.x] == false) {
			error_on(r);
			return;
		}
		hand = spaces[c.x];
	}
	else if(c.op == 6) { // jump
		if(c.x <= 0 || c.x >= (int)v.size()) {
			error_on(r);
			return;
		}
		r = c.x;
		return; // 不再下一行
	}
	else if(c.op == 7) { // jumpifzero
		if(hand == EMPTY) {
			error_on(r);
			return;
		}
		if(c.x <= 0 || c.x >= (int)v.size()) {
			error_on(r);
			return;
		}
		if(hand == 0) {
			r = c.x;
			return;
		}
	}
	else {
		cout << "Fatal Error!" << endl;
		exit(0);
	}
	r++; // 下一行
}

int cmds;
void test() {
	int currow = 1;
	inp = 1, outp = 1;
	hand = EMPTY;
	cmds = 0;
	memset(flag, false, sizeof(flag));
	while(true) {
		execute(currow);
		cmds++;
		if(currow == (int)v.size()) break;
		if(status != 0) break;
		if(cmds > MAX_WAIT) {
			status = TLE;
			break;
		}
	}
}

const int TEST_ROUND = 1; // 测试次数

void output() {
	for(int i = 1; i <= indata_n; i++) {
		cout << indata[i] << ' ';
	}
	cout << endl;
	for(int i = 1; i <= outdata_n; i++) {
		cout << outdata[i] << ' ';
	}
	cout << endl;
	for(int i = 1; i <= outdata_n; i++) {
		cout << ans[i] << ' ';
	}
	cout << endl;
}

int convert(string &x) {
	if(x == "inbox") return 0;
	if(x == "outbox") return 1;
	if(x == "add") return 2;
	if(x == "sub") return 3;
	if(x == "copyto") return 4;
	if(x == "copyfrom") return 5;
	if(x == "jump") return 6;
	if(x == "jumpifzero") return 7;
	return -1;
}

/*
3
9
inbox
copyto 0
inbox
sub 0
jumpifzero 7
jump 1
copyfrom 0
outbox
jump 1
*/

int main() {
	cin >> level;
	v.push_back((command){-1, -1}); // start from line 1
	int n;
	cin >> n;
	for(int i = 1; i <= n; i++) {
		int op, x = -1; string str;
		cin >> str;
		op = convert(str);
		if(op >= 2) {
			cin >> x;
		}
		v.push_back((command){op, x});
		//cout << v[i].op << ' ' << v[i].x << endl;
	}
	//return 0;
	bool right = true;
	for(int i = 1; i <= TEST_ROUND; i++) {
		//cout << "Test #" << i << ": " << endl;
		generate_data(level);
		status = 0;
		test();
		if(status == TLE) {
			cout << "Endless Loop!" << endl;
			right = false;
			continue;
		}
		if(status && status != END) {
			if(status == FAIL) 
				cout << "Fail" << endl;
			else if(status > 0)
				cout << "Error on instruction " << status << endl;
			right = false;
		}
		else {
			for(int i = 1; i <= outdata_n; i++) {
				//cout << ans[i] << endl;
				if(ans[i] != outdata[i]) {
					fail();
					break;
				}
			}
			if(status == FAIL) {
				cout << "Fail" << endl;
				right = false;
				continue;
			}
			cout << "Success" << endl;
		}
	}
	//output();
	return 0;
}
/*
2
11
inbox
copyto 0
inbox
copyto 1
copyfrom 0
sub 1
outbox
copyfrom 1
sub 0
outbox
jump 1
*/