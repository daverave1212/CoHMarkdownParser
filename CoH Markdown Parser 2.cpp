// CoH Markdown Parser 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include "utils.h"
#include "DOM.h"
#include "Token.h"
#include "py.h"
#include "ColorCoder.h"
#include <Windows.h>
#include <map>


using namespace std;

// Fluff and useless shit :)
string progressBar = "[";

void _updateProgressBar() {
	progressBar += "H";
	print progressBar;
	print "]\n";
}

string projectPath = "C:\\Users\\Dave\\source\\repos\\CoH Markdown Parser 2\\CoH Markdown Parser 2";
// End Fluff and useless shit :)


enum HashTag {
	Null,
	None,
	Undefined,
	Class,
	Options,
	Skills,
	Language,
	Stats,
	MainStat,
	Abilities,
	ClassAbilities,
	Other,
	Spec,
	PageEnd,
	Specialization,
	SpellList,
	WeaponsLanguage,
	Weapons,
	Race,
	LevelBonus,
	Ignore,
	ItemTable,
	Background,
	BackgroundDescription,
	StartingItems,
	PageTitle,
	Gear
};

#define HandleStringCase(TheTag) case TheTag: return ""#TheTag ;

string toString(HashTag t) {
	switch (t) {
		HandleStringCase(Class);
		HandleStringCase(Options);
		HandleStringCase(Skills);
		HandleStringCase(Language);
		HandleStringCase(Stats);
		HandleStringCase(MainStat);
		HandleStringCase(Abilities);
		HandleStringCase(ClassAbilities);
		HandleStringCase(Other);
		HandleStringCase(Spec);
		HandleStringCase(PageEnd);
		HandleStringCase(Specialization);
		HandleStringCase(SpellList);
		HandleStringCase(Weapons);
		HandleStringCase(WeaponsLanguage);
		HandleStringCase(Race);
		HandleStringCase(LevelBonus);
		HandleStringCase(Ignore);
		HandleStringCase(Undefined);
		HandleStringCase(ItemTable);
		HandleStringCase(Background);
		HandleStringCase(BackgroundDescription);
		HandleStringCase(StartingItems);
		HandleStringCase(PageTitle);
		HandleStringCase(Gear);
		default:			 return "NONE";
	}
}

string getTagString(string line) {
	if (line.size() is 0) return "";
	if (line[0] isnot '#') return "";
	else {
		int end = 1;
		for (int i = 1; i < line.size(); i++) {
			if (line[i] is '#') {
				end = i;
				break;
			}
		}
		string tag = substring(line, 1, end);
		return tag;
	}
}

#define HandleGetTagCase(TheCase) if(tag == ""#TheCase) return TheCase;

HashTag getTag(string line) {
	if (line.size() is 0) return None;
	if (line[0] isnot '#') return None;
	else {
		int end = 1;
		for (int i = 1; i < line.size(); i++) {
			if (line[i] is '#') {
				end = i;
				break;
			}
		}
		string tag = substring(line, 1, end);
		HandleGetTagCase(None);
		HandleGetTagCase(Class);
		HandleGetTagCase(Race);
		HandleGetTagCase(Options);
		HandleGetTagCase(Skills);
		HandleGetTagCase(Language);
		HandleGetTagCase(Stats);
		HandleGetTagCase(MainStat);
		HandleGetTagCase(Abilities);
		HandleGetTagCase(ClassAbilities);
		HandleGetTagCase(Other);
		HandleGetTagCase(Spec);
		HandleGetTagCase(PageEnd);
		HandleGetTagCase(SpellList);
		HandleGetTagCase(Specialization);
		HandleGetTagCase(Weapons);
		HandleGetTagCase(WeaponsLanguage);
		HandleGetTagCase(LevelBonus);
		HandleGetTagCase(Ignore);
		HandleGetTagCase(Undefined);
		HandleGetTagCase(ItemTable);
		HandleGetTagCase(Background);
		HandleGetTagCase(BackgroundDescription);
		HandleGetTagCase(StartingItems);
		HandleGetTagCase(PageTitle);
		HandleGetTagCase(Gear);
		return Undefined;
	}
}

