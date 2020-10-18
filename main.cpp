#include <iostream>
#include <vector>
#include <fstream>
#include "state.h"
#include "branch prediction.h"
using namespace std;

void error_and_die(int c) {
	if (c == 1) cerr << "invalid argument!" << endl << "sample: (program name), (file name)" << endl;
	else if (c == 2) cerr << "can't open file!" << endl;
	exit(c);
}

int main(int argc, char* argv[])
{
	if (argc != 2) error_and_die(1);
	ifstream inFile(argv[1], ios::in);
	if (!inFile) error_and_die(2);

	vector<string> instructions;
	string s;
	while (getline(inFile, s)) {
		instructions.push_back(s);
	}
	int size;
	cout << "entry size:";
	cin >> size;
	prediction pred(size);
	pred.parseText(instructions);
	pred.run_inst();
}