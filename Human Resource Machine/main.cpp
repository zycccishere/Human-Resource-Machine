//
//  main.cpp
//  mainprogram
//
//  Created by zhangyc on 2023/12/2.
//
//#include "iooperation.h"
#include "header.h"

Code cd[3]; // 3 panels

int notelines, cmd_panel = 0;
int outnum;
int expected_out[100]={0};
int spacenum;
string name;
string note;
int level;
int opnum;
int innum;
int input_line[100]={0};
int limit[8] = {0};

int speedx = 1;

class block {
public:
	block(){
		number = 0;
		block_pos.x = 3;
		block_pos.y = 3;
		appear = true;
	}
	block(int num){
		number = num;
		block_pos.x = 3;
		block_pos.y = 3;
		appear = true;
	}
	void num_appear(){
		appear = true;
	}
	void num_hide(){
		appear = false;
	}
	bool available(){//表示该块可访问
		return appear;
	}
	void set(int x,int y){
		block_pos.x = x;
		block_pos.y = y;
	}
	void up(){
		block_pos.x -= 1;
	}
	void down(){
		block_pos.x += 1;
	}
	void write(int num){
		number = num;
	}
	void show(){//1->show;0->seal
		if(block_pos.x <= 24) {
			cout << posit("+---+\n",block_pos.x - 1 + notelines, block_pos.y - 2);
			cout << posit("|   |\n",block_pos.x + notelines, block_pos.y - 2);
			cout << posit("+---+\n",block_pos.x + 1 + notelines, block_pos.y - 2);
		}
		else if(block_pos.x == 25) {
			cout << posit("+---+\n",block_pos.x - 1 + notelines, block_pos.y - 2);
			cout << posit("|   |\n",block_pos.x + notelines, block_pos.y - 2);
		}
		else if(block_pos.x == 26) {
			cout << posit("+---+\n", block_pos.x - 1 + notelines, block_pos.y - 2);
		}
		if(block_pos.x >= 26) return;
		if(appear){
			if(number < 10 && number > -10){
				cout << posit(to_string(number), block_pos.x + notelines, block_pos.y);
			}
			else{
				cout << posit(to_string(number), block_pos.x + notelines, block_pos.y - 1);
			}
		}
	}
	void hide(){
		if(block_pos.x <= 24){
			cout<<posit("     \n", block_pos.x - 1 + notelines, block_pos.y - 2);
			cout<<posit("     \n", block_pos.x + notelines, block_pos.y - 2);
			cout<<posit("     \n", block_pos.x + 1 + notelines, block_pos.y - 2);
		}
		else if(block_pos.x == 25){
			cout<<posit("     \n", block_pos.x - 1 + notelines, block_pos.y - 2);
			cout<<posit("     \n", block_pos.x + notelines, block_pos.y - 2);
		}
		else if(block_pos.x==26){
			cout<<posit("     \n", block_pos.x - 1 + notelines, block_pos.y - 2);
		}
		if(block_pos.x >= 26) return;
		if(appear){
			if(number < 10 && number > -10){
				cout << posit(" ", block_pos.x + notelines, block_pos.y);
			}
			else{
				cout << posit(" ", block_pos.x + notelines, block_pos.y - 1);
			}
		}
	}
	void erase(){
		int stx = block_pos.x - 3, sty = block_pos.y - 2;
		for(int i = 0; i < 3; i++) {
			set_xy(sty, stx + notelines + i);
			cout << "     ";
		}
	}
	void move(int to){
		switch (to) {
			case DOWN:
				block_pos.x++;
				break;
			case UP:
				block_pos.x--;
				break;
			case LEFT:
				block_pos.y--;
				break;
			case RIGHT:
				block_pos.y++;
				break;
			default:
				cout<<"error!"<<endl;
				break;
		}
	}
	int read(){
		return number;
	}
private:
	int number;
	pos block_pos;
	bool appear;//是否可见
};
vector <block> in;
vector <block> out;
vector <block> space;
block block_on_hand;

