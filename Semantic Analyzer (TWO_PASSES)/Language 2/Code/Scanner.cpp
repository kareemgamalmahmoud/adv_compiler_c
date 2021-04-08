#include "Scanner.h"

//check whether a token is a reserved word in the grammar (private)
Token Scanner::checkReserved(string _tname) {
	Token tk;
	if      (_tname == "program")	{ tk.tname = _tname;   tk.ttype = Program_SY; }
	else if (_tname == "begin")	    { tk.tname = _tname;   tk.ttype = Begin_SY; }
	else if (_tname == "is")        { tk.tname = _tname;   tk.ttype = Is_SY; }
	else if (_tname == "end")		{ tk.tname = _tname;   tk.ttype = End_SY; }
	else if (_tname == "int")	    { tk.tname = _tname;   tk.ttype = Int_SY; } 
	else if (_tname == "bool")	    { tk.tname = _tname;   tk.ttype = Bool_SY; }
	else if (_tname == "read")		{ tk.tname = _tname;   tk.ttype = Read_SY; }
	else if (_tname == "write")		{ tk.tname = _tname;   tk.ttype = Write_SY; }
	else if (_tname == "if")		{ tk.tname = _tname;   tk.ttype = If_SY; }
	else if (_tname == "then")		{ tk.tname = _tname;   tk.ttype = Then_SY; }
	else if (_tname == "const")     { tk.tname = _tname;   tk.ttype = Const_SY; }
	else if (_tname == "true")      { tk.tname = _tname;   tk.ttype = True_SY; }
	else if (_tname == "false")     { tk.tname = _tname;   tk.ttype = False_SY; }
	else if (_tname == "character") { tk.tname = _tname;   tk.ttype = Character_SY; }
	else if (_tname == "char")      { tk.tname = _tname;   tk.ttype = Char_SY; }
	else                            { tk.tname = _tname;   tk.ttype = ID_SY; }
	
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
		tk.ttype = Number_SY;
		return tk;
	}

	//******************** Allowed Symbols ********************
	//:= (there is no :)
	if(ch == ':') {
		f.get(ch);
		if(ch == '=')
			tk.ttype = Assign_SY;
		else {
			tk.ttype = ERROR_SY;
			f.putback(ch);
		}

		return tk;
	}

	//=
	if(ch == '=') { tk.ttype = Equal_SY; return tk; }
	//,
	if(ch == ',') { tk.ttype = Comma_SY; return tk; }
	//;
	if(ch == ';') { tk.ttype = SemiColon_SY; return tk; }

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