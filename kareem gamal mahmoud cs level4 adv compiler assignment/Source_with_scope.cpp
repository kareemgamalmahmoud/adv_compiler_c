/*
		kareem gamal mahmoud mohamed 
			cs level 4 
			adv compiler assignment 
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
using namespace std;


//----------------------SCANNER----------------------------

enum TokenType {/*resrved words*/   BEGIN_SY, END_SY, PROC_SY, INTEGER_SY, BOOL_SY, READ_SY, WRITE_SY, IF_SY, THEN_SY, ELSE_SY, WHILE_SY,
									DO_SY, CALL_SY, OR_SY, AND_SY, NOT_SY,
				/*identefier*/      ID_SY,
				/*allowed symbols*/ COLON_SY, SEMICOL_SY, COMMA_SY, ASSIGN_SY, LPARN_SY, RPARN_SY, LessEQ_SY, LESS_SY, EQ_SY, LARGE_SY, LargeEQ_SY,
									NotEQ_SY, PLUS_SY, MINUS_SY, MULT_SY, DIV_SY, NUMBER_SY,
				/*end of the file*/		END_SOURCE_SY,
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
		case NUMBER_SY:    return "number"; break;
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
		//to make sure we skip all spaces
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

		//--------------------- Allowed Symbols ---------------------
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


//---------------SYMBOLTABLE-----------------------


enum SymbolType { procedure_t, integer, boolean, undefined }; //undefined for the identifiers that does NOT exist
enum SymbolKind { procedure_k, variable, empty1 }; //empty for the identifiers that does NOT exist

struct Symbol {
	string sname;
	SymbolType stype;
	SymbolKind skind;
	
	vector<Symbol> sparameters; //for procedure type
		
	Symbol()//Constructor
	{
		sname = "";
		stype = undefined;
		skind = empty1;
		sparameters = vector<Symbol>(0);
	}

	Symbol(string _name, SymbolType _type, SymbolKind _kind, vector<Symbol> _parameters = vector<Symbol>(0))//Constructor
	{
		sname = _name;
		stype = _type;
		skind = _kind;
		sparameters = _parameters;
		//Distinguish between 0-size sparameters which means the Symbol has no parametrs data member
		//and the one which means the Symbol has 0 parameters BY check if its stype is 'procedure' or something else
	}

	string getSymbolType()//get the type (string) from symbol type (enum)
	{
		switch (stype) {
		case procedure_t: return "procedure"; break;
		case integer: return "integer"; break;
		case boolean: return "boolean"; break;
		default: return "undefined"; break;
		}
	}

	string getSymbolKind()//get the kind (string) from the symbol kind (enum)
	{
		switch (skind) {
		case procedure_k: return "procedure"; break;
		case variable: return "variable"; break;
		default: return "empty"; break;
		}
	}	

