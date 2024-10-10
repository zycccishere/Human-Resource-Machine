/*
 * 程序正确性检验器，调用时需要 judge.exe level_num，并且需要在temp文件夹下生成好code.cd
 */

#include <fstream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <random>
#include <iostream>
using namespace std;

#define FAIL (-1)
#define TLE (-2)

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

void generate_data(int lev) {
	memset(flag, false, sizeof(flag));
	default_random_engine e(time(NULL));
	if(lev == 1) {
		uniform_int_distribution <int> u(10, 100);
		indata_n = u(e);
		//indata_n = 3;
		uniform_int_distribution <int> r(-10000, 10000);
		for(int i = 1; i <= indata_n; i++) {
			indata[i] = r(e);
			outdata[i] = indata[i]; // Level 1
		}
		outdata_n = indata_n;
	}
	else if(lev == 2) {
		uniform_int_distribution <int> u(5, 50);
		indata_n = u(e) * 2; // 要求为偶数

		uniform_int_distribution <int> r(-5000, 5000);
		for(int i = 1; i <= indata_n; i += 2) {
			indata[i] = r(e);
			indata[i + 1] = r(e);
			outdata[i] = indata[i] - indata[i + 1];
			outdata[i + 1] = -outdata[i];
		}
		outdata_n = indata_n;
	}
	else if(lev == 3) {
		uniform_int_distribution <int> u(5, 50);
		indata_n = u(e) * 2; // 要求为偶数
		uniform_int_distribution <int> r(-5000, 5000);
		uniform_int_distribution <int> poss(1, 5);

		outdata_n = 0;
		for(int i = 1; i <= indata_n; i += 2) {
			if(poss(e) == 1) { //中奖了
				indata[i] = indata[i + 1] = r(e);
				outdata[++outdata_n] = indata[i];
			}
			else {
				indata[i] = r(e);
				indata[i + 1] = r(e);
			}
		}
	}
}

bool illegal_sp(int x) {
	if(x < 0 || x > spaces_lim[level]) return true;
	return false;
}

void execute(int &r) {
	//system("pause");
	command &c = v[r];
	if(c.op == 0) { // inbox
		if(inp > indata_n) {
			//cout << indata_n << " " << inp << endl;
			error_on(r);
			return;
		}
		hand = indata[inp++];
	}
	else if(c.op == 1) { // outbox
		if(hand == -1) {
			error_on(r);
			return;
		}
		ans[outp++] = hand;
		hand = -1;
		if(outp == outdata_n + 1) {
			return;
		}		
	}
	else if(c.op == 2) { // add
		if(hand == -1 || illegal_sp(c.x) || flag[c.x] == false) {
			error_on(r);
			return;
		}
		hand += spaces[c.x];
	}
	else if(c.op == 3) { // sub
		if(hand == -1 || illegal_sp(c.x) || flag[c.x] == false) {
			error_on(r);
			return;
		}
		hand -= spaces[c.x];
	}
	else if(c.op == 4) { // copyto
		if(hand == -1 || illegal_sp(c.x)) {
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
		r = c.x;
		return; // 不再下一行
	}
	else if(c.op == 7) { // jumpifzero
		if(hand == -1) {
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
	hand = -1;
	cmds = 0;
	memset(flag, false, sizeof(flag));
	while(true) {
		execute(currow);
		cmds++;
		if(currow == (int)v.size()) break;
		if(status != 0) break;
		if(outp == outdata_n + 1) break;
		if(cmds > MAX_WAIT) {
			status = TLE;
			break;
		}
	}
}

const int TEST_ROUND = 10; // 测试次数

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

int main(int argc, char const *argv[]) {
	ifstream fin("./temp/code.cd", ios::in);
	//level = atoi(argv[1]);
	level = 3;
	int n;
	fin >> n;
	v.push_back((command){-1, -1}); // start from line 1
	for(int i = 1; i <= n; i++) {
		int op, x = -1;
		fin >> op;
		if(op >= 2) {
			fin >> x;
		}
		v.push_back((command){op, x});
		//cout << v[i].op << ' ' << v[i].x << endl;
	}
	//return 0;
	bool right = true;
	for(int i = 1; i <= TEST_ROUND; i++) {
		cout << "Test #" << i << ": " << endl;
		generate_data(level);
		status = 0;
		test();
		if(status == TLE) {
			cout << "Endless Loop!" << endl;
			right = false;
			continue;
		}
		if(status != 0) {
			if(status == FAIL) 
				cout << "Fail" << endl;
			else if(status != 0)
				cout << "Error on instrction " << status << endl;
			right = false;
		}
		else {
			for(int i = 1; i <= outdata_n; i++) {
				if(ans[i] != outdata[i]) {
					fail();
					break;
				}
			}
			if(status) {
				cout << "Fail" << endl;
				right = false;
				continue;
			}
			cout << "Success" << endl;
		}
	}
	if(right) {
		cout << "Congrats! Accepted." << endl;
	}
	else {
		cout << "Uh oh. Unaccepted." << endl;
	}
	//output();
	return 0;
}
/*
3
8
0
4 0
0
3 0
7 7
6 1
1
6 1
*/