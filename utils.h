#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "dirent.h"


#define print cout<<
#define For(i, n) for(int i = 0; i<n; i++)
#define getPointerValue(x) *x
#define valueToPointer(x) &x

#define mapContains(map, key) (map->find(key) == map->end())

using namespace std;

inline string substring(string s, int start, int end) {
	if(start < end) return s.substr(start, end - start);
	else return "";
}

inline string substringFrom(string s, int start) {
	return substring(s, start, s.size());
}

inline string substringTo(string s, int end) {
	return substring(s, 0, end);
}

bool stringsEqualFrom(string base, int from, string toCompare) {
	int i = from;
	if (base.size() - from < toCompare.size()) return false;
	for (int j = 0; j < toCompare.size(); j++) {
		if (toCompare[j] != base[i]) {
			return false;
		}
		i++;
	}
	return true;
}

char firstLetter(string s) {
	if (s == "") {
		return 0;
	}
	else return s[0];
}

template<class T> void appendVector(vector<T>& base, vector<T> appendix) {
	base.insert(base.end(), appendix.begin(), appendix.end());
}


inline string quote(string s) {
	return "\"" + s + "\"";
}

inline string tag(string t, string cls, string src = "") {
	if(src == "") return "<" + t + " class=" + quote(cls) + ">";
	else return "<" + t + " class=" + quote(cls) + " src=" + quote(src) + ">";
}


string tabs(int nTabs) {					// Can be optimized hard
	string indentation = "";
	for (int i = 0; i < nTabs; i++) {
		indentation += "\t";
	}
	return indentation;
}

string toString(vector<string> v) {
	string ret = "";
	For(i, v.size()) {
		ret += v[i] + "\n";
	}
	return ret;
}

bool isEmptySpace(char c) {
	if (c == '\t' || c == ' ') return true;
	return false;
}

bool isEmptySpace(string s) {
	if (s.size() == 0) return true;
	for (int i = 0; i < s.size(); i++) {
		if (!isEmptySpace(s[i]))
			return false;
	}
	return true;
}

bool isEmptySpaceOrComment(string s) {
	if (s.size() == 0) return true;
	for (int i = 0; i < s.size() - 1; i++) {
		if (!isEmptySpace(s[i])) {
			if (s[i] == '/' && s[i + 1] == '/') {
				return true;
			}
			else return false;
		}
	}
	return true;
}

int trimSpaces(string s, int pos) {
	for (int i = pos; i < s.size(); i++) {
		if (!isEmptySpace(s[i])) return i;
	}
	return s.size() - 1;
}

string trimSpaces(string s) {
	int start = 0;
	int end = -1;
	for (int i = 0; i < s.size(); i++) {
		if (!isEmptySpace(s[i])) {
			start = i;
			break;
		}
	}
	for (int i = s.size() - 1; i >= 0; i--) {
		if (!isEmptySpace(s[i])) {
			end = i;
			break;
		}
	}
	if (start > end) return "";
	return substring(s, start, end + 1);
}

string trimSpacesAndComments(string s) {
	if (s.size() == 0) return "";
	for (int i = 0; i < s.size() - 1; i++) {
		if (s[i] == '/' && s[i + 1] == '/') {
			return trimSpaces(substringTo(s, i - 1));
		}
	}
	return trimSpaces(s);
}

int trimEmptyLinesFromBack(vector<string> lines, int from, int to) {
	int i = to;
	while (isEmptySpaceOrComment(lines[i]) && i > from) {
		i--;
	}
	return i;
}

int trimEmptyLinesFromFront(vector<string> lines, int from, int to) {
	int i = from;
	while (isEmptySpaceOrComment(lines[i]) && i < to) {
		i++;
	}
	return i;
}

bool isCharInString(char c, string s) {
	for (int i = 0; i < s.size(); i++) {
		if (c == s[i]) return true;
	}
	return false;
}

int isAnyCharInString(string s, string charArray) {
	for (int i = 0; i < charArray.size(); i++) {
		if (isCharInString(charArray[i], s)) return i;
	}
	return -1;
}

int trimSpacesAnd(string s, int pos, string other) {
	for (int i = pos; i < s.size(); i++) {
		if (!isEmptySpace(s[i]) && !isCharInString(s[i], other)) return i;
	}
	return s.size() - 1;
}

int findInString(string s, char c, int from = 0) {
	for (int i = from; i < s.size(); i++) {
		if(s[i] == c) return i;
	}
	//cout << "Character " << c << " not found. Returning -1\n";
	return -1;
}

int findInString(string s, string subs, int from = 0) {
	for (int i = from; i < s.size(); i++) {
		if (stringsEqualFrom(s, i, subs)) {
			return i;
		}
	}
	return -1;
}

template<class T> vector<T> arrayToVector(T a[], int nElements) {
	vector<T> ret;
	For(i, nElements) {
		ret.push_back(a[i]);
	}
	return ret;
}


enum CodeReadingState { ReadingTabs, ReadingSpaces, ReadingFirstWord };
string getFirstWord(string s) {
	if (s.length() == 0) return "";
	int i = 0;
	CodeReadingState state = ReadingTabs;
	int wordStart = 0;
	while (i < s.size()) {
		switch (state) {
		case ReadingTabs:
			if (!isEmptySpace(s[i])) {
				wordStart = i;
				state = ReadingFirstWord;
			}
			break;
		case ReadingFirstWord:
			if (isEmptySpace(s[i])) {
				return substring(s, wordStart, i);
			}
			break;
		}
		i++;
	}
	return substring(s, wordStart, s.size());
}