class man{
public:
	man(){status = 0; man_pos = {0, 0}; }
	void show(){
		int showx, showy;
		if(status == Hold_block) {
			block_on_hand.set(man_pos.x - 4, man_pos.y + 2);
			showx = man_pos.x-2; showy = man_pos.y - 2;
		}
		else if(status==Interact_space) {
			block_on_hand.set(man_pos.x + 3, man_pos.y + 2);
			showx = man_pos.x; showy = man_pos.y - 2;
		}
		else {
			showx = man_pos.x; showy = man_pos.y - 2;
		}
		for(int i = 0; i < robot_lines[status]; i++) {
			set_xy(showy, showx + i + notelines - 3);
			cout << handsome_robot[status][i];
		}
		if(status == Hold_block || status == Interact_space)
			block_on_hand.show();
	}
	void hide(){
		int showx, showy;
		int height;
		if(status == Hold_block) {
			showx = man_pos.x - 2 + notelines; showy = man_pos.y - 2;
			height = 10;
		}
		else {
			showx = man_pos.x - 1 + notelines; showy = man_pos.y - 2;
			height = 8;
		}
		for(int i = 0; i < height; i++) {
			set_xy(showy, showx + i - 3);
			cout << "         ";
		}
		if(status == Hold_block || status == Interact_space) {
			block_on_hand.set(man_pos.x - 3, man_pos.y + 2);
			block_on_hand.erase();
			block_on_hand.set(man_pos.x + 2, man_pos.y + 2);
			block_on_hand.erase();
		}
	}
	void change_status(int num){
		status = num;
	}
	void set(int x,int y){
		man_pos={x ,y};
	}
	
	pos get_pos(){
		return man_pos;
	}
	
	void move(int to){
		switch (to) {
			case DOWN:
				man_pos.x++;
				block_on_hand.move(to);
				break;
			case UP:
				man_pos.x--;
				block_on_hand.move(to);
				break;
			case LEFT:
				man_pos.y--;
				block_on_hand.move(to);
				break;
			case RIGHT:
				man_pos.y++;
				block_on_hand.move(to);
				break;
				
			default:
				cout<<"error!"<<endl;
				break;
		}
	}
private:
	int status = Norm;
	pos man_pos;//头的左上角位置(man4是画框的左上角位置)
};

man robot;
int decide_mov(int &rowdis,int &coldis){
	int ret=-1;
	int rowsgn=sgn(rowdis),colsgn=sgn(coldis);
	if(abs(rowdis)<abs(coldis)&&colsgn==1){
		ret=RIGHT;
		coldis--;
	}else if (abs(rowdis)<abs(coldis)&&colsgn==-1){
		ret=LEFT;
		coldis++;
	}else if (abs(rowdis)>abs(coldis)&&rowsgn==1){
		ret=DOWN;
		rowdis--;
	}else if (abs(rowdis)>abs(coldis)&&rowsgn==-1){
		ret=UP;
		rowdis++;
	}
	return ret;
}

void go(pos from,pos to){ // robot move
	int row_distance=to.x-from.x;
	int col_distance=to.y-from.y;
	if(row_distance > 0) { // down
		while(row_distance > 0) {
			robot.show();
			Sleep(speedx == 0 ? 1 : SHORT_WAIT / speedx);
			robot.hide();
			robot.move(DOWN);
			row_distance--;
		}
	}
	else if(row_distance < 0) { // up
		while(row_distance < 0) {
			robot.show();
			Sleep(speedx == 0 ? 1 : SHORT_WAIT / speedx);
			robot.hide();
			robot.move(UP);
			row_distance++;
		}
	}

	if(col_distance > 0) { // right
		while(col_distance > 0) {
			robot.show();
			Sleep(speedx == 0 ? 1 : SHORT_WAIT / speedx);
			robot.hide();
			robot.move(RIGHT);
			col_distance--;
		}
	}
	else if(col_distance < 0) { // left
		while(col_distance < 0) {
			robot.show();
			Sleep(speedx == 0 ? 1 : SHORT_WAIT / speedx);
			robot.hide();
			robot.move(LEFT);
			col_distance++;
		}
	}
	robot.show();
}

