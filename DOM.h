#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "py.h"
#include "utils.h"
#include <map>

using namespace std;

bool isSingleTag(string tag) {
	if (tag == "img") return true;
	return false;
}

class DOM { public:
	
	string tagName = "";
	string id = "";
	string classes = "";
	string innerHTML = "";
	string src = "";
	int colspan = -1;
	vector<DOM*> children;

	static vector<DOM*> makeEachLine(vector<string> lines, int start, int end, string tag, string classOrID) {
		vector<DOM*> ret;
		for (int i = start; i <= end; i++) {
			DOM* d = new DOM(tag, classOrID);
			d->innerHTML = trimSpacesAndComments(lines[i]);
			ret.push_back(d);
		}
		return ret;
	}

	void addClass(string cls) {
		classes += " " + cls;
	}

	void setClassOrID(string classOrID) {
		if (classOrID != "") {
			if (classOrID[0] == '#') {
				id = substringFrom(classOrID, 1);
			}
			else if (classOrID[0] == '.') {
				classes = substringFrom(classOrID, 1);
			}
			else {
				classes = classOrID;
			}
		}
	}

	DOM(string t, string classOrID = "") {
		tagName = t;
		setClassOrID(classOrID);
	}

	void appendChild(DOM* child) {
		children.push_back(child);
	}

	void appendChildren(vector<DOM*> v) {
		for (int i = 0; i < v.size(); i++) {
			appendChild(v[i]);
		}
	}

	string toHTML(int nTabs = 0) {
		string ret = tabs(nTabs) + "<" + tagName;
		if (id != "") {
			ret += " id=" + quote(id);
		}
		if (classes != "") {
			ret += " class=" + quote(classes);
		}
		if (src != "") {
			ret += " src=" + quote(src);
		}
		if (colspan != -1) {
			ret += " colspan=" + quote(to_string(colspan));
		}
		if (isSingleTag(tagName)) {
			ret += ">\n";
			return ret;
		}
		if (children.size() == 0) {
			ret += ">" + innerHTML + "</" + tagName + ">\n";
		}
		else {
			ret += ">\n";
			if (innerHTML != "") {
				ret += " " + innerHTML + "\n";
			}
			for (int i = 0; i < children.size(); i++) {
				ret += children[i]->toHTML(nTabs + 1);
			}
			ret += tabs(nTabs) + "</" + tagName + ">\n";
		}
		return ret;
	}
};

class P : public DOM { public:
	P(string classOrID = "", string text = "") : DOM("p", classOrID) {
		innerHTML = text;
	}
};

class H1 : public DOM {
public:
	H1(string classOrID = "", string text = "") : DOM("h1", classOrID) {
		innerHTML = text;
	}
};

class H2 : public DOM {
public:
	H2(string classOrID = "", string text = "") : DOM("h2", classOrID) {
		innerHTML = text;
	}
};

class H3 : public DOM {
public:
	H3(string classOrID = "", string text = "") : DOM("h3", classOrID) {
		innerHTML = text;
	}
};

class H4 : public DOM {
public:
	H4(string classOrID = "", string text = "") : DOM("h4", classOrID) {
		innerHTML = text;
	}
};

class Img : public DOM {
public:
	Img(string classOrID = "", string s = "") : DOM("img", classOrID) {
		src = s;
	}
};

class Div : public DOM { public:
	Div(string classOrID = "", string text = "") : DOM("div", classOrID) {
		innerHTML = text;
	}
};





class Th : public DOM {
public:
	Th(string classOrID = "", string text = "") : DOM("th", classOrID) {
		innerHTML = text;
	}
};

class Td : public DOM {
public:
	Td(string classOrID = "", string text = "") : DOM("td", classOrID) {
		innerHTML = text;
	}
};

class Tr : public DOM {
public:
	Tr(string classOrID = "") : DOM("tr", classOrID) {}

	void addTh(string text, string classOrID = "") {
		Th* th = new Th(classOrID, text);
		this->appendChild(th);
	}

	void addTd(string text, string classOrID = "") {
		Td* td = new Td(classOrID, text);
		this->appendChild(td);
	}
};

class Table : public DOM {
public:
	Table(string classOrID = "") : DOM("table", classOrID) {}

	void setHeaders(string h1 = "", string h2 = "", string h3 = "", string h4 = "") {
		Tr* tr = new Tr();
		if(h1 != "") tr->addTh(h1);
		if(h2 != "") tr->addTh(h2);
		if(h3 != "") tr->addTh(h3);
		if(h4 != "") tr->addTh(h4);
		this->appendChild(tr);
	}

	void addRow(string d1 = "", string d2 = "", string d3 = "", string d4 = "") {
		Tr* tr = new Tr();
		if (d1 != "") tr->addTd(d1);
		if (d2 != "") tr->addTd(d2);
		if (d3 != "") tr->addTd(d3);
		if (d4 != "") tr->addTd(d4);
		this->appendChild(tr);
	}

};

string colorSpan(string s, string colorCode) {
	return "<span style=\"color: #" + colorCode + "\">" + s + "</span>";
}

string classSpan(string s, string cls) {
	return "<span class=" + quote(cls) + ">" + s + "</span>";
}

enum SpellType {
	SpellEnclosure,
	TalentEnclosure,
	CreatureEnclosure,
	SubSpellEnclosure
};

enum CostColor {
	ActionColor,
	PassiveColor
};

