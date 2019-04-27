#pragma once
#include "utils.h"
#include "DOM.h"

const string DIGITS = "01234567890";
const string DICE_COLOR = "FF5500";

bool isDigit(char c) {
	if (isCharInString(c, DIGITS)) return true;
	return false;
}

inline void replaceWithSpanClass(string &text, string word, string cls) {
	replaceAll(text, word, classSpan(word, cls));
}

enum ReplaceDiceState {
	LookingForNumber,
	ParsingNDice,
	ParsingD,
	ParsingDiceNumber
};

inline void replaceDiceColors(string &text) {
	string ret = "";
	ReplaceDiceState state = LookingForNumber;
	int diceStart = 0;
	int diceEnd = 0;
	for (int i = 0; i < text.size(); i++) {
		char letter = text[i];
		switch (state) {
			case LookingForNumber: {
				if (isDigit(letter)) {
					state = ParsingNDice;
					diceStart = i;
					continue;
				}
				break;
			}
			case ParsingNDice: {
				if (isDigit(letter)) {
					state = LookingForNumber;
					continue;
				}
				else if (letter == 'd') {
					state = ParsingD;
					continue;
				}
			}
			case ParsingD: {
				if (isDigit(letter)) {
					state = ParsingDiceNumber;
					continue;
				}
				else {
					state = LookingForNumber;
				}
				break;
			}
			case ParsingDiceNumber: {
				if (isDigit(letter)) continue;
				else {
					//print ret << endl;
					ret += substring(text, diceEnd, diceStart);
					diceEnd = i;
					ret += colorSpan(substring(text, diceStart, diceEnd), DICE_COLOR);
					diceStart = i;
					state = LookingForNumber;
				}
			}
		}
	}
	if (diceStart < text.size()) {
		ret += substring(text, diceStart, text.size());
	}
	text = ret;
}

void colorCodeContent(string &text) {
	replaceWithSpanClass(text, "Loyalty", "Tag_Loyalty");
	replaceDiceColors(text);
}