#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "utils.h"
using namespace std;



class Token {
public:
	string token = "";

	Token() {}
	Token(string s) {
		token = s;
	}

	Token& operator =(const string &s) {
		token = s;
		return *this;
	}

	char& operator[](int index) {
		return token[index];
	}

	//bool is(string s) {
	//	if (s == token) return true;
	//	return false;
	//}

	bool isAny(vector<string> v) {
		for (int i = 0; i < v.size(); i++) {
			if (token == v[i]) return true;
		}
		return false;
	}

	bool isAny(string* v, int nElements) {
		for (int i = 0; i < nElements; i++) {
			if (token == v[i]) return true;
		}
		return false;
	}

};

string toString(vector<Token> s) {
	string str = "";
	for (int i = 0; i < s.size(); i++) {
		str += s[i].token + " ";
	}
	return str;
}




class Line {
public:
	vector<Token> tokens;
	int indentation = 0;

	// Takes a string and converts to a token
	void add(string s) {
		tokens.push_back(Token(s));
	}

	string str() {
		return toString(tokens);
	}

	Token& operator[](int index) {
		return tokens[index];
	}
};



string toString(vector<Line> s) {
	string str = "";
	for (int i = 0; i < s.size(); i++) {
		str += s[i].str() + "\n";
	}
	return str;
}