void basic_output(){
	int j = 0;
	notelines = (note.length() / NOTE_WIDTH) + (note.length() % NOTE_WIDTH ? 1 : 0) + 2;
	cout << "\033[" + to_string(YELLOW + 60) + "m";
	for(int i = 1; i <= OUTLINE_WIDTH; i++) cout << '*';
	cout << endl << "\033[0m";

	for(int i = 0; i < (int)note.length(); i++) {
		if(i % NOTE_WIDTH == 0) cout << convert("* ", YELLOW, FORE_BRIGHT);
		cout << note[i];
		if((i + 1) % NOTE_WIDTH == 0) cout << convert(" *", YELLOW, FORE_BRIGHT) << endl;
	}
	int T = NOTE_WIDTH - ((int)note.length() % NOTE_WIDTH);
	if(T != 0) {
		for(int i = 1; i <= T; i++) cout << ' ';
		cout << convert(" *", YELLOW, FORE_BRIGHT);
	}
	cout << endl;
	cout << "\033[" + to_string(YELLOW + 60) + "m";
	for(int i = 1; i <= OUTLINE_WIDTH; i++) cout << '*';
	cout << endl << "\033[0m";
	cout << outline;
	cout << posit(level == -1 ? "Customized" : to_string(level), levpos.x + notelines, levpos.y);
	cout << posit(name, nampos.x + notelines, nampos.y);
	cout << posit(to_string(cmd_panel + 1), panel.x + notelines, panel.y);
	
	for(int i = 0; i <= 7; i++){
		if(limit[i]){
			j++;
			cout << posit(to_string(j) + ". " + op[i], valpos.x + j - 1 + notelines, valpos.y);
		}
	}
}

int line_limit, compute_limit;
void getdata(){
	int val;
	ifstream fin;
	fin.open("./temp/level.dat");
	fin >> level;
	fin.ignore(numeric_limits<streamsize>::max(), '\n');
	getline(fin,name);
	getline(fin,note);
	fin >> innum;
	for(int i = 0; i < innum; i++){
		fin >> input_line[innum-1-i];
	}
	fin >> outnum;
	for(int i = 0; i < outnum; i++){
		fin>>expected_out[i];
	}
	fin >> spacenum;
	for(int i = 0; i < spacenum; i++){
		block a;
		space.push_back(a);
	}
	for(int i = 0; i < spacenum; i++) {
		int tmp;
		fin >> tmp;
		if(tmp != 114514) {
			space[i].num_appear();
			space[i].write(tmp);
		}
		else space[i].num_hide();
	}
	fin >> opnum;
	for(int i = 0; i < opnum; i++){
		fin >> val;
		limit[val] = 1;
	}

	fin >> line_limit >> compute_limit;
	fin.close();
}

void show_space(){
	for(int i = 0; i < spacenum; i++){
		space[i].show();
	}
}