// Skips empty lines
vector<string> readFileLineByLine(string fileName, bool skipEmptyLines = true) {
	vector<string> ret;
	ifstream read(fileName);
	string currentLine;
	while (getline(read, currentLine)) {
		if (not (isEmptySpace(currentLine) && skipEmptyLines))
			ret.push_back(currentLine);
	}
	return ret;
}

string readFile(string fileName) {
	string ret = "";
	ifstream read(fileName);
	string currentLine;
	while (getline(read, currentLine)) {
		ret += currentLine + "\n";
	}
	return ret;
}

void writeFile(string message, string fileName) {
	ofstream write(fileName);
	write << message;
}

string changeExtension(string fileName, string newExtension) {
	for (int i = fileName.size(); i >= 0; i--) {
		if (fileName[i] == '.') {
			return substringTo(fileName, i) + "." + newExtension;
		}
	}
	return fileName;
}

bool isComment(string s) {
	int i = (trimSpaces(s, 0) == -1) ? 0 : trimSpaces(s, 0);
	if (s.size() - i - 2 <= 0) return false;	// might be wrong
	if (s[i] == '/' && s[i + 1] == '/') return true;
	return false;
}

bool isEmpty(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (!isEmptySpace(s[i])) {
			return false;
		}
	}
	return true;
}

bool startTheSame(string text, string as) {
	int i = trimSpaces(text, 0);
	if (text.size() - i < as.size()) return false;
	for (int j = 0; j < as.size(); j++) {
		if (as[j] != text[i]) {
			return false;
		}
		i++;
	}
	return true;
}





vector<string> splitString(string s, string delimiter, bool trim = false) {
	vector<string> ret;
	int delimiterSize = delimiter.size();
	int start = 0;
	int end = 0;
	for (int i = 0; i < s.size(); i++) {
		if (stringsEqualFrom(s, i, delimiter)) {
			end = i;
			ret.push_back(substring(s, start, end));
			if (trim) ret[ret.size() - 1] = trimSpaces(ret[ret.size() - 1]);
			start = i + delimiter.size();
		}
	}
	if (start < s.size() - 1) {
		ret.push_back(substringFrom(s, start));
		if (trim) ret[ret.size() - 1] = trimSpaces(ret[ret.size() - 1]);
	}
	return ret;
}



class Paragraph {
public:
	int start;
	int end;
	vector<string>* lines;
	Paragraph(int s, int e) {
		start = s;
		end = e;
	}

	Paragraph(int s, int e, vector<string> &_lines) {
		start = s;
		end = e;
		lines = valueToPointer(_lines);
	}

	int get(int i) {
		return start + i;
	}

	string getLine(int i) {
		return (getPointerValue(lines))[start + i];
	}

	string firstLine() {
		return (getPointerValue(lines))[start];
	}

	string toString() {
		string ret = "";
		for (int i = start; i <= end; i++) {
			ret += (getPointerValue(lines))[i] + "\n";
		}
		return ret;
	}

	int size() {
		return end - start + 1;
	}

};

enum LineParsingState {
	NoState,
	ReadingParagraph,
	ReadingEmptyLines
};

//void replaceAll(string &s, string sub, string replacement) {
//	string ret = "";
//	int index = 0;
//	int pos = 0;
//	while (true) {
//		pos = s.find(sub, index);
//		if (pos == string::npos) break;
//		ret += substring(s, index, pos);
//		ret += s.substr(pos, sub.size());
//		index = pos + sub.size();
//	}
//	s = ret;
//}

void replaceAll(string &s, string sub, string replacement) {
	string ret = "";
	int start = 0;
	for (int i = 0; i < s.size(); i++) {
		if (stringsEqualFrom(s, i, sub)) {
			ret += substring(s, start, i);
			ret += replacement;
			i += sub.size() - 1;
			start = i + 1;
		}
	}
	if (start < s.size() + 1) {
		ret += substring(s, start, start + s.size() - 1);
	}
	s = ret;
}



vector<Paragraph> findParagraphs(vector<string> &lines, int start, int end, bool recogniseClosures = false, bool recogniseSubClosures = false) {
	vector<Paragraph> ret;
	int i = start;
	int parStart = start;
	LineParsingState state = ReadingEmptyLines;
	for (int i = start; i <= end; i++) {
		string line = trimSpacesAndComments(lines[i]);
		if (state == ReadingEmptyLines) {
			if (line.size() > 0) {
				state = ReadingParagraph;
				parStart = i;
			}
		}
		else if (state == ReadingParagraph) {
			if (line.size() > 0) {
				if(recogniseClosures){
					char l = line[0];
					if (l == '{' || l == '[' || l == '|') {
						if(parStart <= i - 1) ret.push_back(Paragraph(parStart, i - 1, lines));
						parStart = i;
					}
					else if (recogniseSubClosures && l == '<') {
						if (parStart <= i - 1) ret.push_back(Paragraph(parStart, i - 1, lines));
						parStart = i;
					}
				}
			}
			else {
				ret.push_back(Paragraph(parStart, i - 1, lines));
				state = ReadingEmptyLines;
			}
		}
	}
	if (state == ReadingParagraph && parStart <= end) {
		ret.push_back(Paragraph(parStart, end, lines));
	}
	return ret;
}

vector<string> getAllFilesInFolder(string path) {
	DIR *dir;
	struct dirent *ent;
	vector<string> files = vector<string>();
	if ((dir = opendir(path.c_str())) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			//printf("%s\n", ent->d_name);
			string fileName = ent->d_name;
			if (fileName != "." && fileName != "..") {
				files.push_back(fileName);
			}
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("");
	}
	return files;
}