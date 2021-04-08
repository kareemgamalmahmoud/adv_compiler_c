#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
using namespace std;


enum TokenType {/*resrved words*/   BEGIN_SY, END_SY, PROC_SY, INTEGER_SY, BOOL_SY, READ_SY, WRITE_SY, IF_SY, THEN_SY, ELSE_SY, WHILE_SY,
	DO_SY, CALL_SY, OR_SY, AND_SY, NOT_SY,
	/*identefier*/      ID_SY,
	/*allowed symbols*/ COLON_SY, SEMICOL_SY, COMMA_SY, ASSIGN_SY, LPARN_SY, RPARN_SY, LessEQ_SY, LESS_SY, EQ_SY, LARGE_SY, LargeEQ_SY,
	NotEQ_SY, PLUS_SY, MINUS_SY, MULT_SY, DIV_SY, NUMBER_SY,
	/*end of the file*/ END_SOURCE_SY,
	/*a word out of the WREN grammar*/ ERROR_SY
};

struct Token {

	string tname;
	TokenType ttype;

	//constructor
	Token() {
		tname = "";
	}


	//get the type (as string) from the token type (as enum)
	string getTokenType() {
		switch (ttype) {
		case BEGIN_SY:      return "begin"; break;
		case END_SY:        return "end"; break;
		case PROC_SY:       return "proc"; break;
		case INTEGER_SY:    return "integer"; break;
		case BOOL_SY:       return "boolean"; break;
		case READ_SY:       return "read"; break;
		case WRITE_SY:      return "write"; break;
		case IF_SY:         return "if"; break;
		case THEN_SY:       return "then"; break;
		case ELSE_SY:       return "else"; break;
		case WHILE_SY:      return "while"; break;
		case DO_SY:         return "do"; break;
		case CALL_SY:       return "call"; break;
		case OR_SY:         return "or"; break;
		case AND_SY:        return "and"; break;
		case NOT_SY:        return "not"; break;
		case COLON_SY:      return ":"; break;
		case SEMICOL_SY:    return ";"; break;
		case COMMA_SY:      return ","; break;
		case ASSIGN_SY:     return ":="; break;
		case LPARN_SY:      return "("; break;
		case RPARN_SY:      return ")"; break;
		case LessEQ_SY:     return "<="; break;
		case LESS_SY:       return "<"; break;
		case EQ_SY:         return "="; break;
		case LARGE_SY:      return ">"; break;
		case LargeEQ_SY:    return ">="; break;
		case NotEQ_SY:      return "<>"; break;
		case PLUS_SY:       return "+"; break;
		case MINUS_SY:      return "-"; break;
		case MULT_SY:       return "*"; break;
		case DIV_SY:        return "/"; break;
		case ID_SY:         return tname + " (Identifier)"; break;
		case END_SOURCE_SY: return "End of file"; break;
		default:            return "Undefined Token!"; break;
		}
	}

};

class Scanner {

private:

	ifstream f;

	//check whether a token is a reserved word in the grammar
	Token checkReserved(string _tname) {
		Token tk;
		if (_tname == "begin") { tk.tname = "begin";   tk.ttype = BEGIN_SY; }
		else if (_tname == "end") { tk.tname = "end";	    tk.ttype = END_SY; }
		else if (_tname == "proc") { tk.tname = "proc";	tk.ttype = PROC_SY; }
		else if (_tname == "integer") { tk.tname = "integer"; tk.ttype = INTEGER_SY; }
		else if (_tname == "boolean") { tk.tname = "boolean"; tk.ttype = BOOL_SY; }
		else if (_tname == "read") { tk.tname = "read";    tk.ttype = READ_SY; }
		else if (_tname == "write") { tk.tname = "write";   tk.ttype = WRITE_SY; }
		else if (_tname == "if") { tk.tname = "if";	    tk.ttype = IF_SY; }
		else if (_tname == "then") { tk.tname = "then";    tk.ttype = THEN_SY; }
		else if (_tname == "else") { tk.tname = "else";    tk.ttype = ELSE_SY; }
		else if (_tname == "while") { tk.tname = "while";   tk.ttype = WHILE_SY; }
		else if (_tname == "do") { tk.tname = "do";	    tk.ttype = DO_SY; }
		else if (_tname == "call") { tk.tname = "call";    tk.ttype = CALL_SY; }
		else if (_tname == "or") { tk.tname = "or";      tk.ttype = OR_SY; }
		else if (_tname == "and") { tk.tname = "and";     tk.ttype = AND_SY; }
		else if (_tname == "not") { tk.tname = "not";     tk.ttype = NOT_SY; }
		else { tk.tname = _tname;    tk.ttype = ID_SY; }

		return tk;
	}
public:
	//Constructor
	Scanner(string fileName) {
		f.open(fileName);
		if (!f) {
			cout << "ERROR: Unable to open the file!" << endl;
			system("pause");
			exit(1);
		}
	}