void start(){
	set_xy(newoppos.y, newoppos.x + notelines);
	const string instr = 	"You can now edit your code by entering the following commands without \"x.\", and end by entering 'Q': (<> - must   [] - optional)\n"
							"\t1. new <command_name> [op_number]\t\t\t---add a command to the back of your code.\n"
							"\t2. ins <position> <command_name> [op_number]\t\t---insert a command after line position.\n"
							"\t3. del <position>\t\t\t\t\t---delete line position.\n";
	cout << instr;
	while(true) {
		show_file_op(-1, youpos, cd[cmd_panel].cmds, notelines); // 刷新命令栏目
		set_xy(newoppos.y, newoppos.x + notelines + 4);
		cout << "                                                   " << endl << "                                                   " << endl 
		<< "                                                   " << endl;
		set_xy(newoppos.y, newoppos.x + notelines + 4);
		cout << "Please Enter:" << endl;
		string inp;
		show_cursor();
		getline(cin, inp);
		if(inp[0] == 'Q' && inp.length() == 1) {
			set_xy(newoppos.y, newoppos.x + notelines + 4);
			cout << "                                                   " << endl << "                                                   " << endl 
			<< "                                                   " << endl;
			break;
		}
		Console_Cmd ans = analyze_console_input(inp);
		if(ans.op == -1 || ans.content.length() >= 16) {
			cout << "Invalid Command!" << endl;
			Sleep(NORMAL_WAIT);
			continue;
		}
		if(ans.op == 0) { // new
			if((int)cd[cmd_panel].cmds.size() >= 20) {
				cout << "Too Many Lines!" << endl;
				Sleep(NORMAL_WAIT);
				continue;
			}
			if(ans.x2 == -2)
				cd[cmd_panel].cmds.push_back((operation){-1, -1, file_syntax(ans.content)});
			else 
				cd[cmd_panel].cmds.push_back((operation){ans.x1, ans.x2, ""});
			cout << "Successfully Executed!" << endl;
			Sleep(NORMAL_WAIT);
			continue;
		}
		if(ans.op == 1) { // ins
			if(ans.x1 > (int)cd[cmd_panel].cmds.size()) {
				cout << "Invalid Command!" << endl;
				Sleep(NORMAL_WAIT);
				continue;
			}
			if((int)cd[cmd_panel].cmds.size() >= 20) {
				cout << "Too Many Lines!" << endl;
				Sleep(NORMAL_WAIT);
				continue;
			}
			if(ans.x2 == -2)
				cd[cmd_panel].cmds.insert(cd[cmd_panel].cmds.begin() + ans.x1, (operation){-1, -1, file_syntax(ans.content)});
			else
				cd[cmd_panel].cmds.insert(cd[cmd_panel].cmds.begin() + ans.x1, (operation){ans.x2, ans.x3, ""});
			cout << "Successfully Executed!" << endl;
			Sleep(NORMAL_WAIT);
			continue;
		}
		if(ans.op == 2) { // del
			if(ans.x1 <= 0 || ans.x1 > (int)cd[cmd_panel].cmds.size()) {
				cout << "Invalid Command!" << endl;
				Sleep(NORMAL_WAIT);
				continue;
			}
			cd[cmd_panel].cmds.erase(cd[cmd_panel].cmds.begin() + ans.x1 - 1);
			cout << "Successfully Executed!" << endl;
			Sleep(NORMAL_WAIT);
			continue;
		}
	}
	set_xy(term_pos.y, term_pos.x + notelines);
	cout << 
	"                                                                                                                                                " << endl <<
	"                                                                                                                                                " << endl << 
	"                                                                                                                                                " << endl << 
	"                                                                                                                                                " << endl;
	hide_cursor();
}

bool test_output(){ // 输出是否和标答一致
	if(outnum != (int)out.size())
		return false;
	for(int i = 0; i < (int)out.size(); i++){
		if(out[i].read() != expected_out[i])
			return false;
	}
	return true;
}

void show_inbox(){
	for(int i = 0; i < (int)in.size(); i++){
		in[i].show();
	}
}
void erase_inbox(){
	for(int i = 0; i < (int)in.size(); i++){
		in[i].hide();
	}
}
void show_outbox(){
	for(int i = 0; i < (int)out.size(); i++){
		out[i].show();
	}
}
void erase_outbox(){
	for(int i = 0; i < (int)out.size(); i++){
		out[i].hide();
	}
}
bool in_all_up(){ // true:正常结束  false:还需继续
	if(in.size() == 0) {
		return true;
	}
	erase_inbox();
	block_on_hand = in.back().read();
	block_on_hand.num_appear();
	in.pop_back();
	show_inbox();
	for(int times = 1; times <= 3; times++) {
		Sleep(speedx == 0 ? 1 : SHORT_WAIT / speedx);
		erase_inbox();
		for(int i = 0; i < (int)in.size(); i++){
			in[i].up();
		}
		show_inbox();
	}
	return false;
}
bool out_all_down(){ // true:异常结束 false:正常结束
	if(!block_on_hand.available()){
		return true;
	}
	for(int times = 1; times <= 3; times++) {
		erase_outbox();
		for(int i = 0; i < (int)out.size(); i++){
			out[i].down();
		}
		show_outbox();
		Sleep(speedx == 0 ? 1 : SHORT_WAIT / speedx);
	}
	block a = block_on_hand.read();
	a.set(outputbegin.x, outputbegin.y);
	out.push_back(a);
	block_on_hand.num_hide();
	return false;
}
void init_set(){
	for(int i = 0; i < (int)in.size(); i++){
		in[i].set(inputbegin.x + int((in.size() - i - 1)) * yinterval, inputbegin.y);
	}
}

