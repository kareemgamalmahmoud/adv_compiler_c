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
//�� ��� ��� ������ ��� ����
//-------------------------------
//��� ������� ���� 

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

class Parser {
public:
	//Constructor
	Parser(string fileName) {
		sc = new Scanner(fileName);
	}

	//<parse_VEN> ::= <program> EOF
	void parse_VEN() {
		cur_token = sc->getToken(); //get the first token in the program file
		program();
		match(END_SOURCE_SY);
	}

private:
	Scanner* sc;
	Token cur_token;

	SymbolTable ST;

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
	void program() {
		block(ST);
	}

	//<block> ::= begin <declaration-seq> <command-seq> end
	void block(SymbolTable & _ST) {
		match(BEGIN_SY);
		dec_seq(_ST);
		cmd_seq(_ST);
		match(END_SY);
	}


	void dec_seq(SymbolTable & _st)
	{
		dec(_st);
		while(cur_token.ttype == INTEGER_SY || cur_token.ttype == BOOL_SY || cur_token.ttype == PROC_SY)
			dec(_ST);
	}

	void dec(SymbolTable & _ST) {

		if (cur_token.ttype == INTEGER_SY || cur_token.ttype == BOOL_SY) {

			SymbolType _stype =  type();
			name_list();

		}

	}


};