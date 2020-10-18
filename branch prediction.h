#pragma once
#include "state.h"
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

class prediction
{
public:
	using func_type = function<void(int, int, int)>;
	using instruction = tuple<func_type, int, int, int>;
	using instruction_set = vector<instruction>;
	struct istate {
		int err = 0;
		state s;
	};

	//constructor with size of entry
	prediction(int size) {
		isBrh = false;
		jmp = false;
		pc = 0;
		reg.assign(32, 0);
		inst["li"] = std::bind(&prediction::li, this, placeholders::_1, placeholders::_2, placeholders::_3);
		inst["beq"] = std::bind(&prediction::beq, this, placeholders::_1, placeholders::_2, placeholders::_3);
		inst["addi"] = std::bind(&prediction::addi, this, placeholders::_1, placeholders::_2, placeholders::_3);
		BHT.resize(size);
	}

	

	//parse text
	//deal with Label, register, and set format
	void parseText(vector<string>);

	//run program and predict
	void run_inst() {
		int last_pc;
		while (pc < instructions.size()) {
			last_pc = pc;
			isBrh = false;
			jmp = false;
			get<0>(instructions[pc])(get<1>(instructions[pc]), get<2>(instructions[pc]), get<3>(instructions[pc]));
			if (isBrh) {
				cout << "entry:" << last_pc % BHT.size() << setw(20) << text[last_pc] << endl;
				getState(last_pc).s.print();
				cout << ' ' << (getState(last_pc).s ? 'T' : 'N') << ' ' << (jmp ? 'T' : 'N') << "         ";
				if (jmp != getState(last_pc).s) {
					++getState(last_pc).err;
					if (jmp) ++getState(last_pc).s;
					else --getState(last_pc).s;
				}
				cout << "misprediction: " << getState(last_pc).err << endl << endl;
				getState(last_pc).s.push(jmp);
			}
		}
	}

	
private:
	//counter for inst, means position of program
	int pc; 

	//for branch
	bool isBrh;

	//for jump
	bool jmp;

	//program
	instruction_set instructions;

	//text
	vector<string> text;

	//BHT with (int, state)
	vector<istate> BHT; 

	//register 0-31
	vector<int> reg; 

	//instruction with (int, int, int) types parameter
	map<string, func_type> inst;

	//push instruction into inst
	//string str: instruction name
	//int first, int second, int third: each three parameter
	void set(string str, int first, int second, int third) {
		instructions.push_back(instruction(inst[str], first, second, third));
	}

	istate& getState(int i) { return BHT[i % BHT.size()]; }

	void beq(int, int, int);
	void li(int, int, int);
	void addi(int, int, int);
	
	void jump(int);
	bool getReg(string&, int&);
};

void prediction::parseText(vector<string> sText)
{
	map<string, int> label;
	
	size_t pos;
	int p = 0;
	for (vector<string>::iterator it = sText.begin(); it != sText.end(); ) {
		pos = (*it).find_first_of('/');
		if (pos != string::npos)
			(*it).resize(pos);
		for (int j = 0; j < (*it).size(); ++j) { //convert ',' into ' '
			if ((*it)[j] == '\t') (*it)[j] = ' ';
		}
		pos = (*it).find(':');
		if (pos != string::npos) //if find label, put position into [label]
			label[(*it).substr((*it).find_first_not_of(' '), pos)] = p;

		if ((*it)[(*it).find_last_not_of(' ')] == ':') { //if not find instruction erase current line
			it = sText.erase(it);
		}
		else {
			++pos;
			if (pos == string::npos)
				pos = 0;
			text.push_back((*it).substr((*it).find_first_not_of(' ', pos)));
			++it;
			++p;
		}
	}

	string str;
	string strbuf;
	int first = 0, second = 0, third = 0;
	stringstream ss;
	int buf;
	for (int i = 0; i < text.size(); ++i) {
		str = text[i];
		for (int j = 0; j < str.size(); ++j) { //convert ',' into ' '
			if (str[j] == ',') str[j] = ' ';
			if (str[j] == '\t') str[j] = ' ';
		}
		ss << str;
		ss >> str;
		if (ss >> strbuf) 
			first = getReg(strbuf, buf) ? buf : label[strbuf] - i;
		if (ss >> strbuf)
			second = getReg(strbuf, buf) ? buf : label[strbuf] - i;
		if (ss >> strbuf)
			third = getReg(strbuf, buf) ? buf : label[strbuf] - i;

		set(str, first, second, third);
		ss.clear();
	}
}

bool prediction::getReg(string& str, int& buf)
{
	if (str[0] == 'R') {
		int i = -1;
		if (str.size() == 2) i = str[1] - '0';
		else if (str.size() == 3) i = (str[1] - '0') * 10 + (str[2] - '0');
		buf = i;
		if (i >= 0 && i <= 31) return true;
	}
	else if (atoi(str.c_str())) {
		buf = atoi(str.c_str());
		return true;
	}
	return false;
}

void prediction::beq(int rs1, int rs2, int offset)
{
	isBrh = true;
	if (reg[rs1] == reg[rs2]) jump(offset);
	else ++pc;
}

void prediction::li(int rd, int imm, int)
{
	reg[rd] = imm;
	++pc;
}

void prediction::addi(int rd, int rs1, int imm)
{
	reg[rd] = reg[rs1] + imm;
	++pc;
}

inline void prediction::jump(int offset)
{
	jmp = true;
	pc += offset;
}
