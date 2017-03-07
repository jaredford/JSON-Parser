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
// Prints the menu
void checkInput(int, char**);
// Reads the JSON file into a string
string readJSONFile(int, char**);
// variables to store value counts
int objects = 0, trues = 0, falses = 0, nulls = 0, arrays = 0, strings = 0, numbers = 0;
Object* jsonObject;
int main(int argc, char* argv[]) {
	try {
		checkInput(argc, argv);
		string str = readJSONFile(argc, argv);
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
		if(argc == 2)
			cout << endl << jsonObject->print(0) << endl;
		else {
			ofstream printFile;
			printFile.open(argv[2]);
			if (printFile.is_open()) {
				printFile << jsonObject->print(0);
				cout << "\nPretty printed file to: " << (string)argv[2] << endl;
			}
		}
	}
	catch (const char* error) {
		cout << endl << error;
	}
	catch (string error) {
		cout << endl << error;
	}
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
	while (*f != 'e') {
		f++;
	}
	f++;
	return b;
}

Value* parse_false(char*& f, char* l) {
	Bool* b = new Bool(false);
	falses++;
	while (*f != 'e') {
		f++;
	}
	f++;
	return b;
}

Value* parse_null(char*& f, char* l) {
	Null* n = new Null();
	f++;
	nulls++;
	while (*f != 'l') {
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
void checkInput(int argc, char** argv) {
	if (argc < 2 || argc > 3) {
		throw "You must have 2-3 input parameters\nExample: parser.exe [path to JSON file] [path to pretty print file]";
	}
}

string readJSONFile(int argc, char** argv) {
	ifstream inFile;
	char c;
	string str = "";
	inFile.open(argv[1]);
	if (inFile.is_open()) {
		while (inFile.get(c)) {
			str += c;
		}
	}
	else {
		throw "Could not open json file: " + (string)argv[1];
	}
	return str;
}