	void dispSymbol() {
		/*
		to display a symbol as one of these forms
			<name|type|kind>
			<name|procedure()|kind>
			<name|procedure(p1,p2,...)|kind>
		*/
		cout << "<" << sname << "|" << getSymbolType() << "|";
		if (stype == procedure_t) {
			int num_params = (int)sparameters.size();
			if (num_params == 0) // 0 parameters
				cout << "()|";
			else { //one parameter
				Symbol param; 
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
	hNode(Symbol _sym)//Constructor
	{
		sym = _sym;
		next = NULL;
	}
};

//hash table size
const int MAX_SIZE = 11;

class SymbolTable {

private:
	hNode** hTable; //array of pointers to linked lists

	int hFunction(string _sname)//Hash function
	{
		int indx = 0;
		for (int i = 0; i < (int)_sname.length(); i++)
			indx += int(_sname[i]);
		indx %= MAX_SIZE;
		return indx;
	}

public:
	SymbolTable()//Constructor 
	{
		hTable = new hNode*[MAX_SIZE];
		for (int i = 0; i < MAX_SIZE; i++) {
			hTable[i] = NULL;
		}
	}

	void insertSymbol(Symbol _sym)//insert a symbol in a symbol table
	{
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

	
	void insertSymbolList(SymbolType _stype, vector<string> _snames) //symbols with same types
	{
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

	bool searchSymbol_byName(string _sname)//search for a symbol in a symbol table
	{
		int indx = hFunction(_sname);
		hNode* entry = hTable[indx];
		while (entry != NULL) {
			if (entry->sym.sname == _sname)
				return true;
			entry = entry->next;
		}
		return false;
	}

	Symbol getSymbol_byName(string _sname) //return the symbol by its name
	{
		int indx = hFunction(_sname);
		hNode* entry = hTable[indx];
		while (entry != NULL) {
			if (entry->sym.sname == _sname)
				return entry->sym;
			entry = entry->next;
		}
		Symbol sym0; //empty symbol if it found nothing
		return sym0;
	}

	void unionSymbolTables(SymbolTable stable2)//union two symbol tables
	{
		hNode* entry2;
		for (int i = 0; i < MAX_SIZE; i++) {
			entry2 = stable2.hTable[i];
			while (entry2 != NULL) {
				insertSymbol(entry2->sym);
				entry2 = entry2->next;
			}
		}
	}

	
	bool intersectSymbolTables(SymbolTable stable2)//intersection between two symbol tables
	{
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


//--------------------CODE CLASS---------------------

enum OpCode {
	undefinedOP, ADD, SUB, MULT, DIV, LT, LE, NE, EQ, GE, GT, AND,
	OR, NEG, NOT, ASSIGN, BIF, BNIF, IFN, IFLT, IFLE, IFNE,
	IFEQ, IFGE, IFGT, GOTO, READ, WRITE, HALT, WHILE, CALL
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
	OpCode op;
	Code* next;
	string src1, src2, dest;

	Code(OpCode n = undefinedOP, string m1 = " ", string m2 = " ", string m3 = "", Code* cc = NULL) // Constructor
	{
		op = n;
		src1 = m1;
		src2 = m2;
		dest = m3;
		next = cc;
	}

	string getOpCode(OpCode op)//take operation as OpCode and return it as string 
	{
		switch (op)
		{
		case ADD:
			return "ADD";		break;
		case SUB:
			return "SUB";		break;
		case WHILE:
			return "WHILE";		break;
		case MULT:
			return "MULT";		break;
		case NOT:
			return "NOT";		break;
		case CALL:
			return "CALL";		break;
		case DIV:
			return "DIV";		break;
		case LE:
			return "LE";		break;
		case LT:
			return "LT";		break;
		case NE:
			return "NE";		break;
		case EQ:
			return "EQ";		break;
		case GE:
			return "GE";		break;
		case GT:
			return "GT";		break;
		case AND:
			return "AND";		break;
		case OR:
			return "OR";		break;
		case NEG:
			return "NEG";		break;
		case ASSIGN:
			return "ASSIGN";	break;
		case IFN:
			return "IFN";		break;
		case GOTO:
			return "GOTO";		break;
		case READ:
			return "READ";		break;
		case WRITE:
			return "WRITE";		break;
		case HALT:
			return "HALT";		break;
		case undefinedOP:
		default:
			return "";			break;
		}
	}

};

Code* add(Code* &c1, Code* &c2)//to concate two opjects of code in the first code
{
	Code* temp = c1;
	if (temp != NULL) {
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = c2;
		//c1 = temp;
		return c1;
	}
	else {
		temp = c2;
		return temp;
	}
	//return temp;
}

//---------------------------AST_NODE----------------------
enum NodeKind { Program_N, Block_N, Dec_N, DecS_N, Com_N, ComS_N, Para_N, Id_N, IdS_N, Expr_N, Element_N, Negative_N, Type_N, undefined_N };
enum DecKind { var_D, proc_D, undefined_D };
enum ComKind { assign_cmd, read_cmd, write_cmd, ifThenElse_cmd, whileDo_cmd, call_cmd, block_cmd, undefined_cmd };
enum ExprKind { Or_expr, And_expr, Not_expr, Relation_expr, WeakOp_expr, StrongOp_expr, undefined_expr };
enum ElementKind { Number_elem, Id_elem, expr_elem, negative_elem, undefined_elem };
enum TypeKind { integer_type, boolean_type, undefined_type };

struct AST_Node {
public:
	NodeKind nodeKind;
	DecKind decKind;
	ComKind comKind;
	ExprKind exprKind;
	ElementKind elementKind;
	TypeKind typeKind;
	string name;
	vector<AST_Node*> child;
	Code* code;

	AST_Node(int num_of_children)//constructor take the no. of children to build a tree for it
	{
		nodeKind = undefined_N;
		decKind = undefined_D;
		comKind = undefined_cmd;
		exprKind = undefined_expr;
		elementKind = undefined_elem;
		typeKind = undefined_type;
		name = "";
		code = NULL;
		for (int i = 0; i < num_of_children; i++) 
			child.push_back(NULL);
	}

	string dispNodeKind(NodeKind nk)//to display the kind of the node in string
	{
		switch (nk) {
		case Program_N: return "program";		break;
		case Block_N: return "block";			break;
		case Dec_N: return "declaration";		break;
		case DecS_N: return "DeclarationS";		break;
		case Com_N: return "command";			break;
		case ComS_N: return "CommandS";			break;
		case Para_N: return "Parameter";		break;
		case Id_N: return "identifier";			break;
		case IdS_N: return "identifierS";		break;
		case Expr_N: return "expression";		break;
		case Element_N: return "element";		break;
		case Negative_N: return "negative";		break;
		case Type_N: return "type";				break;
		case undefined_N:
		default: return "_";					break;
		}
	}

	string dispDecKind(DecKind dk) //to display the kind of declration in string
	{
		switch (dk) {
		case var_D: return "variable";		break;
		case proc_D: return "procedure";	break;
		case undefined_D:
		default: return "_";				break;
		}
	}

	string dispComKind(ComKind ck) //to display the kind of the command in sting
	{
		switch (ck) {
		case assign_cmd: return "assignment";		break;
		case read_cmd: return "read";				break;
		case write_cmd: return "write";				break;
		case ifThenElse_cmd: return "if then else"; break;
		case whileDo_cmd: return "while do";		break;
		case call_cmd: return "call";				break;
		case undefined_cmd:
		default: return "_";						break;
		}
	}

	string dispExprKind(ExprKind ek)//to display the king of expretion in string 
	{
		switch (ek)
		{
		case Or_expr: return "or";				break;
		case And_expr: return "and";			break;
		case Not_expr: return "not";			break;
		case Relation_expr: return "relation";	break;
		case WeakOp_expr: return "weak op";		break;
		case StrongOp_expr: return "strong op"; break;
		case undefined_expr:
		default: return "_";					break;
		}
	}

	string dispElmentKind(ElementKind ek) //to diaplay the kind of elment in string
	{
		switch (ek) {
		case Number_elem: return "number";		break;
		case Id_elem: return "id";				break;
		case expr_elem: return "expr";			break;
		case negative_elem: return "negative";	break;
		case undefined_elem:
		default: return "_";					break;
		}
	}

	string dispTypeKind(TypeKind tk) //to display the kind of the type in string
	{
		switch (tk) {
		case integer_type: return "integer"; break;
		case boolean_type: return "boolean"; break;
		case undefined_type:
		default: return "_";				break;
		}
	}

	void dispNode() //to display the node
	{
		string N, n, d, c, e1, e2, t;
		N = name == "" ? "_" : name;
		n = dispNodeKind(nodeKind);
		d = dispDecKind(decKind);
		c = dispComKind(comKind);
		e1 = dispExprKind(exprKind);
		e2 = dispElmentKind(elementKind);
		t = dispTypeKind(typeKind);
		cout << "<" << N << "|" << n << "|" << d << "|" << c << "|" << e1 << "|" << e2 << "|" << t << ">" << endl;
	}
};

bool searchParameter_byName(string _pname, vector<Symbol> _stypes)//search for a symbol in a parameter list of a procedure
{
	int size = _stypes.size();
	if (size == 0)
		return false;
	for (int i = 0; i < size; i++) {
		if (_pname == _stypes[i].sname)
			return true;
	}
	return false;
}

Symbol getParameter_byName(string _pname, vector<Symbol> _stypes)//get a symbol in a parameter list of a procedure
{
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
//--------------------------------------------------------------------

void drawNode(AST_Node *node, int tabs = 1, char ch = '\0')
{
	for (int i = 1; i < tabs; i++) {
		cout << "\t";
	}
	if (node == NULL) {
		cout << endl;
		return;
	}
	node->dispNode(); cout << endl;
	bool isLeaf = true;
	for (int i = 0; i < (int)node->child.size(); i++) {
		if (node->child[i] != NULL) {
			isLeaf = false;
			break;
		}
	}
	if (!isLeaf) {
		tabs++;
		for (int i = 0; i < (int)node->child.size(); i++) {
			drawNode(node->child[i], tabs);
		}
	}
}
//*********************************************************************************************************************
SymbolType getSymbolTypeFromNodeTypeKind(TypeKind t) {
	switch (t) {
	case integer_type: return integer; break;
	case boolean_type: return boolean; break;
	case undefined_type:
	default:
		return undefined;
									   break;
	}
}

AST_Node* root;
stack<SymbolTable*> ST;



bool searchSymbol_byName_inStack(string _sname)//search for a symbol in a stack of symbol tables (different scopes from inner to outer)
{
	stack<SymbolTable*> temp = ST;
	SymbolTable* st;
	while (!temp.empty()) {
		st = temp.top();
		if (st->searchSymbol_byName(_sname))
			return true;

		temp.pop();
	}

	return false;
}


Symbol getSymbol_byName_inStack(string _sname)//get a symbol from a stack of symbol tables (different scopes from inner to outer)
{
	stack<SymbolTable*> temp = ST;
	SymbolTable* st;
	while (!temp.empty()) {
		st = temp.top();
		if (st->searchSymbol_byName(_sname))
			return st->getSymbol_byName(_sname);

		temp.pop();
	}

	Symbol sym0;
	return sym0;
}


string result(AST_Node* &node)//return the destination of a code in string
{
	if (node->code == NULL) {
		cout << "----NULL node ----" << endl;
		cout << node->name << endl;
		return node->name;
	}
	else if (node->code->next == NULL)
		return node->code->dest;
	else {
		Code* c = node->code;
		while (c->next != NULL)
			c = c->next;
		return c->next->dest;
	}
}

//----------------------PARSER class  , CODE GENERATION fun in parser class ------------------------

SymbolType checkSemantic(AST_Node* node, vector<Symbol> _stypes = vector<Symbol>(0));

class Parser {
public:
	Parser(string fileName) //Constructor
	{
		sc = new Scanner(fileName);
	}

	//<parse_VEN> ::= <program> EOF
	void parse_VEN() 
	{
		cur_token = sc->getToken(); //get first token in file
		root = program();

		//drawNode(root);
		checkSemantic(root);
		gencode(root);

		//match(END_SOURCE_SY); //problem*!!!

		cout << "Display code generation :" << endl;
		printFun(root);

	}

private:
	Scanner* sc;
	Token cur_token;
	Code* c;

	void syntax_error(Token tk) //fun to display the syntax error 
	{
		cout << "syntax error: " << tk.getTokenType() << " isn't expected !" << endl;
	}

	void match(TokenType _ttype) {
		if (cur_token.ttype == _ttype)
			cout << cur_token.getTokenType() << " matched ttype" << endl;
		else
			syntax_error(cur_token);
		cur_token = sc->getToken(); //get the next token
	}

	//<program> ::= <block>
	//Program <-- Program(Block)
	AST_Node* program() {
		AST_Node* curNode = new AST_Node(1);
		curNode->nodeKind = Program_N;
		curNode->child[0] = block();
		return curNode;
	}

	//<block> ::= begin <declaration-seq> <command-seq> end
	//Block <-- Block(DecSeq , ComSeq)
	AST_Node* block() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Block_N;
		match(BEGIN_SY); 
		curNode->child[0] = dec_seq(); 
		curNode->child[1] = cmd_seq(); 
		match(END_SY); 
		return curNode;
	}

	//<declaration-seq> ::= <declaration> {<declaration>}
	//DecSeq <-- Declaration+
	AST_Node* dec_seq() {
		AST_Node* curNode = dec(); 
		bool isDecS = false;
		while (cur_token.ttype == INTEGER_SY || cur_token.ttype == BOOL_SY || cur_token.ttype == PROC_SY) {
			isDecS = true;
			AST_Node* temp = new AST_Node(2);
			temp->name = " ";
			//first child
			temp->child[0] = curNode;
			//second child
			temp->child[1] = dec();
			curNode = temp;
		}
		if (isDecS)
			curNode->nodeKind = DecS_N;
		return curNode;
	}

	//<declaration> ::= <type> <name-list> | proc <name> [(<parameter-list>)] = <command>
	//Declaration <-- var(type , Ids) | proc( Id , Paras? , command)
	AST_Node* dec() {
		if (cur_token.ttype == INTEGER_SY || cur_token.ttype == BOOL_SY) {
			AST_Node* curNode = new AST_Node(2);
			curNode->nodeKind = Dec_N;
			curNode->decKind = var_D;
			//first child
			curNode->child[0] = type(); 
			//second child
			curNode->child[1] = name_list(); 
			return curNode;
		}
		else if (cur_token.ttype == PROC_SY) {
			AST_Node* curNode = new AST_Node(3);
			curNode->nodeKind = Dec_N;
			curNode->decKind = proc_D;
			match(PROC_SY);

			//first child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->nodeKind = Id_N;
			curNode->child[0]->name = cur_token.tname;
			match(ID_SY);

			if (cur_token.ttype == LPARN_SY) {
				match(LPARN_SY);
				//second child
				curNode->child[1] = para_list();
				match(RPARN_SY);
			}
			match(EQ_SY);
			//third child
			curNode->child[2] = cmd();
			return curNode;
		}
		else {
			syntax_error(cur_token);
			return NULL;
		}
	}

	//<type> ::= integer | Boolean
	//type <-- integer | Boolean
	AST_Node* type() {
		AST_Node* curNode = new AST_Node(0);
		curNode->nodeKind = Type_N;
		if (cur_token.ttype == INTEGER_SY) {
			match(INTEGER_SY);
			curNode->typeKind = integer_type;
		}
		else if (cur_token.ttype == BOOL_SY) {
			match(BOOL_SY);
			curNode->typeKind = boolean_type;
		}
		else
			syntax_error(cur_token);
		return curNode;
	}

	//<parameter-list> ::= <type> <name-list> { ; <type> <name-list>}
	//Paras <-- Paras(type , Ids)+ (SB ;)
	AST_Node* para_list() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Para_N;
		//first child
		curNode->child[0] = type(); 
		//second child
		curNode->child[1] = name_list();
		while (cur_token.ttype == SEMICOL_SY) {
			match(SEMICOL_SY); 
			AST_Node* temp = new AST_Node(2);
			temp->name = ";";
			//first child
			temp->child[0] = curNode;
			//second child
			temp->child[1] = new AST_Node(2);
			temp->child[1]->nodeKind = Para_N;
			//first child
			temp->child[1]->child[0] = type(); 
			//second child
			temp->child[1]->child[1] = name_list();
			curNode = temp;
		}
		return curNode;
	}

	//<name-list> ::= <name> { , <name>}
	//Ids <-- Id+ (SB ,)
	AST_Node* name_list() {
		AST_Node* curNode = new AST_Node(0);
		curNode->name = cur_token.tname;
		curNode->nodeKind = Id_N;
		match(ID_SY);
		bool isIdS = false;
		while (cur_token.ttype == COMMA_SY) {
			isIdS = true;
			match(COMMA_SY);
			AST_Node* temp = new AST_Node(2);
			temp->name = ",";
			temp->child[0] = curNode;
			temp->child[1] = new AST_Node(0);
			temp->child[1]->name = cur_token.tname;
			temp->child[1]->nodeKind = Id_N;
			match(ID_SY);
			curNode = temp;
		}
		if (isIdS)
			curNode->nodeKind = IdS_N;
		return curNode;
	}

	//<command-seq> ::= <command> { ; <command>}
	//ComSeq <-- command+ (SB ;)
	AST_Node* cmd_seq() {
		AST_Node* curNode = cmd();
		bool isComS = false;
		while (cur_token.ttype == SEMICOL_SY) {
			isComS = true;
			match(SEMICOL_SY);
			AST_Node* temp = new AST_Node(2);
			temp->name = ";";
			temp->child[0] = curNode;
			temp->child[1] = cmd();
			curNode = temp;
		}
		if (isComS)
			curNode->nodeKind = ComS_N;
		return curNode;
	}

	//<command> ::= <name> := <expr> |
	//read <name> |
	//write <expr> |
	//if <expr> then <command-seq> [else <command-seq>] end if |
	//while <expr> do <command-seq> end while |
	//call <name> [(<name-list>)] |
	
	//Command <-- Assign(Id , Expr) |
	//Read(Id) |
	//Write(Expr) |
	//IfThenElse (Expr , ComSeq , ComSeq?) |
	//WhileDo(Expr , ComSeq) |
	//Call(Id , Ids?) 

	AST_Node* cmd() 
	{
		if (cur_token.ttype == ID_SY) {
			AST_Node* curNode = new AST_Node(2);
			curNode->nodeKind = Com_N;
			curNode->comKind = assign_cmd;
			//first child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->name = cur_token.tname;
			curNode->child[0]->nodeKind = Id_N;
			match(ID_SY);
			match(ASSIGN_SY);
			curNode->child[1] = expr();
			return curNode;
		}
		else if (cur_token.ttype == READ_SY) {
			AST_Node* curNode = new AST_Node(1);
			curNode->nodeKind = Com_N;
			curNode->comKind = read_cmd;
			match(READ_SY);
			//1 child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->name = cur_token.tname;
			curNode->child[0]->nodeKind = Id_N;
			match(ID_SY);
			return curNode;
		}
		else if (cur_token.ttype == WRITE_SY) {
			AST_Node* curNode = new AST_Node(1);
			curNode->nodeKind = Com_N;
			curNode->comKind = write_cmd;
			match(WRITE_SY); 
			//1 child
			curNode->child[0] = expr();
			return curNode;
		}
		else if (cur_token.ttype == IF_SY) {
			AST_Node* curNode = new AST_Node(3);
			curNode->nodeKind = Com_N;
			curNode->comKind = ifThenElse_cmd;
			match(IF_SY);
			//first child
			curNode->child[0] = expr();
			match(THEN_SY);
			//second child
			curNode->child[1] = cmd_seq();
			if (cur_token.ttype == ELSE_SY) {
				match(ELSE_SY);
				//third child
				curNode->child[2] = cmd_seq();
			}
			match(END_SY);
			match(IF_SY);
			return curNode;
		}
		else if (cur_token.ttype == WHILE_SY) {
			AST_Node* curNode = new AST_Node(2);
			curNode->nodeKind = Com_N;
			curNode->comKind = whileDo_cmd;
			match(WHILE_SY); 
			//first child
			curNode->child[0] = expr(); 
			match(DO_SY); 
			//second child
			curNode->child[1] = cmd_seq();
			match(END_SY);
			match(WHILE_SY);
			return curNode;
		}
		else if (cur_token.ttype == CALL_SY) {
			AST_Node* curNode = new AST_Node(2);
			curNode->nodeKind = Com_N;
			curNode->comKind = call_cmd;
			match(CALL_SY);
			//first child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->name = cur_token.tname;
			curNode->child[0]->nodeKind = Id_N;
			match(ID_SY);
			if (cur_token.ttype == LPARN_SY) {
				match(LPARN_SY);
				curNode->child[1] = name_list();
				match(RPARN_SY);
			}
			return curNode;
		}
		else if (cur_token.ttype == BEGIN_SY)
		{

			AST_Node* curNode = new AST_Node(1);
			curNode->nodeKind = Com_N;
			curNode->comKind = block_cmd;

			curNode->child[0] = block();
			
			return curNode;
		}
		else {
			syntax_error(cur_token);
			return NULL;
		}
	}

	//<expr> ::= <expr1> {or <expr1>}
	//Expr <-- Expr1+ (SB or)
	AST_Node* expr()
	{
		AST_Node* curNode = expr1();
		while (cur_token.ttype == OR_SY) {
			match(OR_SY);
			AST_Node* temp = new AST_Node(2);
			temp->nodeKind = Expr_N;
			temp->exprKind = Or_expr;
			temp->name = "or";
			//first child
			temp->child[0] = curNode;
			//second child
			temp->child[1] = expr1();
			curNode = temp;
		}
		return curNode;
	}

	//<expr1> ::= <expr2> {and <expr2>}
	//Expr1 <-- Expr2+ (SB and)
	AST_Node* expr1() {
		AST_Node* curNode = expr2(); 

		while (cur_token.ttype == AND_SY) {
			match(AND_SY); 
			AST_Node* temp = new AST_Node(2);
			temp->nodeKind = Expr_N;
			temp->exprKind = And_expr;
			temp->name = "and";
			//first child
			temp->child[0] = curNode;
			//second child
			temp->child[1] = expr2(); 
			curNode = temp;
		}
		return curNode;
	}

	//<expr2> ::= <expr3> | not <expr>
	//Expr2 <-- Expr3 | Not(Expr)
	AST_Node* expr2() {
		if (cur_token.ttype == ID_SY || cur_token.ttype == NUMBER_SY || cur_token.ttype == LPARN_SY || cur_token.ttype == MINUS_SY) {
			AST_Node* curNode = expr3();
			//curNode->nodeKind = Expr_N;
			return curNode;
		}
		else if (cur_token.ttype == NOT_SY) {
			AST_Node* curNode = new AST_Node(1);
			match(NOT_SY);
			//1 child
			curNode->child[0] = expr();
			curNode->nodeKind = Expr_N;
			curNode->exprKind = Not_expr;
			curNode->name = "not";
			return curNode;
		}
		else {
			syntax_error(cur_token);
			return NULL;
		}
	}

	//<expr3> ::= <expr4> {<relation> <expr4>}
	//Expr3 <-- Expr4+ (SB relation)
	AST_Node* expr3() {
		AST_Node* curNode = expr4();
		while (cur_token.ttype == LESS_SY || cur_token.ttype == LessEQ_SY || cur_token.ttype == NotEQ_SY
			|| cur_token.ttype == LARGE_SY || cur_token.ttype == LargeEQ_SY || cur_token.ttype == EQ_SY) {
			AST_Node* temp = relation();
			//first child
			temp->child[0] = curNode;
			//second child
			temp->child[1] = expr4(); 
			curNode = temp;
		}
		return curNode;
	}

	//<expr4> ::= <term> {<weak op> <term>}
	//Expr4 <-- Term+ (SB w_op)
	AST_Node* expr4() {
		AST_Node* curNode = term();
		while (cur_token.ttype == PLUS_SY || cur_token.ttype == MINUS_SY) 
		{
			AST_Node* temp = weak_op();
			//first child
			temp->child[0] = curNode;
			//second child
			temp->child[1] = term();
			curNode = temp;
		}
		return curNode;
	}

	//<term> ::= <element> {<strong op> <element>}
	//Term <-- Element+ (SB s_op)
	AST_Node* term() {
		AST_Node* curNode = element();
		while (cur_token.ttype == MULT_SY || cur_token.ttype == DIV_SY) {
			AST_Node* temp = strong_op();
			//first child
			temp->child[0] = curNode;
			//second child
			temp->child[1] = element();
			curNode = temp;
		}
		return curNode;
	}

	//<element> ::= <numeral> | <name> | (<expr>) | -<element>
	//Element <-- Number | Id | Expr | -(Element)
	AST_Node* element() {
		if (cur_token.ttype == NUMBER_SY) {
			AST_Node* curNode = new AST_Node(0);
			curNode->nodeKind = Element_N;
			curNode->name = cur_token.tname;
			curNode->elementKind = Number_elem;
			match(NUMBER_SY);
			return curNode;
		}
		else if (cur_token.ttype == ID_SY) {
			AST_Node* curNode = new AST_Node(0);
			curNode->nodeKind = Element_N;
			curNode->name = cur_token.tname;
			curNode->elementKind = Id_elem;
			match(ID_SY);
			return curNode;
		}
		else if (cur_token.ttype == LPARN_SY) {
			match(LPARN_SY);
			AST_Node* curNode = expr();
			curNode->nodeKind = Element_N;
			curNode->elementKind = expr_elem;
			match(RPARN_SY);
			return curNode;
		}
		else if (cur_token.ttype == MINUS_SY) {
			AST_Node* curNode = new AST_Node(1);
			curNode->nodeKind = Element_N;
			curNode->name = "-";
			curNode->elementKind = negative_elem;
			match(MINUS_SY);
			//1 child
			curNode->child[0] = element();
			return curNode;
		}
		else {
			syntax_error(cur_token);
			return NULL;
		}
	}

	//<relation> ::= < | <= | <> | = | > | >=
	AST_Node* relation() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Expr_N;
		curNode->exprKind = Relation_expr;
		if (cur_token.ttype == LESS_SY) {
			curNode->name = "<";
			match(LESS_SY);
		}
		else if (cur_token.ttype == LessEQ_SY) {
			curNode->name = "<=";
			match(LESS_SY);
		}
		else if (cur_token.ttype == NotEQ_SY) {
			curNode->name = "<>";
			match(NotEQ_SY);
		}
		else if (cur_token.ttype == EQ_SY) {
			curNode->name = "=";
			match(EQ_SY);
		}
		else if (cur_token.ttype == LARGE_SY) {
			curNode->name = ">";
			match(LARGE_SY);
		}
		else if (cur_token.ttype == LargeEQ_SY) {
			curNode->name = "<=";
			match(LargeEQ_SY);
		}
		else {
			syntax_error(cur_token);
		}
		return curNode;
	}

	//<weak op> ::= + | –
	AST_Node* weak_op() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Expr_N;
		curNode->exprKind = WeakOp_expr;
		if (cur_token.ttype == PLUS_SY) {
			curNode->name = "+";
			match(PLUS_SY);
		}
		else if (cur_token.ttype == MINUS_SY) {
			curNode->name = "-";
			match(MINUS_SY);
		}
		else {
			syntax_error(cur_token);
		}
		return curNode;
	}

	//<strong op> ::= * | /
	AST_Node* strong_op() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Expr_N;
		curNode->exprKind = StrongOp_expr;
		if (cur_token.ttype == MULT_SY) {
			curNode->name = "*";
			match(MULT_SY);
		}
		else if (cur_token.ttype == DIV_SY) {
			curNode->name = "/";
			match(DIV_SY);
		}
		else {
			syntax_error(cur_token);
		}
		return curNode;
	}



	//------------------genration code fun---------------------//------------------genration code fun---------------------
	//------------------genration code fun---------------------//------------------genration code fun---------------------
	//------------------genration code fun---------------------//------------------genration code fun---------------------
	//------------------genration code fun---------------------
	AST_Node* gencode(AST_Node* &node)
	{
		//loop to get the tree from the start to end
		int size = node->child.size();
		//cout << "---->>>>" << size << endl;;
		for (int i = 0; i < size; i++)
			if ((node->child[i] != NULL) && node->child[i]->nodeKind != Dec_N && node->child[i]->nodeKind != DecS_N)
			{
				if (node->child[i]->nodeKind == undefined_N && node->child[i]->name == ";")
					node->child[i]->nodeKind = ComS_N;
				gencode(node->child[i]);
			}
		if (node != NULL) {
			switch (node->nodeKind) {
			case Program_N: {
				//code(program) = code(block)
				Code* c = new Code(HALT);
				node->code = add(node->child[0]->code, c);
			} break;
			case Block_N: {
				node->code = node->child[1]->code;
			} break;
			case ComS_N: {
				for (int i = 0; i < size; ++i) {
					node->code = add(node->code, node->child[i]->code);
				}
			} break;
			case Com_N: {
				/* ex:
					x = y
					ADD y T1
					ASSIGN T1 x
				*/
				if (node->comKind == assign_cmd) {
					Code* c = new Code(ASSIGN, result(node->child[1]), "", result(node->child[0]));
					node->code = add(node->code, node->child[0]->code);
					node->code = add(node->code, node->child[1]->code);
					node->code = add(node->code, c);
				}
				else if (node->comKind == read_cmd) {
					/* ex:
					 read a
					*/
					Code* c = new Code(READ, "", "", result(node->child[0]));
					node->code = add(node->code, node->child[0]->code);
					node->code = add(node->code, c);
				}
				else if (node->comKind == write_cmd) {
					/*
						write x
					*/
					Code* c = new Code(WRITE, "", "", result(node->child[0]));
					node->code = add(node->code, node->child[0]->code);
					node->code = add(node->code, c);
				}
				else if (node->comKind == ifThenElse_cmd) {
					/* ex:
					T1=x+y
					T2=T1>z
					IfN T2 goto L
					T3=a+1
					a=T3
					L:
					*/
					//if (node->child.size() == 2)
					string label_1 = newlabel();
					Code* c = new Code(IFN, result(node->child[0]), node->code->getOpCode(GOTO) + label_1, "");
					node->code = add(node->code, node->child[0]->code);
					node->code = add(node->code, c);
					node->code = add(node->code, node->child[1]->code);
					c = new Code(undefinedOP, "", label_1 + ":", "");
					node->code = add(node->code, c);
					/* ex:
					ADD x, y, T1
					GT T1, z , T2
					IfN T2 goto L1
					ADD a, 1, T3
					ASSIGN T3, a
					Goto L2
					L1:
					SUB a, 1, T4
					Assign T$, a
					L2:node
					*/
					if (node->child[2] != NULL)
						node->code = add(node->code, node->child[2]->code);
				}
				else if (node->comKind == whileDo_cmd) {
					/* ex:
					L1:
					T1=x+y
					T2=T1>z
					IfN T2 goto L2
					T3=a-1
					a=T3
					Goto L1
					L2:
					*/
					string label_1 = newlabel();
					string label_2 = newlabel();

					Code* c = new Code(undefinedOP, "", label_1 + ":", "");
					node->code = c;
					node->code = add(node->code, node->child[0]->code);

					c = new Code(IFN, result(node->child[0]), node->code->getOpCode(GOTO) + label_2, "");
					node->code = add(node->code, c);
					node->code = add(node->code, node->child[1]->code);

					c = new Code(GOTO, "", label_1, "");
					node->code = add(node->code, c);

					c = new Code(undefinedOP, "", label_2 + ":", "");
					node->code = add(node->code, c);

				}
				else if (node->comKind == call_cmd) {
					long long counter = 0;
					if (node->child[1] != NULL) {
						AST_Node* temp = node->child[1];
						//counter++;
						while (true) {
							counter++;
							if (temp->child[0]->name == ",")
								temp = temp->child[0];
							else
							{
								counter++;
								break;
							}
						}
						//counter++;
					}
					Code* c = new Code(CALL, node->child[0]->name, counter == 0 ? "" : to_string(counter), "");
					node->code = c; //(counter == 0 ? c : Add(node->code, c));
				}
			} break;

			case Expr_N: {
				if (node->exprKind == Or_expr) {
					string temp = newtemp();
					Code* c = new Code(OR, result(node->child[0]), result(node->child[1]), temp);
					node->code = add(node->code, node->child[0]->code);
					node->code = add(node->code, node->child[1]->code);
					node->code = c;
				}
				else if (node->exprKind == And_expr) {
					string temp = newtemp();
					Code* c = new Code(AND, result(node->child[0]), result(node->child[1]), temp);
					node->code = add(node->code, node->child[0]->code);
					node->code = add(node->code, node->child[1]->code);
					node->code = c;
				}
				else if (node->exprKind == Not_expr) {
					string temp = newtemp();
					Code* c = new Code(NOT, result(node->child[0]), "", temp);
					node->code = add(node->code, node->child[0]->code);
					node->code = c;
				}
				else if (node->exprKind == Relation_expr) {
					if (node->name == "<")
					{
						string temp = newtemp();
						Code* c = new Code(LT, result(node->child[0]), result(node->child[1]), temp);
						node->code = add(node->code, node->child[0]->code);
						node->code = add(node->code, node->child[1]->code);
						node->code = c;
					}
					else if (node->name == "<=")
					{
						string temp = newtemp();
						Code* c = new Code(LE, result(node->child[0]), result(node->child[1]), temp);
						node->code = add(node->code, node->child[0]->code);
						node->code = add(node->code, node->child[1]->code);
						node->code = c;
					}
					else if (node->name == "<>")
					{
						string temp = newtemp();
						Code* c = new Code(NE, result(node->child[0]), result(node->child[1]), temp);
						node->code = add(node->code, node->child[0]->code);
						node->code = add(node->code, node->child[1]->code);
						node->code = c;
					}
					else if (node->name == "=")
					{
						string temp = newtemp();
						Code* c = new Code(EQ, result(node->child[0]), result(node->child[1]), temp);
						node->code = add(node->code, node->child[0]->code);
						node->code = add(node->code, node->child[1]->code);
						node->code = c;
					}
					else if (node->name == ">")
					{
						string temp = newtemp();
						Code* c = new Code(GT, result(node->child[0]), result(node->child[1]), temp);
						node->code = add(node->code, node->child[0]->code);
						node->code = add(node->code, node->child[1]->code);
						node->code = c;
					}
					else if (node->name == ">=")
					{
						string temp = newtemp();
						Code* c = new Code(GE, result(node->child[0]), result(node->child[1]), temp);
						node->code = add(node->code, node->child[0]->code);
						node->code = add(node->code, node->child[1]->code);
						node->code = c;
					}
				}
				else if (node->exprKind == StrongOp_expr) {
					if (node->name == "*")
					{
						string temp = newtemp();
						Code* c = new Code(MULT, result(node->child[0]), result(node->child[1]), temp);
						node->code = add(node->code, node->child[0]->code);
						node->code = add(node->code, node->child[1]->code);
						node->code = c;
					}
					else if (node->name == "/")
					{
						string temp = newtemp();
						Code* c = new Code(DIV, result(node->child[0]), result(node->child[1]), temp);
						node->code = add(node->code, node->child[0]->code);
						node->code = add(node->code, node->child[1]->code);
						node->code = c;
					}
				}
				else if (node->exprKind == WeakOp_expr) {
					if (node->name == "+")
					{
						string temp = newtemp();
						Code* c = new Code(ADD, result(node->child[0]), result(node->child[1]), temp);
						node->code = add(node->code, node->child[0]->code);
						node->code = add(node->code, node->child[1]->code);
						node->code = c;
					}
					else if (node->name == "-")
					{
						string temp = newtemp();
						Code* c = new Code(SUB, result(node->child[0]), result(node->child[1]), temp);
						node->code = add(node->code, node->child[0]->code);
						node->code = add(node->code, node->child[1]->code);
						node->code = c;
					}
				}
			} break;

			case Element_N: {
				if (node->elementKind == Number_elem)
					node->code = new Code(undefinedOP, "", "", node->name);

				else if (node->elementKind == Id_elem) {
					node->code = new Code(undefinedOP, "", "", node->name);
				}

				else if (node->elementKind == negative_elem) {
					string temp = newtemp();
					Code* c = new Code(NEG, result(node->child[0]), "", temp);
					node->code = add(node->code, node->child[0]->code);
					node->code = c;
				}
			}
			}

		}
		return node;
	}

	void printFun(AST_Node* &root)//fun to display operation src1 & src2 if exest and dest
	{
		Code* temp = root->code;
		while (temp->next != NULL)
		{
			if (temp->op != undefinedOP)
				cout << temp->getOpCode(temp->op) << "  " << temp->src1 << "  " << temp->src2 << "  " << temp->dest << endl;
			temp = temp->next;
		}
	}

};

void error(string message)//fun to pause the program if there if error in checkSemantic fun
{
	cout << message << endl;
	system("pause");
	exit(1);
}

//-----------------CHECK SEMANTIC fun--------------------
SymbolType checkSemantic(AST_Node* node, vector<Symbol> _stypes) {

	
	SymbolType t = undefined; //will be returned at the end
	switch (node->nodeKind)
	{
	case Program_N:
	{
		checkSemantic(node->child[0], _stypes); //Block
	} break;
	case Block_N:
	{
		SymbolTable *sub_st = new SymbolTable();

		ST.push(sub_st);
		for (int i = 0; i < _stypes.size(); i++)
			sub_st->insertSymbol(_stypes[i]);

		checkSemantic(node->child[0], _stypes);
		checkSemantic(node->child[1], _stypes);
		ST.pop();

		//checkSemantic(node->child[0], _stypes); //Declarations (adding identifiers)
		//checkSemantic(node->child[1], _stypes); //Commands
	} break;

	case Dec_N:
	{
		switch (node->decKind)
		{
		case var_D:
		{
			SymbolTable* sub_st = ST.top();
			SymbolType stype = getSymbolTypeFromNodeTypeKind(node->child[0]->typeKind); //integer or boolean
			AST_Node* temp = node->child[1]; //Id or Ids
			while (temp->name == ",") { //in case of Ids
				Symbol sym(temp->child[1]->name, stype, variable);
				sub_st->insertSymbol(sym); //insert the declared symbol in the global st
				temp = temp->child[0];
			}
			Symbol sym(temp->name, stype, variable);
			sub_st->insertSymbol(sym); //insert the declared symbol in the global st

			ST.pop();
			ST.push(sub_st);

		} break;
		case proc_D:
		{
			SymbolTable* sub_st = ST.top();
			Symbol sym(node->child[0]->name, procedure_t, procedure_k);
			vector<Symbol> stypes;
			if (node->child[1] != NULL) {
				AST_Node* temp = node->child[1];
				while (temp->name == ";") {
					SymbolType stype = getSymbolTypeFromNodeTypeKind(temp->child[1]->child[0]->typeKind);
					AST_Node* temp2 = temp->child[1]->child[1]; //Id or Ids
					while (temp2->name == ",") { //in case of Ids
						Symbol sym(temp2->child[1]->name, stype, variable);
						stypes.push_back(sym); //insert the parameter of the procedure
						temp2 = temp2->child[0];
					}
					Symbol sym(temp2->name, stype, variable);
					stypes.push_back(sym); //insert the parameter in the parameter list of the procedure
					temp = temp->child[0];
				}

				SymbolType stype = getSymbolTypeFromNodeTypeKind(temp->child[0]->typeKind);
				AST_Node* temp2 = temp->child[1]; //Id or Ids
				while (temp2->name == ",") { //in case of Ids
					Symbol sym(temp2->child[1]->name, stype, variable);
					stypes.push_back(sym); //insert the parameter of the procedure
					temp2 = temp2->child[0];
				}
				Symbol sym(temp2->name, stype, variable);
				stypes.push_back(sym); //insert the parameter of the procedure
			}
			reverse(stypes.begin(), stypes.end());
			sym.sparameters = stypes; //adding the parameter list to the procedure symbol before inserting it into symbol table
			sub_st->insertSymbol(sym);
			checkSemantic(node->child[2], stypes); //command
			ST.pop();
			ST.push(sub_st);
		} break;
		}
	} break;

	case DecS_N:
	{
		AST_Node* temp = node;
		vector<AST_Node*> declarations;
		while (true) {
			if (temp->name == " ")
			{
				declarations.push_back(temp->child[1]);
				temp = temp->child[0];
			}
			else
			{
				declarations.push_back(temp);
				break;
			}
		}

		reverse(declarations.begin(), declarations.end());
		for (int i = 0; i < (int)declarations.size(); i++) {
			cout << "\t\t" << declarations[i]->name << endl;
			checkSemantic(declarations[i], _stypes);
		}

	} break;
	case Com_N:
	{
		SymbolTable* sub_st = ST.top();
		switch (node->comKind)
		{
		case assign_cmd:
		{
			Symbol lhs_sym;
			if (searchParameter_byName(node->child[0]->name, _stypes)) {
				cout << node->child[0]->name << " is found in the parameter list!" << endl;
				lhs_sym = getParameter_byName(node->child[0]->name, _stypes);
			}
			else if (sub_st->searchSymbol_byName(node->child[0]->name)) {
				cout << node->child[0]->name << " is found in the Symbol table!" << endl;
				lhs_sym = sub_st->getSymbol_byName(node->child[0]->name);
			}
			else if (searchSymbol_byName_inStack(node->child[0]->name)) {
				cout << "symbol is in stack!" << endl;
				lhs_sym = getSymbol_byName_inStack(node->child[0]->name);
			}
			else {
				error("Semantic error(1&2): " + node->child[0]->name + " is not declared!");
			}

			Symbol rhs_sym;
			rhs_sym.stype = checkSemantic(node->child[1], _stypes); //expr

			if (lhs_sym.skind != variable)
				cout << "Semantic error(3): The name, " << node->child[0]->name << ", must be a variable!" << endl;
			if (lhs_sym.stype != rhs_sym.stype)
				error("Semantic error(3): The l.h.s (" + lhs_sym.getSymbolType() + ") and the r.h.s (" + rhs_sym.getSymbolType() + ") must be of the same type!");

		} break;

		case read_cmd:
		{
			Symbol sym;
			if (searchParameter_byName(node->child[0]->name, _stypes)) {
				cout << node->child[0]->name << " is found in the parameter list!" << endl;
				sym = getParameter_byName(node->child[0]->name, _stypes);
			}
			else if (sub_st->searchSymbol_byName(node->child[0]->name)) {
				cout << node->child[0]->name << " is found in the Symbol table!" << endl;
				sym = sub_st->getSymbol_byName(node->child[0]->name);
			}
			else if (searchSymbol_byName_inStack(node->child[0]->name)) {
				cout << "symbol is in stack!" << endl;
				sym = getSymbol_byName_inStack(node->child[0]->name);
			}
			else {
				cout << "Semantic error(1&2): " << node->child[0]->name << " is not declared!" << endl;
			}
			if (sym.stype != integer || sym.skind != variable)
				cout << "Semantic error(6): The name, " << node->child[0]->name << ", in read command must be an integer variable!" << endl;
		} break;

		case write_cmd:
		{
			if (searchParameter_byName(node->child[0]->name, _stypes)) {
				cout << node->child[0]->name << " is found in the parameter list!" << endl;
			}
			else if (sub_st->searchSymbol_byName(node->child[0]->name)) {
				cout << node->child[0]->name << " is found in the Symbol table!" << endl;
			}
			else {
				cout << "Semantic error(1&2): " << node->child[0]->name << " is not declared!" << endl;
			}

		} break;

		case ifThenElse_cmd:
		{
			checkSemantic(node->child[0], _stypes); //expr
			checkSemantic(node->child[1], _stypes); //ComSeq
			if (node->child[2] != NULL) {
				checkSemantic(node->child[2], _stypes); //ComSeq
			}
		} break;
		case whileDo_cmd:
		{
			checkSemantic(node->child[0], _stypes); //expr
			checkSemantic(node->child[1], _stypes); //ComSeq
		} break;
		case call_cmd:
		{
			Symbol sym;
			if (searchParameter_byName(node->child[0]->name, _stypes)) {
				cout << node->child[0]->name << " is found in the parameter list!" << endl;
				sym = getParameter_byName(node->child[0]->name, _stypes);
			}
			else if (sub_st->searchSymbol_byName(node->child[0]->name)) {
				cout << node->child[0]->name << " is found in the Symbol table!" << endl;
				sym = sub_st->getSymbol_byName(node->child[0]->name);
			}
			else if (searchSymbol_byName_inStack(node->child[0]->name)) {
				cout << "symbol is in stack!" << endl;
				sym = getSymbol_byName_inStack(node->child[0]->name);
			}
			else {
				cout << "Semantic error(1,2): " << node->child[0]->name << " isn't declared!" << endl;
			}

			if (sym.stype != procedure_t || sym.skind != procedure_k)
				cout << "Semantic error(7): " << node->child[0]->name << " must be of type procedure!" << endl;
			vector<string> paras;
			if (node->child[1] != NULL) { //if there is parameter list
				AST_Node* temp = node->child[1];
				while (temp->name == ",") {
					paras.push_back(temp->child[1]->name);
					temp = temp->child[0];
				}
				paras.push_back(temp->name);
			}
			reverse(paras.begin(), paras.end());
			if (paras.size() == sym.sparameters.size()) {
				for (int i = 0; i < (int)sym.sparameters.size(); i++) {
					if (sub_st->searchSymbol_byName(paras[i])) {
						Symbol _sym = sub_st->getSymbol_byName(paras[i]);
						if (_sym.stype != sym.sparameters[i].stype) {
							cout << "Semantic error(7): parameter No. " << i + 1 << " must be of " << sym.sparameters[i].getSymbolType() << " type!" << endl;
						}
						if (searchSymbol_byName_inStack(sym.sparameters[i].sname)) {
							cout << "parameter is in stack!" << endl;
							_sym = getSymbol_byName_inStack(sym.sparameters[i].sname);
						}
					}
					else {
						cout << "Semantic error(1&2): " << paras[i] << " isn't declared!" << endl;
					}
				}
			}
			else {
				cout << "Semantic error(7): " << node->child[0]->name << " must have " << sym.sparameters.size() << " parameters!" << endl;
			}

		} break;

		case block_cmd:
			checkSemantic(node->child[0]);
		}
		ST.pop();
		ST.push(sub_st);
	} break;

	case ComS_N:
	{
		vector<AST_Node*> commands;
		AST_Node* temp = node;
		while (temp->name == ";") {
			commands.push_back(temp->child[1]);
			temp = temp->child[0];
		}
		commands.push_back(temp);

		reverse(commands.begin(), commands.end());
		for (int i = 0; i < (int)commands.size(); i++) {
			checkSemantic(commands[i], _stypes);
		}

	} break;
	case Expr_N:
	{
		switch (node->exprKind)
		{
		case Or_expr:
		{
			SymbolType t = boolean;
			SymbolType t1;
			AST_Node* temp = node;
			while (temp->exprKind == Or_expr) {
				t1 = checkSemantic(temp->child[1], _stypes);
				if (t1 != boolean) {
					t = undefined;
					error("Semantic error(3): Expression must be of type boolean!");
				}
				temp = temp->child[0];
			}

			t1 = checkSemantic(temp, _stypes);
			if (t1 != boolean) {
				t = undefined;
				error("Semantic error(3): Expression must be of type boolean!");
			}

			return t;
		} break;

		case And_expr:
		{
			SymbolType t = boolean;
			SymbolType t1;
			AST_Node* temp = node;
			while (temp->exprKind == And_expr) {
				t1 = checkSemantic(temp->child[1], _stypes);
				if (t1 != boolean) {
					t = undefined;
					error("Semantic error(3): Expression must be of type boolean!");
				}
				temp = temp->child[0];
			}

			t1 = checkSemantic(temp, _stypes); //the last left node is an expr1
			if (t1 != boolean) {
				t = undefined;
				error("Semantic error(3): Expression must be of type boolean!");
			}

			return t;
		} break;

		case Not_expr:
		{
			SymbolType t = boolean;
			SymbolType t1 = checkSemantic(node->child[0], _stypes);
			if (t1 != boolean) {
				t = undefined;
				error("Semantic error(3): Expression must be of type boolean!");
			}

			return t;

		} break;

		case Relation_expr:
		{
			SymbolType t = boolean;
			SymbolType t1;
			AST_Node* temp = node;
			while (temp->exprKind == Relation_expr) {
				t1 = checkSemantic(temp->child[1], _stypes);
				if (t1 != integer) {
					t = undefined;
					error("Semantic error(3): Expression must be of type integer!");
				}
				temp = temp->child[0];
			}

			t1 = checkSemantic(temp, _stypes);
			if (t1 != integer) {
				t = undefined;
				error("Semantic error(3): Expression must be of type integer!");
			}

			return t;
		} break;

		case WeakOp_expr:
		{
			SymbolType t = integer;
			SymbolType t1;
			AST_Node* temp = node;
			while (temp->exprKind == WeakOp_expr) {
				t1 = checkSemantic(temp->child[1], _stypes);
				if (t1 != integer) {
					t = undefined;
					error("Semantic error(3): Expression must be of type integer!");
				}
				temp = temp->child[0];
			}

			t1 = checkSemantic(temp, _stypes);
			if (t1 != integer) {
				t = undefined;
				error("Semantic error(3): Expression must be of type integer!");
			}

			return t;

		} break;

		case StrongOp_expr:
		{
			SymbolType t = integer;
			SymbolType t1;
			AST_Node* temp = node;
			while (temp->exprKind == StrongOp_expr) {
				t1 = checkSemantic(temp->child[1], _stypes);
				if (t1 != integer) {
					t = undefined;
					error("Semantic error(3): Expression must be of type integer!");
				}
				temp = temp->child[0];
			}

			t1 = checkSemantic(temp, _stypes);
			if (t1 != integer) {
				t = undefined;
				error("Semantic error(3): Expression must be of type integer!");
			}

			return t;

		} break;
		}

	} break;

	case Element_N:
	{
		SymbolTable* sub_st = ST.top();
		switch (node->elementKind)
		{
		case Number_elem:
		{
			return integer;
		} break;
		case Id_elem:
		{
			Symbol sym;
			;
			if (searchParameter_byName(node->name, _stypes)) {
				cout << node->name << " is found in the parameter list!" << endl;
				sym = getParameter_byName(node->name, _stypes);
			}
			else if (sub_st->searchSymbol_byName(node->name)) {
				cout << node->name << " is found in the Symbol table!" << endl;
				sym = sub_st->getSymbol_byName(node->name);
			}
			else {
				error("Semantic error(1&2): " + node->name + " is not declared!");
			}
			return sym.stype; 

		} break;
		case expr_elem:
		{
			node->nodeKind = Expr_N; 
			checkSemantic(node, _stypes);
		} break;
		case negative_elem:
		{
			SymbolType t = integer; 
			SymbolType t1 = checkSemantic(node->child[0], _stypes);
			if (t1 != integer) {
				t = undefined;
				error("Semantic error(3): Expression must be of type integer!");
			}

			return t;

		} break;
		}
		ST.pop();
		ST.push(sub_st);
	} break;
	}
	SymbolType t0 = undefined;
	return t0;
}

void main() {

	Parser p("test.txt");

	p.parse_VEN();

	system("pause");
}