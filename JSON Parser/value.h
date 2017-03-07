#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

using namespace std;
struct Value {
	virtual ~Value() = default;
	virtual string print(int indent) { return ""; };
	string indent(int indention) {
		string temp = "";
		for (int i = 0; i < indention; i++)
		{
			temp += " ";
		}
		return temp;
	}
};

struct Null : Value {
	string print(int indent) {
		return "null";
	}
};

struct Bool : Value {
	Bool(bool b) : Val(b) {}
	bool Val;
	string print(int indent) {
		if (Val) {
			return "true";
		}
		return "false";
	}
};

struct String : Value, string {
	using string::string;
	using string::size;
	String(string s) : Val(s) {}
	string print(int indent) {
		return "\"" + Val + "\"";
	}
	string Val;
};

struct Number : Value, string {
	using string::string;
	using string::size;
	Number(string s) : Val(s) {}
	string print(int indent) {
		return Val;
	}
	string Val;
};

struct Array : Value, vector<Value*> {
	vector<Value*> values;
	~Array() {
		for (Value* v : *this) {
			delete v;
		}
	}
	string print(int indent) {
		string s = "[";
		for (Value* v : values) {
			string temp = v->print(indent + 1);
			if (values.back() != v) {
				temp += ", ";
			}
			s += temp;
		}
		if (values.size() == 0) {
			return "[ ]";
		}
		return s + "]";
	}
};

struct Object : Value, vector<Value*> {
	vector<pair<string, Value*>> members;
	~Object() {
		for (Value* v : *this) {
			delete v;
		}
	}
	string print(int indent) {
		string s = "{\n";
		for (pair<string, Value*> member : members) {
			string temp = this->indent(indent + 1) + "\"" + member.first + "\": " + member.second->print(indent + 2);
			if (members.back() != member) {
				temp += ",\n";
			}
			s += temp;
		}
		if (members.size() == 0) {
			return "{ }";
		}
		return s + "\n" +this->indent(indent) + "}";
	}
};