#pragma once
#include <string>
class state2bit
{
public:
	state2bit() {};

	//increase s until s = 11
	state2bit& operator++() {
		if (s[0] && s[1]);
		else {
			if (s[0]) s[1] = !s[1];
			s[0] = !s[0];
		}
		return *this;
	}

	//decrease s until s = 00
	state2bit& operator--() {
		if (!s[0] && !s[1]);
		else {
			if (!s[0]) s[1] = !s[1];
			s[0] = !s[0];
		}
		return *this;
	}

	//return value of this
	operator bool() const{
		return s[1];
	}

	//return inverse value of this
	bool operator!() const {
		return !s[1];
	}

	char* getState() {
		if (s[1] && s[0]) return ST;
		else if (s[1] && !s[0]) return WT;
		else if (!s[1] && s[0]) return WN;
		else if (!s[1] && !s[0]) return SN;
	}
private:
	bool s[2] = { false };
	char SN[3] = "SN";
	char WN[3] = "WN";
	char WT[3] = "WT";
	char ST[3] = "ST";
};

class state {
public:
	state() {
		stateBits[0] = stateBits[1] = false;
	}

	//call state2bit::operator++()
	state& operator++() {
		++stateList[stateBits[0] * 1 + stateBits[1] * 2];
		return *this;
	}

	//call state2bit::operator--()
	state& operator--() {
		--stateList[stateBits[0] * 1 + stateBits[1] * 2];
		return *this;
	}

	//return value of current state
	operator bool() const {
		return stateList[stateBits[0] * 1 + stateBits[1] * 2];
	}

	//return inverse value of current state
	bool operator!() const {
		return !stateList[stateBits[0] * 1 + stateBits[1] * 2];
	}

	//push value to stateBits
	void push(bool b) {
		stateBits[1] = stateBits[0];
		stateBits[0] = b;
	}

	void print() {
		printf("(%d%d, %s, %s, %s, %s)",
			stateBits[1],
			stateBits[0],
			stateList[0].getState(),
			stateList[1].getState(),
			stateList[2].getState(),
			stateList[3].getState());
	}

private:
	bool stateBits[2]; //for current state
	state2bit stateList[4]; //2 bit state list
};