int pointer = 0;
int compute_times;

int simulate(){
	if(compute_times >= 200) return ENDLESS_LOOP;
	int &label = cd[cmd_panel].cmds[pointer].label;
	int &num = cd[cmd_panel].cmds[pointer].num;
	if(label >= 0 && limit[label] == false) { // not in available command
		return pointer;
	}
	if(label == 0) { // inbox
		go(robot.get_pos(), in_pos);
		robot.hide();
		robot.change_status(Fetch_inbox);
		robot.show();
		Sleep(speedx == 0 ? 1 : (4 * SHORT_WAIT) / speedx);
		bool state = in_all_up();
		if(state) { // 正常结束
			robot.hide();
			robot.change_status(Norm);
			robot.show();
			return END_NORMAL;
		}
		robot.hide();
		robot.change_status(Hold_block); //这里有个问题，就是如果异常结束如inbox无东西，依然会先出动画后判定，state传值方式可能得改
		robot.show();
		pointer++; compute_times++;
	}
	else if(label == 1) { // outbox
		go(robot.get_pos(), out_pos);
		
		bool state = out_all_down();
		robot.hide();
		robot.change_status(Put_outbox);
		robot.show();
		Sleep(speedx == 0 ? 1 : (4 * SHORT_WAIT) / speedx);
		robot.hide();
		robot.change_status(Norm);
		robot.show();
		if(state) return pointer;
		pointer++; compute_times++;
	}
	else if(label == 2) { // add
		go(robot.get_pos(), space_loc[num]);
		if(!space[num].available() || !block_on_hand.available() || num>spacenum - 1) {
			return pointer;
		}
		robot.change_status(Interact_space);
		robot.hide();
		robot.show();
		Sleep(speedx == 0 ? 1 : (4 * SHORT_WAIT) / speedx);
		block_on_hand.write(block_on_hand.read() + space[num].read()); // 进行加操作
		block_on_hand.show();
		Sleep(speedx == 0 ? 1 : (4 * SHORT_WAIT) / speedx);
		robot.change_status(Hold_block);
		robot.hide();
		robot.show();
		
		pointer++; compute_times++;
	}
	else if(label == 3) { // sub
		go(robot.get_pos(), space_loc[num]);
		if(!space[num].available() || !block_on_hand.available() || num>spacenum - 1) {
			return pointer;
		}
		robot.change_status(Interact_space);
		robot.hide();
		robot.show();
		Sleep(speedx == 0 ? 1 : (4 * SHORT_WAIT) / speedx);
		block_on_hand.write(block_on_hand.read() - space[num].read()); // 进行减操作
		block_on_hand.show();
		Sleep(speedx == 0 ? 1 : (4 * SHORT_WAIT) / speedx);
		robot.change_status(Hold_block);
		robot.hide();
		robot.show();
		
		pointer++; compute_times++;
	}
	else if(label == 4) { // copyto
		go(robot.get_pos(), space_loc[num]);
		if(num > spacenum - 1 || !block_on_hand.available()) {
			return pointer;
		}
		robot.hide();
		robot.change_status(Interact_space);
		robot.show();
		Sleep(speedx == 0 ? 1 : (4 * SHORT_WAIT) / speedx);
		space[num].num_appear();
		space[num].write(block_on_hand.read());
		space[num].show();
		Sleep(speedx == 0 ? 1 : (4 * SHORT_WAIT) / speedx);
		robot.hide();
		robot.change_status(Hold_block);
		robot.show();
		pointer++; compute_times++;
	}
	else if(label == 5) { // copyfrom
		go(robot.get_pos(), space_loc[num]);
		if(num > spacenum - 1 || !space[num].available()){
			return pointer;
		}
		robot.change_status(Interact_space);
		robot.hide();
		robot.show();
		Sleep(speedx == 0 ? 1 : (4 * SHORT_WAIT) / speedx);
		block_on_hand.num_appear();
		block_on_hand.write(space[num].read());
		block_on_hand.show();
		Sleep(speedx == 0 ? 1 : (4 * SHORT_WAIT) / speedx);
		robot.change_status(Hold_block);
		robot.hide();
		robot.show();
		pointer++; compute_times++;
	}
	else if(label == 6) { // jump
		if(num >= int(cd[cmd_panel].cmds.size())){
			return pointer;
		}
		pointer = num - 1; compute_times++;
	}
	else if(label == 7) { // jumpifzero
		if(num >= int(cd[cmd_panel].cmds.size()) || !block_on_hand.available()){
			return pointer;
		}
		if(block_on_hand.read() == 0){
			pointer = num - 1; compute_times++;
		} else {
			pointer++; compute_times++;
		}
	}
	else if(label == -1) {
		return pointer;
	}
	return CONTINUE;
}