class Spell {
public:
	string name = "";
	string description = "";
	string cost = "";
	vector<Spell> children;
	SpellType type = SpellEnclosure;
	CostColor costColor = PassiveColor;

	void setNameAndCost(string line) {
		int i = trimSpaces(line, 0);
		if (line[i] is '[') {
			int braceClose = findInString(line, ']');
			name = substring(line, i + 1, braceClose);
			i = trimSpacesAnd(line, braceClose + 1, ":");
			cost = substringFrom(line, i);
		}
		else if (line[i] is '{') {
			int braceClose = findInString(line, '}');
			name = substring(line, i + 1, braceClose);
			i = trimSpacesAnd(line, braceClose + 1, ":");
			cost = substringFrom(line, i);
			type = TalentEnclosure;
		}
		else if (line[i] is '<') {
			int braceClose = findInString(line, '>');
			name = substring(line, i + 1, braceClose);
			i = trimSpacesAnd(line, braceClose + 1, ":");
			cost = substringFrom(line, i);
			type = SubSpellEnclosure;
		}
		else if (line[i] is '|') {
			int braceClose = findInString(line, '|', i+1);
			name = substring(line, i + 1, braceClose);
			i = trimSpacesAnd(line, braceClose + 1, ":");
			cost = substringFrom(line, i);
			type = SubSpellEnclosure;
		}
		if (findInString(cost, "Action") != -1) {
			costColor = ActionColor;
		}
	}

	void setDescriptionFromLines(vector<string> lines, int start, int end) {
		description = trimSpacesAndComments(lines[start]);
		for (int i = start + 1; i <= end; i++) {
			string line = trimSpacesAndComments(lines[i]);
			description += "<br>" + line;
		}
	}

	void appendChild(Spell s) {
		children.push_back(s);
	}

	string toString(int nTabs = 0) {
		string ret = tabs(nTabs) + "\"name\" : " + quote(name) + ",\n" + tabs(nTabs) + "\"cost\" : ";
		ret += quote(cost) + ",\n" + tabs(nTabs) + "\"desc\" : " + quote(description) + "\n";
		return ret;
	}

	string toJSON(int nTabs = 0) {
		string ret = tabs(nTabs) + "{\n";
		ret += toString(nTabs + 1) + tabs(nTabs) + "}";
		return ret;
	}

	DOM* getDOM() {
		string classes = ".Spell";
		if (type == TalentEnclosure) {
			classes += " Talent";
		}
		DOM* spell = new DOM("div", classes);
		DOM* spellContentWrapper = new DOM("div", ".SpellContentWrapper");
		DOM* spellIconWrapper = new DOM("div", ".SpellIconWrapper");
		DOM* spellIcon = new DOM("img", ".SpellIcon");
		spellIcon->src = "None.png";
		spellIconWrapper->appendChild(spellIcon);
		spellContentWrapper->appendChild(spellIconWrapper);
		DOM* spellText = new DOM("div", ".SpellText");
		DOM* spellTitle = new DOM("h4", ".SpellTitle");
		switch (type) {
			case SpellEnclosure: spellTitle->addClass("SpellTitleAbility"); break;
			case TalentEnclosure: spellTitle->addClass("SpellTitleTalent"); break;
			case CreatureEnclosure: spellTitle->addClass("SpellTitleCreature"); break;
			case SubSpellEnclosure: spellTitle->addClass("SpellTitleSubSpell"); break;
		}
		string spellActionsColor;
		switch (costColor) {
			case ActionColor: spellActionsColor = "SpellActionsNormal"; break;
			case PassiveColor: spellActionsColor = "SpellActionsPassive"; break;
		}
		spellTitle->innerHTML = classSpan(name, "SpellName") + " : " + classSpan(cost, spellActionsColor);
		spellText->appendChild(spellTitle);
		DOM* spellDescription = new DOM("p", ".SpellDescription");
		spellDescription->innerHTML = description;
		spellText->appendChild(spellDescription);
		spellContentWrapper->appendChild(spellText);
		spell->appendChild(spellContentWrapper);
		for (int i = 0; i < children.size(); i++) {
			spell->appendChild(children[i].getDOM());
		}
		return spell;
	}

	string toHTML() {
		return getDOM()->toHTML();
	}

	static Spell createFromParagraph(vector<string> lines, int start, int end) {
		vector<Paragraph> pars = findParagraphs(lines, start, end, true, true);
		Spell s;
		s.setNameAndCost(trimSpacesAndComments(lines[start]));
		s.setDescriptionFromLines(lines, start + 1, pars[0].end);
		for (int i = 1; i < pars.size(); i++) {
			int pstart = pars[i].start;
			int pend = pars[i].end;
			Spell ss;
			ss.setNameAndCost(trimSpacesAndComments(lines[pstart]));
			ss.setDescriptionFromLines(lines, pstart + 1, pend);
			s.appendChild(ss);
		}
		return s;
	}

	static bool isLineSpell(string line) {
		line = trimSpacesAndComments(line);
		if (line.size() < 2) return false;
		if (line[0] == '{' || line[0] == '[' || line[0] == '|') return true;
		return false;
	}

	static string mapToJSON(map<string, Spell>* m) {
		string ret = "{\n";
		for (map<string, Spell>::iterator it = m->begin(); it != m->end(); ++it){
			string key = quote(it->first);
			Spell value = it->second;
			ret += tabs(1) + key + " : " + value.toJSON(1) + ",\n";
		}
		ret += "\t_done_ : null\n}";
		return ret;
	}
};


