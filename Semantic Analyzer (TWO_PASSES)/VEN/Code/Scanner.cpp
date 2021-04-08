#include "Scanner.h"

//check whether a token is a reserved word in the grammar (private)
Token Scanner::checkReserved(string _tname) {
	Token tk;
	if      (_tname == "begin")	    { tk.tname = "begin";   tk.ttype = BEGIN_SY; }
	else if (_tname == "end")		{ tk.tname = "end";	    tk.ttype = END_SY; }
	else if (_tname == "proc")		{ tk.tname = "proc";	tk.ttype = PROC_SY; } 
	else if (_tname == "integer")	{ tk.tname = "integer"; tk.ttype = INTEGER_SY; } 
	else if (_tname == "boolean")	{ tk.tname = "boolean"; tk.ttype = BOOL_SY; }
	else if (_tname == "read")		{ tk.tname = "read";    tk.ttype = READ_SY; }
	else if (_tname == "write")		{ tk.tname = "write";   tk.ttype = WRITE_SY; }
	else if (_tname == "if")		{ tk.tname = "if";	    tk.ttype = IF_SY; }
	else if (_tname == "then")		{ tk.tname = "then";    tk.ttype = THEN_SY; }
	else if (_tname == "else")		{ tk.tname = "else";    tk.ttype = ELSE_SY; }
	else if (_tname == "while")		{ tk.tname = "while";   tk.ttype = WHILE_SY; }
	else if (_tname == "do")		{ tk.tname = "do";	    tk.ttype = DO_SY; }
	else if (_tname == "call")		{ tk.tname = "call";    tk.ttype = CALL_SY; }  
	else if (_tname == "or")		{ tk.tname = "or";      tk.ttype = OR_SY; }
	else if (_tname == "and")		{ tk.tname = "and";     tk.ttype = AND_SY; } 
	else if (_tname == "not")		{ tk.tname = "not";     tk.ttype = NOT_SY; }
	else                            { tk.tname = _tname;    tk.ttype = ID_SY; }
	
	return tk;
}

//Constructor
Scanner::Scanner(string fileName) {
	f.open(fileName);
	if(!f) {
		cout<<"ERROR: Unable to open the file!"<<endl;
		system("pause");
		exit(1);
	}
}

//get a token from program file 
Token Scanner::getToken() {
	char ch;
	string s = "";
	Token tk;

	f.get(ch);
	//skip all spaces
	while(isspace(ch) && !f.eof()) {
		f.get(ch);
	}

	if(f.eof()) {
		tk.ttype = END_SOURCE_SY;
		return tk;
	}

	//identifier or reserved word
	if(isalpha(ch)) {
		s += ch;
		f.get(ch);
		while(isalnum(ch) && !f.eof()) {
			s += ch;
			f.get(ch);
		}
		f.putback(ch);

		tk = checkReserved(s);
		return tk;
	}

	//Number
	if(isdigit(ch)) {
		s += ch;
		f.get(ch);
		while(isdigit(ch) && !f.eof()) {
			s += ch;
			f.get(ch);
		}
		f.putback(ch);

		tk.tname = s;
		tk.ttype = NUMBER_SY;
		return tk;
	}

	//******************** Allowed Symbols ********************
	//< or <= or <>
	if(ch == '<') {
		f.get(ch);
		if(ch == '=')
			tk.ttype = LessEQ_SY;
		else if(ch == '>')
			tk.ttype = NotEQ_SY;
		else {
			tk.ttype = LESS_SY;
			f.putback(ch);
		}

		return tk;
	}

	//> or >=
	if(ch == '>') {
		f.get(ch);
		if(ch == '=')
			tk.ttype = LargeEQ_SY;
		else {
			tk.ttype = LARGE_SY;
			f.putback(ch);
		}

		return tk;
	}
	
	//:= (there is no :)
	if(ch == ':') {
		f.get(ch);
		if(ch == '=')
			tk.ttype = ASSIGN_SY;
		else {
			tk.ttype = ERROR_SY;
			f.putback(ch);
		}

		return tk;
	}

	//=
	if(ch == '=') { tk.ttype = EQ_SY; return tk; }
	//+
	if(ch == '+') { tk.ttype = PLUS_SY; return tk; }
	//*
	if(ch == '*') { tk.ttype = MULT_SY; return tk; }
	///
	if(ch == '/') { tk.ttype = DIV_SY; return tk; }
	//,
	if(ch == ',') { tk.ttype = COMMA_SY; return tk; }
	//;
	if(ch == ';') { tk.ttype = SEMICOL_SY; return tk; }
	//(
	if(ch == '(') { tk.ttype = LPARN_SY; return tk; }
	//)
	if(ch == ')') { tk.ttype = RPARN_SY; return tk; }

	return tk;
}

//get all the tokens from program file token by token
void Scanner::displayTokens() {
	Token tk = getToken();
	while (tk.ttype != END_SOURCE_SY) {
		cout << tk.getTokenType() << endl;
		tk = getToken();
	}
}