void update_code_file(bool pass){//将更改后的指令重写入code.cd
	ofstream fout("./temp/code.cd", ios::out);
	if(pass) fout << level << endl; // success
	else fout << 0 << endl;
	for(int i = 0; i < MAX_PANEL; i++) {
		cd[i].lines = cd[i].cmds.size();
		fout << cd[i].lines << endl;
		for(int j = 0; j < cd[i].lines; j++) {
			int &op = cd[i].cmds[j].label;
			fout << op;
			if(op == -1) fout << ' ' << file_syntax(cd[i].cmds[j].content) << endl;
			else if(op >= 2) fout << ' ' << cd[i].cmds[j].num << endl;
			else fout << endl;
		}
		fout << endl;
	}
}

void terminate_output(string x, bool pass) {
	set_xy(term_pos.y, term_pos.x + notelines);
	cout << "                                               ";
	set_xy(term_pos.y, term_pos.x + notelines);
	cout << x << endl;
	if(pass) {
		int lines = cd[cmd_panel].cmds.size();
		if(line_limit != -1) {
			cout << "Lines of Codes:     " << lines << "\t\tRequirement: "
			 << convert(to_string(line_limit), lines <= line_limit ? GREEN : RED, FORE_BRIGHT) << endl;
		}
		if(compute_limit != -1) {
			cout << "Times of Computing: " << compute_times << "\t\tRequirement: "
			 << convert(to_string(compute_limit), compute_times <= compute_limit ? GREEN : RED, FORE_BRIGHT) << endl;
		}
	}
	update_code_file(pass);
	system("pause");
	exit(0);
}