	//get a token from program file 
	Token getToken() {
		char ch;
		string s = "";
		Token tk;

		f.get(ch);
		//skip all spaces
		while (isspace(ch) && !f.eof()) {
			f.get(ch);
		}

		if (f.eof()) {
			tk.ttype = END_SOURCE_SY;
			return tk;
		}

		//identifier or reserved word
		if (isalpha(ch)) {
			s += ch;
			f.get(ch);
			while (isalnum(ch) && !f.eof()) {
				s += ch;
				f.get(ch);
			}
			f.putback(ch);

			tk = checkReserved(s);
			return tk;
		}

		//Number
		if (isdigit(ch)) {
			s += ch;
			f.get(ch);
			while (isdigit(ch) && !f.eof()) {
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
		if (ch == '<') {
			f.get(ch);
			if (ch == '=')
				tk.ttype = LessEQ_SY;
			else if (ch == '>')
				tk.ttype = NotEQ_SY;
			else {
				tk.ttype = LESS_SY;
				f.putback(ch);
			}

			return tk;
		}

		//> or >=
		if (ch == '>') {
			f.get(ch);
			if (ch == '=')
				tk.ttype = LargeEQ_SY;
			else {
				tk.ttype = LARGE_SY;
				f.putback(ch);
			}

			return tk;
		}

		//:= (there is no :)
		if (ch == ':') {
			f.get(ch);
			if (ch == '=')
				tk.ttype = ASSIGN_SY;
			else {
				tk.ttype = ERROR_SY;
				f.putback(ch);
			}

			return tk;
		}

		//=
		if (ch == '=') { tk.ttype = EQ_SY; return tk; }
		//+
		if (ch == '+') { tk.ttype = PLUS_SY; return tk; }
		//*
		if (ch == '*') { tk.ttype = MULT_SY; return tk; }
		///
		if (ch == '/') { tk.ttype = DIV_SY; return tk; }
		//,
		if (ch == ',') { tk.ttype = COMMA_SY; return tk; }
		//;
		if (ch == ';') { tk.ttype = SEMICOL_SY; return tk; }
		//(
		if (ch == '(') { tk.ttype = LPARN_SY; return tk; }
		//)
		if (ch == ')') { tk.ttype = RPARN_SY; return tk; }

		return tk;
	}



	//get all the tokens from program file token by token
	void displayTokens() {
		Token tk = getToken();
		while (tk.ttype != END_SOURCE_SY) {
			cout << tk.getTokenType() << endl;
			tk = getToken();
		}
	}

};

//-------------------------------
//ãä Çæá åäÇ ÇáÌÏíÏ Îáí ÈÇáß
//-------------------------------
//Ýåã ÇáÓäÈæá ÊíÈá 

/*

	enum type
	enum kind

	struct symbol

		string name;
		type t
		kind k

		vector<symbol> params;

		symbol()
		{
			name = "";
			t = un;
			k = empty

			sparams = vector<symbol>(0);
		}

		symbol(string _name , symbolType _type , SymbolKind _kind , vector<Symbol> _parameters = vector<Symbol>(0))
			sname = _name;
			stype = _type;
			skind = _kind;
			sparameters = _parameters;


	struct node

		symbol sym;
		node* next;

		node(symbol s){
			 sym = s;
			 next = NULL
		}

	const int max_size = 10;

	class symbol table
		private:
			hnode ** htable;

			int hashfun

		publice:
			symboltable();

			 void insertsymbol();
			 void insert list();

			 bool searchby name();

			 void union()

			 void intersect()


*/

enum SymbolType { procedure_t, integer, boolean, undefined }; //undefined for the identifiers that does NOT exist
enum SymbolKind { procedure_k, variable, empty1 }; //empty for the identifiers that does NOT exist

struct Symbol {
	string sname;
	SymbolType stype;
	SymbolKind skind;

	vector<Symbol> sparameters; //when its stype is 'procedure'


	//Default Constructor
	Symbol() {
		sname = "";
		stype = undefined;
		skind = empty1;
		sparameters = vector<Symbol>(0);
	}

	//Constructor
	Symbol(string _name, SymbolType _type, SymbolKind _kind, vector<Symbol> _parameters = vector<Symbol>(0)) {
		sname = _name;
		stype = _type;
		skind = _kind;
		sparameters = _parameters;
		//Distinguish between 0-size sparameters which means the Symbol has no parametrs data member
		//and the one which means the Symbol has 0 parameters BY check if its stype is 'procedure' or something else
	}

	//get the type (as string) from the symbol type (as enum)

	//-->not on maybe

	string getSymbolType() {
		switch (stype) {
		case procedure_t: return "procedure"; break;
		case integer: return "integer"; break;
		case boolean: return "boolean"; break;
		default: return "undefined"; break;
		}
	}

	//get the kind (as string) from the symbol kind (as enum)

	//-->not on maybe

	string getSymbolKind() {
		switch (skind) {
		case procedure_k: return "procedure"; break;
		case variable: return "variable"; break;
		default: return "empty"; break;
		}
	}

	////display a symbol in one of the following forms
	//<name|type|kind>
	//<name|procedure()|kind>
	//<name|procedure(p1,p2,...)|kind>

	//-->not on maybe

	void dispSymbol() {
		//display a symbol in one of the following forms
		//<name|type|kind>
		//<name|procedure()|kind>
		//<name|procedure(p1,p2,...)|kind>
		cout << "<" << sname << "|" << getSymbolType() << "|";
		if (stype == procedure_t) {
			int num_params = (int)sparameters.size();
			if (num_params == 0) //procedure with at 0 parameters
				cout << "()|";
			else { //procedure with at least one parameter
				Symbol param; //used to store each parameter
				for (int i = 0; i < num_params - 1; i++) {
					param.stype = sparameters[i].stype;
					cout << param.getSymbolType() << ",";
				}
				param.stype = sparameters[num_params - 1].stype;
				cout << param.getSymbolType() << ")|";
			}
		}

		cout << getSymbolKind() << ">" << endl;
	}
};



struct hNode {
	Symbol sym;
	hNode* next;

	//Constructor
	hNode(Symbol _sym) {
		sym = _sym;
		next = NULL;
	}
};

const int MAX_SIZE = 11;


class SymbolTable {

private:
	hNode** hTable; //array of pointers to linked lists

	//Hash function
	int hFunction(string _sname) {
		int indx = 0;
		for (int i = 0; i < (int)_sname.length(); i++)
			indx += int(_sname[i]);

		indx %= MAX_SIZE;

		return indx;
	}

public:

	//Constructor
	SymbolTable() {
		hTable = new hNode*[MAX_SIZE];

		for (int i = 0; i < MAX_SIZE; i++) {
			hTable[i] = NULL;
		}
	}

	//insert a symbol in a symbol table
	void insertSymbol(Symbol _sym) {

		if (searchSymbol_byName(_sym.sname))
			cout << "Semantic ERROR(1): The name " << _sym.sname << " is declared more than once!" << endl;

		hNode* newNode = new hNode(_sym);
		int indx = hFunction(_sym.sname);

		if (hTable[indx] == NULL)
			hTable[indx] = newNode;
		else {
			hNode* entry = hTable[indx];
			while (entry->next != NULL)
				entry = entry->next;

			entry->next = newNode;
		}
	}

	//insert a list of symbol with the same type in a symbol table
	//(e.g. integer x, y, z will be inserted as (x,integer,variable), (y,integer,variable), (z,integer,variable))
	void insertSymbolList(SymbolType _stype, vector<string> _snames) {
		int size = _snames.size();
		for (int i = 0; i < size; i++) {
			if (!searchSymbol_byName(_snames[i])) {
				Symbol sym; //(_snames[i],_stype,variable);
				sym.sname = _snames[i];
				sym.stype = _stype;
				sym.skind = variable;
				insertSymbol(sym);
			}
			else
				cout << "Semantic ERROR(1): The name " << _snames[i] << " is declared more than once!" << endl;
		}
	}


	//search for a symbol in a symbol table
	bool searchSymbol_byName(string _sname) {
		int indx = hFunction(_sname);
		hNode* entry = hTable[indx];
		while (entry != NULL) {
			if (entry->sym.sname == _sname)
				return true;
			entry = entry->next;
		}

		return false;
	}

	//retrieve a symbol from a symbol table given its name
	Symbol getSymbol_byName(string _sname) {
		int indx = hFunction(_sname);
		hNode* entry = hTable[indx];
		while (entry != NULL) {
			if (entry->sym.sname == _sname)
				return entry->sym;
			entry = entry->next;
		}

		Symbol sym0; //empty symbol
		return sym0;
	}

	//union two symbol tables stable1 and stable2 in the first one table1
	void unionSymbolTables(SymbolTable stable2) {
		hNode* entry2;
		for (int i = 0; i < MAX_SIZE; i++) {
			entry2 = stable2.hTable[i];
			while (entry2 != NULL) {
				insertSymbol(entry2->sym);
				entry2 = entry2->next;
			}
		}
	}

	//is there an intersection between two symbol tables
	bool intersectSymbolTables(SymbolTable stable2) {
		hNode* entry2;
		for (int i = 0; i < MAX_SIZE; i++) {
			entry2 = stable2.hTable[i];
			while (entry2 != NULL) {
				if (searchSymbol_byName(entry2->sym.sname))
					return true;
				entry2 = entry2->next;
			}
		}

		return false;
	}
};



//---------------------CODEGENERATION-----------

enum OpCode {
	ADD, SUB, MULT, DIV, LT, LE, NE, EQ, GE, GT, AND,
	OR, XOR, NEG, NOT, ASSIGN, BIF, BNIF, IFN, IFLT, IFLE, IFNE, IFEQ,
	IFGE, IFGT, GOTO, READ, WRITE, HALT, STO, LOAD
};

long long lable = 0;
long long temp = 0;
int count;


string newlabel()
{
	return "L" + to_string(++lable);
}
string newtemp()
{
	return "T" + to_string(++temp);
}

class Code {
public:
	string op, operand1; //operand2 , dest;
	Code* next;
	Code(string n, string m1) // Constructor
	{
		op = n;
		operand1 = m1;
		//operand2 = m2;
		//dest = d;
		next = NULL;
	}
};

int length(Code* c)
{
	Code* temp = c; int count = 0;
	while (temp != NULL)
	{
		count++;
		temp = temp->next;
	}
	return count;
}

Code* add(Code * c1, Code* c2)
{
	Code* temp = c1;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = c2;
	return c1;
}

Code * optimize(Code* c1, string op1)
{
	Code * c2 = c1;
	if (length(c2) == 1)
	{
		c2->op = op1;
		return c2;
	}
	else
	{
		string t1 = newtemp();
		string t2 = newtemp();
		c2 = new Code("STO", t1);
		c1 = add(c2, c1);
		c2 = new Code("STO", t2);
		c1 = add(c1, c2);
		c2 = new Code("LOAD", t1);
		c1 = add(c1, c2);
		c2 = new Code(op1, t2);
		c1 = add(c1, c2);
		return c1;
	};
}



/*
	class parser
		public:
					//constructor
			parser {
			sc = new Scanner(fileName);
			}

			void parser_ven {
				cur_token = sc->getToken(); //get the first token in the program file
				program();
				match(END_SOURCE_SY);
			}

		private:
			scanner* sc;
			token cur_token;

			void syntax_error(token tk){

			cout << "Syntax ERROR: " << tk.getTokenType() << " is not expected!" << endl;

			}

			void match(tokentype _ttype) {

			if (cur_token.ttype == _ttype)
				cout << cur_token.getTokenType() << " is matched." << endl;
			else
				syntax_error(cur_token);

			cur_token = sc->getToken(); //get the next token in the program file

			}

			void program()

			...
			...
*/


//----------------------PARSER & SEMANTIC & CODE GEN------------------------

class Parser {
public:
	//Constructor
	Parser(string fileName) {
		sc = new Scanner(fileName);
	}

	//<parse_VEN> ::= <program> EOF
	void parse_VEN(Code* code) {
		cur_token = sc->getToken(); //get the first token in the program file
		program(code);
		match(END_SOURCE_SY);
	}

private:
	Scanner* sc;
	Token cur_token;

	SymbolTable ST;

	Code *code; // for code gen

	//for scope
	//stack<SymbolTable*> S;

	void syntax_error(Token tk) {
		cout << "Syntax ERROR: " << tk.getTokenType() << " is not expected!" << endl;
	}

	void match(TokenType _ttype) {
		if (cur_token.ttype == _ttype)
			cout << cur_token.getTokenType() << " is matched." << endl;
		else
			syntax_error(cur_token);

		cur_token = sc->getToken(); //get the next token in the program file
	}

	//<program> ::= <block>
	void program(Code* code) {
		block(ST, code);
	}

	//<block> ::= begin <declaration-seq> <command-seq> end
	void block(SymbolTable & _ST, Code* c) {
		match(BEGIN_SY);
		dec_seq(_ST, c);
		cmd_seq(_ST, c);
		match(END_SY);
	}

	//<declaration-seq> ::= <declaration> {<declaration>}
	void dec_seq(SymbolTable & _ST, Code* c) {
		Code* c1;
		dec(_ST, c);
		while (cur_token.ttype == INTEGER_SY || cur_token.ttype == BOOL_SY || cur_token.ttype == PROC_SY)
		{
			dec(_ST, c1);
			c = add(c, c1);
		}
	}

	//<declaration> ::= <type> <name-list> |
	//proc <name> [(<parameter-list>)] = <command>
	void dec(SymbolTable & _ST, Code* c) {
		if (cur_token.ttype == INTEGER_SY || cur_token.ttype == BOOL_SY) {
			//insert list of symbols with different names but same type in the symbol table
			SymbolType _stype = type(); //their type is the same
			vector<string> _snames = name_list(); //but different names

			_ST.insertSymbolList(_stype, _snames); //in this function every symbol has the kind variable 
												  //AND it checks if any of the symbol names is already in the symbol table

		}
		else if (cur_token.ttype == PROC_SY) {
			//insert a symbol with kind and type 'procedure' and name of the ID  in the symbol table
			Symbol sym;
			sym.stype = procedure_t; //its type is 'procedure'
			sym.skind = procedure_k; //its kind is 'procedure'
			match(PROC_SY);
			sym.sname = cur_token.tname; //its name is the id name
			match(ID_SY);
			if (cur_token.ttype == LPARN_SY) {
				match(LPARN_SY);
				sym.sparameters = para_list(); //its parameters
				match(RPARN_SY);
			}

			_ST.insertSymbol(sym); //in this function it checks if the symbol name is already in the symbol table

			match(EQ_SY);

			cmd(_ST, c, sym.sparameters); //because every symbol appears inside a command MUST be declared
									  //in the parameters of the procedure (if there is one) OR in the symbol table
		}
		else {
			syntax_error(cur_token);
		}
	}

	//<type> ::= integer | Boolean
	SymbolType type() {
		if (cur_token.ttype == INTEGER_SY) {
			match(INTEGER_SY);
			return integer;
		}

		if (cur_token.ttype == BOOL_SY) {
			match(BOOL_SY);
			return boolean;
		}

		syntax_error(cur_token);
	}

	//------------------------------------------
	//search for a symbol in a parameter list of a procedure
	bool searchParameter_byName(string _pname, vector<Symbol> _stypes) {
		int size = _stypes.size();
		if (size == 0)
			return true;

		for (int i = 0; i < size; i++) {
			if (_pname == _stypes[i].sname)
				return true;
		}

		return false;
	}

	//get a symbol in a parameter list of a procedure
	Symbol getParameter_byName(string _pname, vector<Symbol> _stypes) {
		int size = _stypes.size();
		Symbol sym;
		if (size == 0)
			return sym; //empty Symbol

		for (int i = 0; i < size; i++) {
			if (_pname == _stypes[i].sname)
				return _stypes[i];
		}

		return sym; //empty Symbol
	}
	//------------------------------------------

	//<parameter-list> ::= <type> <name-list> { ; <type> <name-list>}
	vector<Symbol> para_list() {
		vector<Symbol> syms;
		SymbolType _stype;
		_stype = type();
		vector<string> _snames;
		_snames = name_list();
		for (int i = 0; i < (int)_snames.size(); i++) {
			Symbol s;// (_snames[i], _stype, variable);
			s.sname = _snames[i];
			s.stype = _stype;
			s.skind = variable;
			if (searchParameter_byName(s.sname, syms) && i != 0) {//i != 0 because when the parameter list is empty the search function returns true
				cout << "Semantic ERROR(1): The parameter " << _snames[i] << " appears more than once in the same parameter list!" << endl;
				continue;
			}
			syms.push_back(s);
		}

		while (cur_token.ttype == SEMICOL_SY) {
			match(SEMICOL_SY);
			_stype = type();
			_snames = name_list();
			for (int i = 0; i < (int)_snames.size(); i++) {
				Symbol s;// (_snames[i], _stype, variable);
				s.sname = _snames[i];
				s.stype = _stype;
				s.skind = variable;
				if (searchParameter_byName(s.sname, syms)) {
					cout << "Semantic ERROR(1): The parameter " << _snames[i] << " appears more than once in the same parameter list!" << endl;
					continue;
				}
				syms.push_back(s);
			}
		}

		return syms;
	}

	//<name-list> ::= <name> { , <name>}
	vector<string> name_list() {
		vector<string> snames;
		snames.push_back(cur_token.tname);
		match(ID_SY);

		while (cur_token.ttype == COMMA_SY) {
			match(COMMA_SY);
			snames.push_back(cur_token.tname);
			match(ID_SY);
		}

		return snames;
	}

	//<command-seq> ::= <command> { ; <command>}
	void cmd_seq(SymbolTable _ST, Code* c) {
		//Command here MUST have symbols only declared in the symbol table
		Code* c1;
		cmd(_ST, c);

		while (cur_token.ttype == SEMICOL_SY) {
			match(SEMICOL_SY);
			cmd(_ST, c1);
			c = add(c1, c);
		}
	}

	//<command> ::= <name> := <expr> |
	//read <name> |
	//write <expr> |
	//if <expr> then <command-seq> [else <command-seq>] end if |
	//while <expr> do <command-seq> end while |
	//call <name> [(<name-list>)] |
	//<block>
	void cmd(SymbolTable _ST, Code* c, vector<Symbol> _stypes = vector<Symbol>(0)) {

		Code *c1, *c2;
		//<command> ::= <name> := <expr>
		if (cur_token.ttype == ID_SY) {
			if (!_ST.searchSymbol_byName(cur_token.tname) && !searchParameter_byName(cur_token.tname, _stypes)) //here symbol MUST be declared in the symbol
																											  //table OR parameters list of a procedure
				cout << "Semantic ERROR(1 & 2): The name " << cur_token.tname << " is not declared!" << endl;

			Symbol s = _ST.getSymbol_byName(cur_token.tname);

			match(ID_SY);
			match(ASSIGN_SY);
			expr(_ST);
			c1 = new Code("STO", cur_token.tname);
			SymbolType LHS_type = s.stype;
			SymbolType RHS_type = expr(_ST, _stypes);

			if (LHS_type != RHS_type)
				cout << "Semantic ERROR(3): The l.h.s and r.h.s of an assignment expression MUST be of the same type: " << s.getSymbolType() << "!" << endl;

		}
		//<command> ::= read <name>
		else if (cur_token.ttype == READ_SY) {
			match(READ_SY);
			if (!_ST.searchSymbol_byName(cur_token.tname) && !searchParameter_byName(cur_token.tname, _stypes)) //here symbol MUST be declared in the symbol
																											  //table OR parameters list of a procedure
				cout << "Semantic ERROR(1 & 2): The name " << cur_token.tname << " is not declared!" << endl;

			Symbol s = _ST.getSymbol_byName(cur_token.tname);
			if (s.stype != integer || s.skind != variable)
				cout << "Semantic ERROR(6): The name after 'read' " << cur_token.tname << " MUST be an integer variable!" << endl;
			c = new Code("READ", cur_token.tname);
			match(ID_SY);
		}
		//<command> ::= write <expr>
		else if (cur_token.ttype == WRITE_SY) {
			match(WRITE_SY);
			expr(_ST);
			c = c1->operand1 + "STO" + to_string(temp) + "WRITE" + to_string(temp);
		}
		//<command> ::= if <expr> then <command-seq> [else <command-seq>] end if
		else if (cur_token.ttype == IF_SY) {
			match(IF_SY);
			expr(_ST);
			string l = newlabel();
			c1 = new Code("JF", l);
			c = add(c, c1);
			match(THEN_SY);
			cmd_seq(_ST, c2);
			c1 = new Code("LABEL", 1);
			c = add(c, c1);
			if (cur_token.ttype == ELSE_SY) {
				match(ELSE_SY);
				cmd_seq(_ST, c);
			}

			match(END_SY);
			match(IF_SY);
		}
		//<command> ::= while <expr> do <command-seq> end while 
		else if (cur_token.ttype == WHILE_SY) {
			match(WHILE_SY);
		}
		//<command> ::= call <name> [(<name-list>)] 
		else if (cur_token.ttype == CALL_SY) {
			match(CALL_SY);
			if (!_ST.searchSymbol_byName(cur_token.tname) && !searchParameter_byName(cur_token.tname, _stypes)) //here symbol MUST be declared in the symbol
																											  //table OR parameters list of a procedure
				cout << "Semantic ERROR(1): The name " << cur_token.tname << " is not declared!" << endl;

			Symbol s = _ST.getSymbol_byName(cur_token.tname);
			if (s.stype != procedure_t && s.skind != procedure_k)
				cout << "Semantic ERROR(7): The name after 'call' " << cur_token.tname << " MUST be a procedure!" << endl;

			match(ID_SY);

			if (cur_token.ttype == LPARN_SY) {
				match(LPARN_SY);
				vector<string> _parametersNames = name_list();
				match(RPARN_SY);

				if (s.sparameters.size() == _parametersNames.size()) {
					Symbol para;
					for (int i = 0; i < (int)_parametersNames.size(); i++) {
						para = ST.getSymbol_byName(_parametersNames[i]);
						if (para.stype == undefined) {
							cout << "Semantic ERROR(7): parameter No. " << i + 1 << " is not declared!" << endl;
						}

						if (para.stype != s.sparameters[i].stype) {
							cout << "Semantic ERROR(7): parameter No. " << i + 1 << " must be " << s.getSymbolType() << endl;
						}
					}
				}
				else
					cout << "Semantic ERROR(7): There must be " << s.sparameters.size() << " parameters!" << endl;
			}

		}

	}

	//<expr> ::= <expr1> {or <expr1>}
	SymbolType expr(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t, t1, t2;
		t1 = expr1(_ST, _stypes);
		t = t1;
		bool flag = false;
		while (cur_token.ttype == OR_SY) {
			match(OR_SY);
			t = boolean; //Now it must be boolean and t1 MUST be boolean or then t will be undefined
			t2 = expr1(_ST, _stypes);
			if (t1 != boolean || t2 != boolean) {
				flag = true;
				cout << "Semantic ERROR(3): Expression must be of type boolean!" << endl;
			}
		}

		if (flag)
			t = undefined;

		return t; //if one of the expressions between the or's is not boolean then the whole expression has undefined type
	}

	//<expr1> ::= <expr2> {and <expr2>}
	SymbolType expr1(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t, t1, t2;
		t1 = expr2(_ST, _stypes);
		t = t1;
		bool flag = false;
		while (cur_token.ttype == AND_SY) {
			match(AND_SY);
			t = boolean; //Now it must be boolean and t1 MUST be boolean or then t will be undefined
			t2 = expr2(_ST, _stypes);
			if (t1 != boolean || t2 != boolean) {
				flag = true;
				cout << "Semantic ERROR(3): Expression must be of type boolean!" << endl;
			}
		}

		if (flag)
			t = undefined;

		return t; //if one of the expressions between the and's is not boolean then the whole expression has undefined type
	}


	//<expr2> ::= <expr3> | not <expr>
	SymbolType expr2(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t;
		if (cur_token.ttype == ID_SY || cur_token.ttype == NUMBER_SY || cur_token.ttype == LPARN_SY || cur_token.ttype == MINUS_SY)
			t = expr3(_ST, _stypes);
		else if (cur_token.ttype == NOT_SY) {
			match(NOT_SY);
			t = boolean;
			t = expr(_ST, _stypes); //it must return boolean since 'not' appeared
			if (t != boolean)
				cout << "Semantic ERROR(3): Expression must be of type boolean!" << endl;

		}
		else
			syntax_error(cur_token);

		return t;
	}

	//<expr3> ::= <expr4> {<relation> <expr4>}
	SymbolType expr3(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t, t1, t2;
		t1 = expr4(_ST, _stypes);
		t = t1;
		bool flag = false;
		while (cur_token.ttype == LESS_SY || cur_token.ttype == LessEQ_SY || cur_token.ttype == NotEQ_SY
			|| cur_token.ttype == LARGE_SY || cur_token.ttype == LargeEQ_SY || cur_token.ttype == EQ_SY) {
			t = boolean; //because 'relation' appeared
			relation();
			t2 = expr4(_ST, _stypes);
			if (t1 != integer || t2 != integer) {
				flag = true;
				cout << "Semantic ERROR(3): Expression must be of type integer!" << endl;
			}
		}

		if (flag)
			t = undefined;

		return t;
	}

	//<expr4> ::= <term> {<weak op> term}
	SymbolType expr4(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t, t1, t2;
		t1 = term(_ST, _stypes);
		t = t1;
		bool flag = false;
		while (cur_token.ttype == PLUS_SY || cur_token.ttype == MINUS_SY) {
			t = integer; //Now it must be integer and t1 MUST be integer becasue 'weak op' appeared
			weak_op();
			t2 = term(_ST, _stypes);
			if (t1 != integer || t2 != integer) {
				flag = true;
				cout << "Semantic ERROR(3): Expression must be of type integer!" << endl;
			}
		}

		if (flag)
			t = undefined;

		return t;
	}

	//<term> ::= <element> {<strong op> <element>}
	SymbolType term(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t, t1, t2;
		t1 = element(_ST, _stypes);
		t = t1;
		bool flag = false;
		while (cur_token.ttype == MULT_SY || cur_token.ttype == DIV_SY) {
			t = integer; //Now it must be integer and t1 MUST be integer becasue 'strong op' appeared
			strong_op();
			t2 = element(_ST, _stypes);
			if (t1 != integer || t2 != integer) {
				flag = true;
				cout << "Semantic ERROR(3): Expression must be of type integer!" << endl;
			}
		}

		if (flag)
			t = undefined;

		return t;
	}
	//<element> ::= [-] <elem>
	SymbolType element(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t;
		if (cur_token.ttype == MINUS_SY)
			match(MINUS_SY);

		t = elem(_ST, _stypes);
		return t;
	}

	//<elem> ::= <numeral> | <name> | (<expr>)
	SymbolType elem(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t;
		if (cur_token.ttype == NUMBER_SY) {
			t = integer;
			match(NUMBER_SY);
		}
		else if (cur_token.ttype == ID_SY) {
			//Because of thi here we had to pass the symbolTable and list of parameters for all the previous functions
			if (_ST.searchSymbol_byName(cur_token.tname)) { //here symbol MUST be declared in the symbol table OR parameters list of a procedure
				Symbol s = ST.getSymbol_byName(cur_token.tname);
				t = s.stype;
			}
			else if (searchParameter_byName(cur_token.tname, _stypes)) {
				Symbol s = getParameter_byName(cur_token.tname, _stypes);
				t = s.stype;
			}
			else {
				t = undefined;
				cout << "Semantic ERROR(1 & 2): The name " << cur_token.tname << " is not declared!" << endl;
			}
			match(ID_SY);
		}
		else if (cur_token.ttype == LPARN_SY) {
			match(LPARN_SY);
			t = expr(_ST, _stypes);
			match(RPARN_SY);
		}
		else {
			t = undefined;
			syntax_error(cur_token);
		}

		return t;
	}


	//<relation> ::= < | <= | <> | = | > | >=
	void relation() {
		switch (cur_token.ttype) {
		case LESS_SY: match(LESS_SY); break;
		case LessEQ_SY: match(LessEQ_SY); break;
		case NotEQ_SY: match(NotEQ_SY); break;
		case EQ_SY: match(EQ_SY); break;
		case LARGE_SY: match(LARGE_SY); break;
		case LargeEQ_SY: match(LargeEQ_SY); break;
		default: syntax_error(cur_token); break;
		}
	}

	//<weak op> ::= + | –
	void weak_op() {
		switch (cur_token.ttype) {
		case PLUS_SY: match(PLUS_SY); break;
		case MINUS_SY: match(MINUS_SY); break;
		default: syntax_error(cur_token); break;
		}
	}
	//<strong op> ::= * | /
	void strong_op() {
		switch (cur_token.ttype) {
		case MULT_SY: match(MULT_SY); break;
		case DIV_SY: match(DIV_SY); break;
		default: syntax_error(cur_token); break;
		}
	}

};




void main() {
	//Scanner sc("test.txt");
	//sc.displayTokens();

	Parser p("test.txt");
	Code* code;
	p.parse_VEN(code);
	system("pause");
}