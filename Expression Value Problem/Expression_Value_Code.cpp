#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

const double MAX_VAL = pow(2.0,50); //2^50

enum TokenType { NUMBER_SY, PLUS_SY, MINUS_SY, MULT_SY, DIV_SY, POWER_SY, LPARN_SY, RPARN_SY, ERROR_SY, END_SOURCE_SY };

struct Token {
	string name;
	TokenType type;

	Token() {
		name = "Undefined";
		type = ERROR_SY;
	}
};

class Parser {
public:

	Parser (string fileName) {
		f.open(fileName);
		if(!f) {
			cout << "Unable to open file!" << endl;
			system("pause");
			exit(1);
		}
	}

	void Parse () {
		cur_Token = getToken();
		program();
		match(END_SOURCE_SY);
	}

private:

	ifstream f;

	Token cur_Token;

	Token getToken () {
		Token tk;
		char ch;
		string s = "";
		f.get(ch);
		while (isspace(ch)) {
			f.get(ch);
		}

		if(ch == '$') {
			tk.name = "End of file";
			tk.type = END_SOURCE_SY;
			return tk;
		}

		if(isdigit(ch)) {
			s += ch;
			f.get(ch);
			while (isdigit(ch)) {
				s += ch;
				f.get(ch);
			}
			f.putback(ch);
			tk.name = s;
			tk.type = NUMBER_SY;
			return tk;
		}

		if(ch == '+') { tk.name = "+"; tk.type = PLUS_SY;  return tk; } 
		if(ch == '-') { tk.name = "-"; tk.type = MINUS_SY; return tk; } 
		if(ch == '*') { tk.name = "*"; tk.type = MULT_SY;  return tk; }
		if(ch == '/') { tk.name = "/"; tk.type = DIV_SY;   return tk; } 
		if(ch == '^') { tk.name = "^"; tk.type = POWER_SY; return tk; } 
		if(ch == '(') { tk.name = "("; tk.type = LPARN_SY; return tk; } 
		if(ch == ')') { tk.name = ")"; tk.type = RPARN_SY; return tk; } 

		return tk;
	}

	void match (TokenType tk) {
		if (cur_Token.type == tk)
			cout << cur_Token.name << " is matched." << endl;
		else
			cout << cur_Token.name << " is unexpected!" << endl; 

		cur_Token = getToken();
	}


	//<program> ::= <expr>
	void program () {
		double val = expr();

		if (val >= MAX_VAL)
			cout << "Semantic ERROR: The value of the expression is " << val << " which exceeds " << MAX_VAL << endl;
		else
			cout << "Expression value = " << val <<endl; 
	}

	//<expr> ::= <term> { <weak_op> <term>}
	double expr () {
		double val, val1, val2;
		val1 = term();
		val = val1;
		while (cur_Token.type == PLUS_SY || cur_Token.type == MINUS_SY) {
			TokenType op = cur_Token.type;
			weak_op();
			val2 = term();
			if (op == PLUS_SY) {
				val += val2;
			}
			else if (op == MINUS_SY) {
				val -= val2;
			}
			else {
				cout << "Syntax ERROR: a weak operation is expected!" <<endl;
			}
		}

		return val;
	}

	//<term> :: <element> { <strong_op> <element> }
	double term () {
		double val, val1, val2;
		val1 = element();
		val = val1;
		while (cur_Token.type == MULT_SY || cur_Token.type == DIV_SY) {
			TokenType op = cur_Token.type;
			strong_op();
			val2 = element();
			if (op == MULT_SY) {
				val *= val2;
			}
			else if (op == DIV_SY) {
				if(val2 != 0) {
					val /= val2;
				}
				else
					cout << "DIVISION BY ZERO!!!" << endl;
			}
			else {
				cout << "Syntax ERROR: a strong operation is expected!" <<endl;
			}
		}

		return val;
	}

	//<element> ::= <factor> [^ <element>]
	double element () {
		double val, val1, val2;
		val1 = factor();
		val = val1;
		if(cur_Token.type == POWER_SY) {
			match(POWER_SY);
			val2 = element();
			val = pow(val,val2);
		}

		return val;
	}



	//-----------------------------------------------
	double string2Long (string s) {
		double num = 0;
		int dec = 1;
		for (int i = (int)s.length() - 1; i >= 0; i--) {
			num += (s[i] - '0') * dec;
			dec *= 10;
		}

		return num;
	}
	//-----------------------------------------------

	//<factor> ::= <number> | (<expr>)
	double factor () {
		double val;
		if(cur_Token.type == NUMBER_SY) {
			val = string2Long(cur_Token.name);
			match(NUMBER_SY);
			return val;
		}
		else if (cur_Token.type == LPARN_SY) {
			match(LPARN_SY);
			val = expr();
			match(RPARN_SY);
			return val;
		}
		else {
			cout << "Syntax ERROR: " << cur_Token.name << " is not expected!" << endl;
		}
	}
	

	//<weak_op> ::= + | -
	void weak_op () {
		if (cur_Token.type == PLUS_SY) { match(PLUS_SY); }
		else if (cur_Token.type == MINUS_SY) { match(MINUS_SY); }
		else { cout << "Syntax ERROR: " << cur_Token.name << " is not expected!" << endl; }
	}

	//<strong_op> ::= * | /
	void strong_op () {
		if (cur_Token.type == MULT_SY) { match(MULT_SY); }
		else if (cur_Token.type == DIV_SY) { match(DIV_SY); }
		else { cout << "Syntax ERROR: " << cur_Token.name << " is not expected!" << endl; }
	}
};

void main () {

	Parser p("test.txt");
	p.Parse();

	system("pause");
}