int main() {
	full_screen();
	hide_cursor();
	Sleep(100);
	block_on_hand.num_hide();

	system("cls");
	getdata(); // read level.dat
	getfileop(cd); // read code.cd
	system("cls");
	basic_output(); // 界面
	
	for(int i = 0; i < spacenum; i++){
		space[i].set(spapos.x, int(spapos.y - (spacenum - 1) *2.5 + i * 5));
		space_loc.push_back((pos){spapos.x - 7, int(spapos.y - (spacenum - 1) *2.5 + i * 5) - 2});
	}

	for(int i = 0; i < innum; i++){
		block a;
		a.write(input_line[i]);
		in.push_back(a);
	}
	
	//此时in是block容器，tmp.out是输出序列的答案
	show_space();

	show_file_op(-1, youpos, cd[cmd_panel].cmds, notelines);
	init_set();
	show_inbox();
	show_outbox();

	robot.change_status(Norm);
	robot.set(init_pos.x + notelines, init_pos.y);
	robot.show();

	set_xy(0, newoppos.x + notelines);
	const string instruction = 	"Press left and right keys to select a panel. Press Enter to confirm.\n"
								"If your code is ready, press \'S\' to start! You can also press Esc to go back.\n"
								"Up and Down Keys can be used to adjust speed.";
	cout << instruction;
	show_spd(speedx, notelines);
	while(true) {
		if(kbhit() && is_pressing(VK_RIGHT)) {
			clear_op(youpos, cd[cmd_panel].cmds, notelines);
			cmd_panel = (cmd_panel + 1) % MAX_PANEL;
			cout << posit(to_string(cmd_panel + 1), panel.x + notelines, panel.y);
			show_file_op(-1, youpos, cd[cmd_panel].cmds, notelines);
			while(is_pressing(VK_RIGHT));
		}
		if(kbhit() && is_pressing(VK_LEFT)) {
			clear_op(youpos, cd[cmd_panel].cmds, notelines);
			cmd_panel = (cmd_panel - 1 + 3) % MAX_PANEL;
			cout << posit(to_string(cmd_panel + 1), panel.x + notelines, panel.y);
			show_file_op(-1, youpos, cd[cmd_panel].cmds, notelines);
			while(is_pressing(VK_LEFT));
		}

		if(kbhit() && is_pressing(VK_UP)) {
			speedx++;
			if(speedx > 64) {
				speedx = 0;
			}
			show_spd(speedx, notelines);
			while(is_pressing(VK_UP));
		}
		if(kbhit() && is_pressing(VK_DOWN)) {
			speedx--;
			if(speedx < 0) {
				speedx = 64;
			}
			show_spd(speedx, notelines);
			while(is_pressing(VK_DOWN));
		}

		if(kbhit() && is_pressing(VK_RETURN) && !is_pressing(VK_LEFT) && !is_pressing(VK_RIGHT)) {
			Sleep(10);
			if(!kbhit() || !is_pressing(VK_RETURN)) continue;
			clear_buffer();
			set_xy(0, newoppos.x + notelines);
			cout << "                                                                                      " << endl << 
					"                                                                                      " << endl << 
					"                                                                                      " << endl;
			start();
			set_xy(0, newoppos.x + notelines);
			cout << instruction;
			show_spd(speedx, notelines);
			while(kbhit());
		}
		if(kbhit() && is_pressing(VK_ESCAPE)) {
			Sleep(10);
			if(!kbhit() || !is_pressing(VK_ESCAPE)) continue;
			update_code_file(false);
			return 0;
		}
		if(kbhit() && is_pressing('S')) {
			Sleep(10);
			if(!kbhit() || !is_pressing('S')) continue;
			set_xy(0, newoppos.x + notelines);
			cout << "                                                                               " << endl << 
					"                                                                               " << endl << 
					"                                                                               " << endl;
			break;
		}
	}

	Sleep(NORMAL_WAIT);
	show_spd(speedx, notelines);
	while(true){
		show_file_op(pointer, youpos, cd[cmd_panel].cmds, notelines);
		show_inbox();
		show_outbox();

		int val = (pointer >= (int)cd[cmd_panel].cmds.size() ? END_NORMAL : simulate());
		if(val == END_NORMAL) { // 程序正常结束
			if(test_output()){
				Sleep(NORMAL_WAIT);
				terminate_output(convert("Success", WHITE, GREEN, FORE_BRIGHT | BACK_BRIGHT), true);
			}
			else {
				Sleep(NORMAL_WAIT);
				terminate_output(convert("Fail", WHITE, RED, FORE_BRIGHT | BACK_BRIGHT), false);
			}
		}
		else if(val == ENDLESS_LOOP) {
			Sleep(NORMAL_WAIT);
			terminate_output(convert("Endless Loop", WHITE, CYAN, FORE_BRIGHT | BACK_BRIGHT), false);
		}
		else if(val != CONTINUE){ // error on instruction X
			Sleep(NORMAL_WAIT);
			terminate_output(convert("Error on instrction " + to_string(val + 1), WHITE, MAGENTA, FORE_BRIGHT | BACK_BRIGHT), false);
		}
		show_inbox();
		show_outbox();
		if(speedx == 0) {
			set_xy(term_pos.y, term_pos.x + notelines);
			system("pause");
		}
		else Sleep(NORMAL_WAIT / speedx);
	}
	return 0;
}