#define Defines {
#define FirstLine trimSpacesAndComments(lines[start])
#define parser(Name) string parse##Name##ToHTML (vector<string> lines, int start, int end) { DOM* wrapper = new DOM("div", "."#Name"");
#define autoparameterized (vector<string> lines, int start, int end)
#define does(x) ;x; return wrapper->toHTML();}
#define CurrentLine trimSpacesAndComments(lines[i])
#define Call(Name) parse##Name##ToHTML
#define For(i, start, end) for(int i = start; i<end; i++)
#define append(x) wrapper->appendChild(x)
#define findNextChunkStart(x) while(i <= end and isEmptySpaceOrComment(CurrentLine)){i++;} x = i;
#define findCurrentChunkEnd(x) while(i <= end and not isEmptySpaceOrComment(CurrentLine)){i++;} x = i;
#define ForEachInArray(elem, ar, code) for(int i = 0; i<ar.size(); i++){elem = ar[i];code}
#define func() if(true)
#define HandleCase(Name, ExtraClass)									\
	case Name: {											\
		wrapper = new Div(""#Name);							\
		parse##Name##ToHTML(lines, paragraphs, wrapper, paragraphs[0].start, paragraphs[0].end);	\
		return wrapper->toHTML();							\
	}
#define HandleCase2(DivClass, FuncName) 									\
		wrapper = new Div(DivClass);							\
		FuncName(lines, paragraphs, wrapper, paragraphs[0].start, paragraphs[0].end);	\
		return wrapper->toHTML();							
#define Parser(Name) void parse##Name##ToHTML(vector<string> lines, vector<Paragraph> pars, DOM* wrapper, int start, int end)
#define Defines_End }


enum EnclosureType {
	Ability,
	Talent,
	SubAbility,
	Creature,
	NoEnclosure
};

EnclosureType getEnclosureType(string line) {
	if (line.size() < 2) return NoEnclosure;
	if (line[0] == '{') return Talent;
	if (line[0] == '[') return Ability;
	if (line[0] == '<') return SubAbility;
	if (line[0] == '|') return Creature;
	return NoEnclosure;
}

int findEnclosureEnd(vector<string> lines, int start) {
	int i = start + 1;
	string line = trimSpacesAndComments(lines[i]);
	while (!isEmptySpace(line)) {
		EnclosureType type = getEnclosureType(line);
		if (type is Talent or type is Ability or type is Creature or getTag(line) isnot None) {
			return i - 1;
		}
		i++;
		line = trimSpacesAndComments(lines[i]);
	}
	return start;
}



Parser(Class) {
	for (auto par : pars) {
		DOM* name = new H2("Name golden-text", trimSpacesAndComments(lines[par.start]));
		wrapper->appendChild(name);
		For(i, par.start + 1, par.end + 1) {
			P* p = new P("DescriptionPar", CurrentLine);
			wrapper->appendChild(p);
		}
	}
}
Parser(Race) {
	for (auto par : pars) {
		DOM* name = new H2("Name golden-text", trimSpacesAndComments(lines[par.start]));
		wrapper->appendChild(name);
		For(i, par.start + 1, par.end + 1) {
			P* p = new P("DescriptionPar", CurrentLine);
			wrapper->appendChild(p);
		}
	}
}
Parser(Options) {
	H3* header = new H3("OptionsHeader golden-text", FirstLine);
	append(header);
	For(i, start + 1, end + 1) {
		P* p = new P("ChoicePar", CurrentLine);
		append(p);
	}
}
Parser(Skills) {
	H3* h = new H3("SkilsHeader golden-text", "Skills");
	append(h);
	For(i, start, end + 1) {
		P* p = new P("SkillsPar", CurrentLine);
		append(p);
	}
}
Parser(Language) {
	H3* h = new H3("LanguageHeader golden-text", "Languages");
	append(h);
	For(i, start, end + 1) {
		P* p = new P("LanguagePar", CurrentLine);
		append(p);
	}
}
Parser(Weapons) {
	H3* h = new H3("WeaponsLanguageHeader golden-text", "Weapon Skills");
	append(h);
	For(i, start, end + 1) {
		P* p = new P("WeaponsLanguagePar", CurrentLine);
		append(p);
	}
}
Parser(WeaponsLanguage) {
	H3* h = new H3("WeaponsLanguageHeader golden-text", "Weapon Skills and Language");
	append(h);
	For(i, start, end + 1) {
		P* p = new P("WeaponsLanguagePar", CurrentLine);
		append(p);
	}
}
Parser(Stats) {
	H3* h = new H3("StatsHeader golden-text", "Starting Stats:");
	append(h);
	For(i, start, end + 1) {
		P* p = new P("StatsPar", CurrentLine);
		append(p);
	}
}
Parser(MainStat) {
	H3* h = new H3("MainStatHeader golden-text", "Spellcasting and Main Stat");
	append(h);
	for (auto p : pars) {
		DOM* w = new Div("MainStatPart bordered");
		vector<DOM*> eachLine = DOM::makeEachLine(lines, p.start, p.end, "p", "MainStatParagraph");
		w->appendChildren(eachLine);
		append(w);
	}
}
Parser(Other) {
	H3* h = new H3("OtherHeader golden-text", "Other Notices");
	append(h);
	for (auto par : pars) {
		if (Spell::isLineSpell(lines[par.start])) {
			Spell s = Spell::createFromParagraph(lines, par.start, par.end);
			append(s.getDOM());
		}
		else {
			DOM* div = new Div("OtherPart");
			div->appendChildren(DOM::makeEachLine(lines, par.start, par.end, "p", ""));
			append(div);
		}
	}
}
Parser(Abilities) {
	H3* h3 = new H3("AbilitiesTitle golden-text", "Abilities");
	append(h3);
	for (auto par : pars) {
		if (Spell::isLineSpell(lines[par.start])) {
			Spell s = Spell::createFromParagraph(lines, par.start, par.end);
			append(s.getDOM());
		}
		else {
			DOM* div = new Div("AbilitiesPart");
			vector<DOM*> v = DOM::makeEachLine(lines, par.start, par.end, "p", "AbilitiesParagraph");
			div->appendChildren(v);
			append(div);
		}
	}
}
Parser(ClassAbilities) {
	H3* h3 = new H3("AbilitiesTitle golden-text", "Abilities");
	append(h3);
	for (auto par : pars) {
		if (Spell::isLineSpell(lines[par.start])) {
			Spell s = Spell::createFromParagraph(lines, par.start, par.end);
			append(s.getDOM());
		}
		else {
			DOM* div = new Div("AbilitiesPart");
			vector<DOM*> v = DOM::makeEachLine(lines, par.start, par.end, "p", "AbilitiesParagraph");
			div->appendChildren(v);
			append(div);
		}
	}
}
Parser(Spec) {
	H2* specTitle = new H2("SpecTitle golden-text", trimSpacesAndComments(lines[pars[0].start]));
	pars[0].start++;
	append(specTitle);
	for (auto par : pars) {
		if (Spell::isLineSpell(par.firstLine())) {
			Spell s = Spell::createFromParagraph(lines, par.start, par.end);
			append(s.getDOM());
		}
		else {
			Div* specPart = new Div("SpecPart");
			vector<DOM*> v = DOM::makeEachLine(lines, par.start, par.end, "p", "");
			specPart->appendChildren(v);
			append(specPart);
		}
	}
}
Parser(LevelBonus) {
	print "Found LEVELBONUS tag..\n";
	H2* specTitle = new H2("LevelBonus golden-text", "Level Up Bonuses");
	append(specTitle);
	for (auto par : pars) {
		print par.toString() << endl << endl;
		if (Spell::isLineSpell(par.firstLine())) {
			print "Yes, line is spell\n";
			Spell s = Spell::createFromParagraph(lines, par.start, par.end);
			append(s.getDOM());
		}
		else {
			Div* specPart = new Div("LevelBonusPart");
			vector<DOM*> v = DOM::makeEachLine(lines, par.start, par.end, "p", "");
			specPart->appendChildren(v);
			append(specPart);
		}
	}
}
Parser(Specialization) {
	H3* title = new H3("SpecializationTitle golden-text", "Choosing Your Specialization...");
	append(title);
	for (auto par : pars) {
		DOM* theList = new DOM("ul", "SpecializationsList");
		for (int i = par.start; i <= par.end; i++) {
			string line = trimSpacesAndComments(lines[i]);
			if (line[0] == '-') {
				DOM* li = new DOM("li", "SpecializationsListItem");
				li->innerHTML = substringFrom(line, 2);	// - Wizard, so from 'W'
				theList->appendChild(li);
			}
			else {
				P* p = new P("SpecializationPar", line);
				append(p);
			}
		}
		append(theList);
	}
}
Parser(SpellList) {
	H3* title = new H3("Spells golden-text", "Casting Spells and Maneuvers");
	append(title);
	Paragraph par1 = pars[0];
	vector<DOM*> v = DOM::makeEachLine(lines, par1.start, par1.end, "p", "");
	wrapper->appendChildren(v);
	if (pars.size() < 2) return;
	Paragraph par2 = pars[1];
	DOM* aList = new DOM("ul", "ListOfSpells");
	bool isListEmpty = true;
	for (int i = par2.start; i <= par2.end; i++) {
		string line = trimSpacesAndComments(lines[i]);
		if (Spell::isLineSpell(line)) {
			isListEmpty = false;
			DOM* li = new DOM("li", "ListOfSpellsElement");
			int bracketClose = findInString(line, "]", 0);
			if (bracketClose == -1) {
				bracketClose = findInString(line, "}", 0);
			}
			li->innerHTML = substring(line, 1, bracketClose);
			aList->appendChild(li);
		} else {
			if (!isListEmpty) {
				append(aList);
				aList = new DOM("ul", "ListOfSpells");
				isListEmpty = true;
			}
			DOM* listHeader = new DOM("li", "ListOfSpellsHeader");
			listHeader->innerHTML = line;
			aList->appendChild(listHeader);
		}
	}
	if (!isListEmpty) {
		append(aList);
	}
}
Parser(ItemTable) {
	H3* title = new H3("TableName golden-text", lines[pars[0].start]);
	Table* table = new Table("ItemTable");
	table->setHeaders("Item", "Prices");
	Td* lastAddedItemName = NULL;
	for (int i = pars[1].start; i <= pars[1].end; i++) {
		string line = trimSpacesAndComments(lines[i]);
		if (line[0] == '-') {
			if (lastAddedItemName == NULL) continue;
			line = trimSpaces(substring(line, 1, line.size()));
			P* p = new P("ItemDescription", line);
			lastAddedItemName->appendChild(p);
		}
		else {
			int semicolon = findInString(line, ':', 0);
			string itemName = trimSpaces(substring(line, 0, semicolon));
			string itemPrice = trimSpaces(substring(line, semicolon + 1, line.size()));
			Tr* tr = new Tr();
			Td* name = new Td("", itemName);
			Td* price = new Td("", itemPrice);
			Img* goldIcon = new Img("GoldIcon", "Gold.png");
			price->appendChild(goldIcon);
			tr->appendChild(name);
			tr->appendChild(price);
			table->appendChild(tr);
			lastAddedItemName = name;
		}
	}
	append(title);
	append(table);
}
Parser(Background) {
	Img* pageBreak = new Img("BackgroundBreak", "PageBreak.png");
	H3* title = new H3("Name golden-text", trimSpacesAndComments(lines[pars[0].start]));
	H4* subtitle = new H4("Subtitle golden-text", trimSpacesAndComments(lines[pars[0].start + 1]));
	append(pageBreak);
	append(title);
	append(subtitle);
}
Parser(BackgroundDescription) {
	Div* div = new Div("BackgroundDescription");
	vector<DOM*> v = DOM::makeEachLine(lines, pars[0].start, pars[0].end, "p", "BackgroundDescriptionParagraph");
	div->appendChildren(v);
	append(div);
}
Parser(StartingItems) {
	H3* h3 = new H3("StartingItemsHeader golden-text", "Starting Items");
	append(h3);
	Table* table = new Table("ItemTable");
	for (int i = pars[0].start; i <= pars[0].end; i++) {
		table->addRow(lines[i]);
	}
	append(table);
}
Parser(PageTitle) {
	H1* h1 = new H1("Title golden-text", trimSpacesAndComments(lines[pars[0].start]));
	append(h1);
	if (pars[0].size() >= 2) {
		H3* h3 = new H3("Title golden-text", trimSpacesAndComments(lines[pars[0].start + 1]));
		append(h3);
	}
	if (pars.size() >= 2) {
		for (int i = 1; i < pars.size(); i++) {
			Paragraph par = pars[i];
			vector<DOM*> v = DOM::makeEachLine(lines, par.start, par.end, "p", "DescriptionParagraph");
			wrapper->appendChildren(v);
		}
	}
}
Parser(Gear) {
	Paragraph par = pars[0];
	string line1 = trimSpacesAndComments(lines[par.start]);
	int semicolonPosition = findInString(line1, ':');
	string name = trimSpacesAndComments(substringTo(line1, semicolonPosition));
	string stat = trimSpacesAndComments(substringFrom(line1, semicolonPosition + 1));
	string weaponType = trimSpacesAndComments(lines[par.start + 1]);
	string damage = trimSpacesAndComments(lines[par.start + 2]);
	Div* iconWrapper = new Div("SpellIconWrapper");
	Img* icon = new Img("SpellIcon", "None.png");
	iconWrapper->appendChild(icon);
	append(iconWrapper);
	Div* text = new Div("SpellText");
	H3* h3 = new H3("SpellTitle GearTitle", name + " (" + classSpan(stat, "GearTitleWhiteText") + ")");
	text->appendChild(h3);
	P* weaponTypeP = new P("GearGrayText", weaponType);
	text->appendChild(weaponTypeP);
	P* damageP = new P("GearWhiteText", damage);
	text->appendChild(damageP);
	if (par.size() >= 4) {
		if (lines[par.start + 3] isnot "-") {
			P* effectP = new P("GearGreenText", trimSpacesAndComments(lines[par.start + 3]));
			text->appendChild(effectP);
		}
	}
	if (par.size() >= 6) {
		if (lines[par.start + 5] isnot "-") {
			P* negativeP = new P("GearRedText", trimSpacesAndComments(lines[par.start + 5]));
			text->appendChild(negativeP);
		}
	}
	if (par.size() >= 5) {
		if (lines[par.start + 4] isnot "-") {
			P* alternativeP = new P("GearOtherText", trimSpacesAndComments(lines[par.start + 4]));
			text->appendChild(alternativeP);
		}
	}
	append(text);
}

// Returns HTML
// First line always starts with a tag
string parseTag(HashTag tag, vector<string> lines, int from, int to) {
	int contentFrom = trimEmptyLinesFromFront(lines, from + 1, to);
	int contentTo = trimEmptyLinesFromBack(lines, from + 1, to);
	vector<Paragraph> paragraphs;
	if (tag == SpellList) {
		paragraphs = findParagraphs(lines, contentFrom, contentTo, false);
	}
	else {
		paragraphs = findParagraphs(lines, contentFrom, contentTo, true);
	}
	DOM* wrapper;
	switch (tag) {
		case Class: {HandleCase2("Class bordered", parseClassToHTML)}
		case Race: {HandleCase2("Race bordered", parseRaceToHTML)}
		case Options: {HandleCase2("Options bordered split-box", parseOptionsToHTML)}
		case Skills: {HandleCase2("Skills split-box", parseSkillsToHTML)}
		case Language: {HandleCase2("Language bordered split-box", parseLanguageToHTML)}
		case Stats: {HandleCase2("Stats bordered split-box", parseStatsToHTML)}
		case MainStat: {HandleCase2("MainStat bordered split-box", parseMainStatToHTML)}
		case Abilities: {HandleCase2("Abilities", parseAbilitiesToHTML)}
		case ClassAbilities: {HandleCase2("ClassAbilities split-box", parseClassAbilitiesToHTML)}
		case Other: {HandleCase2("Other", parseOtherToHTML)}
		case Specialization: {HandleCase2("Specialization bordered", parseSpecializationToHTML)}
		case SpellList: {HandleCase2("SpellList bordered", parseSpellListToHTML)}
		case Spec: {HandleCase2("Spec bordered", parseSpecToHTML)}
		case LevelBonus: {HandleCase2("LevelBonus bordered", parseLevelBonusToHTML)}
		case Weapons: {HandleCase2("Weapons bordered split-box", parseWeaponsToHTML)}
		case WeaponsLanguage: {HandleCase2("WeaponsLanguage bordered split-box", parseWeaponsLanguageToHTML)}
		case ItemTable: {HandleCase2("ItemTableWrapper", parseItemTableToHTML)}
		case Background: {HandleCase2("Background", parseBackgroundToHTML)}
		case BackgroundDescription: {HandleCase2("BackgroundDescription", parseBackgroundDescriptionToHTML)}
		case StartingItems: {HandleCase2("StartingItems bordered split-box", parseStartingItemsToHTML)}
		case PageTitle: {HandleCase2("PageTitle", parsePageTitleToHTML)}
		case Gear: {HandleCase2("Gear", parseGearToHTML)}
	}
	return "No case for tag " + toString(tag) + "\n";
}



string parseMarkdown(string fileName) {
	string parsedFile = "";
	vector<string> lines = readFileLineByLine(fileName, false);
	//print "Read " << fileName <<  " successfully.\n";
	print "Parsing file" << fileName << "\n";
	_updateProgressBar();
	string line;
	HashTag oldTag = Null;
	HashTag firstTag = Null;
	int start = 0;
	int end = 0;
	for (int i = 0; i < lines.size(); i++) {
		line = trimSpacesAndComments(lines[i]);
		HashTag newTag = getTag(line);
		if (newTag isnot None) {
			if (oldTag is Options or oldTag is Stats or oldTag is MainStat or oldTag is StartingItems) {
				parsedFile += "<div class=" + quote("split") + ">\n";
			}
			if (oldTag is Null) {
				oldTag = newTag;
				firstTag = newTag;
				start = i;
			}
			else if (oldTag is Ignore) {
				oldTag = newTag;
				start = i;
			}
			else {
				parsedFile += parseTag(oldTag, lines, start, i - 1);
				start = i;
				if (oldTag is Skills or oldTag is Language or oldTag is Weapons or oldTag is WeaponsLanguage or oldTag is ClassAbilities) {
					parsedFile += "</div>\n";
				}
				oldTag = newTag;
				if (newTag is PageEnd) return parsedFile;
			}
		}
	}
	return parsedFile;
}

void parseClass(string folder, string fileName) {
	string header = readFile("header.html");
	string footer = readFile("footer.html");
	string body = parseMarkdown("Markdown/" + folder + "/" + fileName + ".txt");
	colorCodeContent(body);
	string content = header + "\n" + body + "\n";
	content += "<style>body{background-image: url(\"Backgrounds/" + fileName + ".jpg\");}</style>\n";
	content += footer;
	writeFile(content, "HTML/" + fileName + ".html");
}
void parseRace(string folder, string fileName) {
	string header = readFile("header.html");
	string footer = readFile("footer.html");
	string body = parseMarkdown("Markdown/" + folder + "/" + fileName + ".txt");
	//colorCodeContent(body);
	string content = header + "\n" + body + "\n";
	content += "<style>body{background-image: url(\"Backgrounds/" + fileName + ".jpg\");}</style>\n";
	content += footer;
	writeFile(content, "HTML/" + fileName + ".html");
}

void parseItems(string folder, string fileName) {
	string header = readFile("header.html") + "\n<div class=\"ItemPageWrapper\">\n";
	string footer = "\n</div>\n" + readFile("footer.html");
	string body = parseMarkdown("Markdown/" + folder + "/" + fileName + ".txt");
	string content = header + "\n" + body + "\n";
	content += "<style>body{background-image: url(\"Backgrounds/Items.jpg\");}</style>\n";
	content += footer;
	writeFile(content, "HTML/" + fileName + ".html");
}

void parseBackgrounds(string folder, string fileName) {
	string header = readFile("header.html");
	string footer = readFile("footer.html");
	string body = parseMarkdown("Markdown/" + folder + "/" + fileName + ".txt");
	string content = header + "\n" + body + "\n";
	content += "<style>body{background-image: url(\"Backgrounds/Backgrounds.jpg\");}</style>\n";
	content += footer;
	writeFile(content, "HTML/" + fileName + ".html");
}

void parseGear(string folder, string fileName) {
	string header = readFile("header.html");
	string footer = readFile("footer.html");
	string body = parseMarkdown("Markdown/" + folder + "/" + fileName + ".txt");
	string content = header + "\n" + body + "\n";
	content += "<style>body{background-image: url(\"Backgrounds/Weapons.jpg\");}</style>\n";
	content += footer;
	writeFile(content, "HTML/" + fileName + ".html");
}

void parseSpellsToJSON(string fileName, map<string, Spell>* json) {
	vector<string> lines = readFileLineByLine(fileName, false);
	vector<Paragraph> paragraphs = findParagraphs(lines, 0, lines.size() - 1, true);
	for (auto par : paragraphs) {
		if (par.size() < 2)
			continue;
		string firstLine = trimSpacesAndComments(lines[par.start]);
		if (firstLine.size() == 0)
			continue;
		if (Spell::isLineSpell(firstLine)) {
			Spell theSpell = Spell::createFromParagraph(lines, par.start, par.end);
			json->insert(make_pair(theSpell.name, theSpell));
		}
	}
}
void parseSpellDatabase(string folder, map<string, Spell>* json) {
	vector<string> files = getAllFilesInFolder(folder);
	for (int i = 0; i < files.size(); i++) {
		parseSpellsToJSON(folder + "\\" + files[i], json);
	}
}

int main(){

	map<string, Spell>* json = new map<string, Spell>();
	parseSpellDatabase(projectPath + "\\Markdown\\Classes", json);
	parseSpellDatabase(projectPath + "\\Markdown\\Races", json);
	writeFile("SpellDatabase = " + Spell::mapToJSON(json), "HTML/SpellDatabase/Spells.js");
	//writeFile("SpellDatabase", "HTML/SpellDatabase/Spells.json");


	//string k = "Deal 3d6 damage to 1d28 enemies";
	//replaceDiceColors(k);
	//print k;
	//return 0;

	parseClass("Classes", "Cleric");
	parseClass("Classes", "Druid");
	parseClass("Classes", "Hunter");
	parseClass("Classes", "Mage");
	parseClass("Classes", "Paladin");
	parseClass("Classes", "Rogue");
	parseClass("Classes", "Shaman");
	parseClass("Classes", "Warlock");
	parseClass("Classes", "Warrior");
	
	parseRace("Races", "Bertle");
	parseRace("Races", "Dragonborn");
	parseRace("Races", "Dwarf");
	parseRace("Races", "Gnome");
	parseRace("Races", "Elf");
	parseRace("Races", "Human");
	parseRace("Races", "Hollow");
	parseRace("Races", "Orc");
	
	parseItems("Items", "Prices");
	parseBackgrounds("Backgrounds", "Backgrounds");
	parseGear("Items", "Weapons");
	parseGear("Items", "Armors");

	//parseSpellDatabase("Classes");

	//vector<string> files = getAllFilesInFolder(projectPath + "\\Markdown\\Classes");
	//print toString(files);

	

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

