#include <iostream>
#include <fstream>
#include <sstream>
#include "value.h"
using namespace std;
// Various parsing algorithms
Value* parse(char*&, char*);
Value* parse_object(char*&, char*);
Value* parse_true(char*&, char*);
Value* parse_false(char*&, char*);
Value* parse_null(char*&, char*);
Value* parse_array(char*&, char*);
Value* parse_string(char*&, char*);
Value* parse_number(char*&, char*);
// Gets path to JSON / Pretty Print files
string getPath(bool);
// Reads the JSON file into a string
string readJSONFile(string);
// Pretty Prints the file
void prettyPrint(string);
// variables to store value counts
int objects = 0, trues = 0, falses = 0, nulls = 0, arrays = 0, strings = 0, numbers = 0;
Object* jsonObject;
int main(int argc, char* argv[]) {
	string str;
	try {
		str = readJSONFile(getPath(false));
		char* f = (char*)str.c_str();
		char* l = (char*)strrchr(str.c_str(), '\0');
		jsonObject = (Object*)parse(f, l);
		cout << "Number of values: " << objects + trues + falses + nulls + arrays + strings + numbers << endl;
		cout << "Objects: " << objects << endl;
		cout << "Arrays: " << arrays << endl;
		cout << "Trues: " << trues << endl;
		cout << "Falses: " << falses << endl;
		cout << "Nulls: " << nulls << endl;
		cout << "Strings: " << strings << endl;
		cout << "Numbers: " << numbers << endl;
		prettyPrint(getPath(true));
	}
	catch (const char* error) {
		cout << error << endl;
		cin >> str;
	}
	catch (string error) {
		cout << error << endl;
		cin >> str;
	}
	cin >> str;
	return 0;
}
Value* parse(char*& f, char* l) {
	Value* v = new Value();
	while (f < l && *f != '\0') {
		switch (tolower(*f)) {
		case 't':
			v = parse_true(f, l);
			return v;
		case 'f':
			v = parse_false(f, l);
			return v;
		case 'n':
			v = parse_null(f, l);
			return v;
		case '[':
			v = parse_array(f, l);
			return v;
		case '{':
			v = parse_object(f, l);
			return v;
		case '"':
			v = parse_string(f, l);
			return v;
		default:
			try {
				if (isdigit(*f) || *f == '-') {
					v = parse_number(f, l);
					return v;
				}
			}
			catch (exception e) {
				cout << e.what();
			}
			f++;
			break;
		}
	}
}
Value* parse_true(char*& f, char* l) {
	Bool* b = new Bool(true);
	trues++;
	while (*f != 'e' && *f != 'E') {
		f++;
	}
	f++;
	return b;
}

Value* parse_false(char*& f, char* l) {
	Bool* b = new Bool(false);
	falses++;
	while (*f != 'e' && *f != 'E') {
		f++;
	}
	f++;
	return b;
}

Value* parse_null(char*& f, char* l) {
	Null* n = new Null();
	f++;
	nulls++;
	while (*f != 'l' && *f != 'L') {
		f++;
	}
	f += 2;
	return n;
}

Value* parse_string(char*& f, char* l) {
	f++;
	strings++;
	string temp = "";
	bool specialCharacter = false;
	while (*f != '"' || specialCharacter) {
		specialCharacter = *f == '\\' ? true : false;
		temp += *f;
		f++;
	}
	String* s = new String(temp);
	f++;
	return s;
}

Value* parse_number(char*& f, char* l) {
	numbers++;
	string temp = "";
	while (!isspace(*f) && *f != ',' && *f != '}' && *f != ']') {
		temp += *f;
		f++;
	}
	Number* n = new Number(temp);
	return n;
}

Value* parse_array(char*& f, char* l) {
	Array* a = new Array();
	bool hasNextElement = true;
	arrays++;
	while (hasNextElement) {
		f++;
		while (isspace(*f)) {
			f++;
		}
		if (*f == ']') {
			break;
		}
		a->values.push_back(parse(f, l));
		while (f != l && *f != ']' && *f != ',') {
			f++;
		}
		hasNextElement = *f == ',' ? true : false;
	}
	f++;
	return a;
}

Value* parse_object(char*& f, char* l) {
	Object* o = new Object();
	bool hasNextElement = true;
	objects++;
	string key;
	while (hasNextElement) {
		key = "";
		while (*f != '"' && *f != '}') {
			f++;
		}
		if (*f == '}')
			break;
		if (*f == '"')
			f++;
		while (*f != '"' || key.length() == 0) {
			key += *f;
			f++;
		}
		f++;
		o->members.push_back(pair<string, Value*>(key, parse(f, l)));
		while (f != l && *f != '}' && *f != ',') {
			f++;
		}
		hasNextElement = *f == ',' ? true : false;
	}
	f++;
	return o;
}
string getPath(bool forPrettyPrint) {
	string filePath;
	if (!forPrettyPrint) {
		cout << "Enter the path to the JSON file you wish to analyze:\n";
		cin >> filePath;
		return filePath;
	}
	cout << "Pretty print to console? [y/n]\n";
	cin >> filePath;
	while (filePath != "y" && filePath != "n") {
		cout << "\nYou must enter 'y' or 'n'!\n";
		cin >> filePath;
	}
	if (filePath == "y") {
		return "print_to_console";
	}
	cout << "Enter the path for the pretty printed file:\n";
	cin >> filePath;
	return filePath;
}

string readJSONFile(string filePath) {
	ifstream inFile;
	char c;
	string str = "";
	inFile.open(filePath.c_str());
	if (inFile.is_open()) {
		while (inFile.get(c)) {
			str += c;
		}
	}
	else {
		throw "Could not open json file: " + filePath;
	}
	return str;
}

void prettyPrint(string ppPath) {
	if (ppPath == "print_to_console")
		cout << endl << jsonObject->print(0) << endl;
	else {
		ofstream printFile;
		printFile.open(ppPath.c_str());
		if (printFile.is_open()) {
			printFile << jsonObject->print(0);
			cout << "\nPretty printed file to: " << ppPath << endl;
		}
	}
}