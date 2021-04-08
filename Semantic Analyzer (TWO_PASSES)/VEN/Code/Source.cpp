#include "Scanner.h"
//#include "SymbolTable.h"
#include "AST_Node.h"

AST_Node* root;
SymbolTable ST;


SymbolType checkSemantic(AST_Node*, vector<Symbol> _stypes = vector<Symbol>(0));

//******************************************** EXTRA FUNCTIONS FOR HELP ***********************************************
SymbolType getSymbolTypeFromNodeTypeKind (TypeKind t) {
	switch (t) {
	case integer_type: return integer; break;
    case boolean_type: return boolean; break;
    case undefined_type:
    default:
		return undefined;
        break;
	}
}


//-----------------------------  ---------------------------------------
	//search for a symbol in a parameter list of a procedure
	bool searchParameter_byName(string _pname, vector<Symbol> _stypes) {
		int size = _stypes.size();
		if(size == 0)
			return false;

		for(int i = 0; i < size; i++) {
			if(_pname == _stypes[i].sname)
				return true;
		}

		return false;
	}

	//get a symbol in a parameter list of a procedure
	Symbol getParameter_byName(string _pname, vector<Symbol> _stypes) {
		int size = _stypes.size();
		Symbol sym;
		if(size == 0)
			return sym; //empty Symbol

		for(int i = 0; i < size; i++) {
			if(_pname == _stypes[i].sname)
				return _stypes[i];
		}

		return sym; //empty Symbol
	}
	//--------------------------------------------------------------------


void drawNode(AST_Node *node, int tabs = 1, char ch = '\0') {
	for (int i = 1; i < tabs; i++) {
		cout << "\t";
	}


	if (node == NULL) {
		cout<<endl;
		return;
	}

	node->dispNode(); cout << endl;

	bool isLeaf = true;
	for(int i = 0; i < (int)node->child.size(); i++) {
		if(node->child[i] != NULL) {
			isLeaf = false;
			break;
		}
	}

	if (!isLeaf) {
		tabs++;
		for(int i = 0; i < (int)node->child.size(); i++) {
			drawNode(node->child[i], tabs);
		}
	}
}
//*********************************************************************************************************************


class Parser {
public:
	//Constructor
	Parser(string fileName) {
		sc = new Scanner(fileName);
	}

	//<parse_VEN> ::= <program> EOF
	void parse_VEN() {
		cur_token = sc->getToken(); //get the first token in the program file
		root = program(); //P

		//drawNode(root);
		checkSemantic(root);

		match(END_SOURCE_SY); //P
	}

private:
	Scanner* sc;
	Token cur_token;

	void syntax_error(Token tk) {
		cout<<"Syntax ERROR: "<<tk.getTokenType()<<" is not expected!"<<endl;
	}

	void match(TokenType _ttype) {
		if(cur_token.ttype == _ttype)
			cout<<cur_token.getTokenType()<<" is matched."<<endl;
		else
			syntax_error(cur_token);

		cur_token = sc->getToken(); //get the next token in the program file
	}

	//<program> ::= <block>
	//Program <-- Program(Block)
	AST_Node* program() {
		AST_Node* curNode = new AST_Node(1);
		curNode->nodeKind = Program_N;

		//the only child
		curNode->child[0] = block(); //P

		return curNode;
	}

	//<block> ::= begin <declaration-seq> <command-seq> end
	//Block <-- Block(DecSeq , ComSeq)
	AST_Node* block() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Block_N;
		match(BEGIN_SY); //P
		curNode->child[0] = dec_seq(); //P
		curNode->child[1] = cmd_seq(); //P
		match(END_SY); //P

		return curNode;
	}

	//<declaration-seq> ::= <declaration> {<declaration>}
	//DecSeq <-- Declaration+
	AST_Node* dec_seq() {
		AST_Node* curNode = dec(); //P

		bool isDecS = false;
		while (cur_token.ttype == INTEGER_SY || cur_token.ttype == BOOL_SY || cur_token.ttype == PROC_SY) {
			isDecS = true;
			AST_Node* temp = new AST_Node(2);
			temp->name = " ";
			//first(left) child
			temp->child[0] = curNode;
			//second(right) child
			temp->child[1] = dec(); //P

			curNode = temp;
		}

		if(isDecS)
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
			//first(left) child
			curNode->child[0] = type(); //P
			//second(right) child
			curNode->child[1] = name_list(); //P

			return curNode;
		}
		else if(cur_token.ttype == PROC_SY) {
			AST_Node* curNode = new AST_Node(3);
			curNode->nodeKind = Dec_N;
			curNode->decKind = proc_D;
			match(PROC_SY); //P

			//first(left) child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->nodeKind = Id_N;
			curNode->child[0]->name = cur_token.tname;
			match(ID_SY); //P

			if(cur_token.ttype == LPARN_SY) {
				match(LPARN_SY); //P
				//second(middle) child
				curNode->child[1] = para_list(); //P
				match(RPARN_SY); //P
			}

			match(EQ_SY); //P

			//third(right) child
			curNode->child[2] = cmd(); //P

			return curNode;
		}
		else {
			syntax_error(cur_token); //P
			return NULL;
		}
	}

	//<type> ::= integer | Boolean
	//type <-- integer | Boolean
	AST_Node* type() {
		AST_Node* curNode = new AST_Node(0);
		curNode->nodeKind = Type_N;
		if(cur_token.ttype == INTEGER_SY) {
			match(INTEGER_SY); //P
			curNode->typeKind = integer_type;
		}

		else if(cur_token.ttype == BOOL_SY) {
			match(BOOL_SY);
			curNode->typeKind = boolean_type;
		}
		else
			syntax_error(cur_token); //P

		return curNode;
	}

	//------------------------------------------
	//search for a symbol in a parameter list of a procedure
	bool searchParameter_byName(string _pname, vector<Symbol> _stypes) {
		int size = _stypes.size();
		if(size == 0)
			return false;

		for(int i = 0; i < size; i++) {
			if(_pname == _stypes[i].sname)
				return true;
		}

		return false;
	}

	//get a symbol in a parameter list of a procedure
	Symbol getParameter_byName(string _pname, vector<Symbol> _stypes) {
		int size = _stypes.size();
		Symbol sym;
		if(size == 0)
			return sym; //empty Symbol

		for(int i = 0; i < size; i++) {
			if(_pname == _stypes[i].sname)
				return _stypes[i];
		}

		return sym; //empty Symbol
	}
	//------------------------------------------

	//<parameter-list> ::= <type> <name-list> { ; <type> <name-list>}
	//Paras <-- Paras(type , Ids)+ (SB ;)
	AST_Node* para_list() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Para_N;
		//first(left) child
		curNode->child[0] = type(); //P
		//second(right) child
		curNode->child[1] = name_list(); //P

		while(cur_token.ttype == SEMICOL_SY) {
			match(SEMICOL_SY); //P
			AST_Node* temp = new AST_Node(2);
			temp->name = ";";
			
			//first(left) child
			temp->child[0] = curNode;

			//second(right) child
			temp->child[1] = new AST_Node(2);
			temp->child[1]->nodeKind = Para_N;
			//first(left) child
			temp->child[1]->child[0] = type(); //P
			//second(right) child
			temp->child[1]->child[1] = name_list(); //P

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
		while(cur_token.ttype == COMMA_SY) {
			isIdS = true;
			match(COMMA_SY); //P
			AST_Node* temp = new AST_Node(2);
			temp->name = ",";
			temp->child[0] = curNode;
			temp->child[1] = new AST_Node(0);
			temp->child[1]->name = cur_token.tname;
			temp->child[1]->nodeKind = Id_N;
			match(ID_SY); //P

			curNode = temp;
		}

		if(isIdS)
			curNode->nodeKind = IdS_N;

		return curNode;
	}

	//<command-seq> ::= <command> { ; <command>}
	//ComSeq <-- command+ (SB ;)
	AST_Node* cmd_seq() {
		AST_Node* curNode = cmd(); //P

		bool isComS = false;
		while(cur_token.ttype == SEMICOL_SY) {
			isComS = true;
			match(SEMICOL_SY); //P
			AST_Node* temp = new AST_Node(2);
			temp->name = ";";
			temp->child[0] = curNode;
			temp->child[1] = cmd(); //P

			curNode = temp;
		}

		if(isComS)
			curNode->nodeKind = ComS_N;

		return curNode;
	}

	//<command> ::= <name> := <expr> |
	//read <name> |
	//write <expr> |
	//if <expr> then <command-seq> [else <command-seq>] end if |
	//while <expr> do <command-seq> end while |
	//call <name> [(<name-list>)] |
	//<block>


	//Command <-- Assign(Id , Expr) |
	//Read(Id) |
	//Write(Expr) |
	//IfThenElse (Expr , ComSeq , ComSeq?) |
	//WhileDo(Expr , ComSeq) |
	//Call(Id , Ids?) |
	//Block
	AST_Node* cmd() {
		//<command> ::= <name> := <expr>
		//Command <-- Assign(Id , Expr)
		if(cur_token.ttype == ID_SY) {
			AST_Node* curNode = new AST_Node(2);
			curNode->nodeKind = Com_N;
			curNode->comKind = assign_cmd;

			//first(left) child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->name = cur_token.tname;
			curNode->child[0]->nodeKind = Id_N;
			match(ID_SY); //P

			match(ASSIGN_SY); //P

			curNode->child[1] = expr(); //P

			return curNode;
		}
		//<command> ::= read <name>
		//Command <-- Read(Id)
		else if(cur_token.ttype == READ_SY) {
			AST_Node* curNode = new AST_Node(1);
			curNode->nodeKind = Com_N;
			curNode->comKind = read_cmd;
			match(READ_SY); //P

			//the only child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->name = cur_token.tname;
			curNode->child[0]->nodeKind = Id_N;

			match(ID_SY); //P

			return curNode;
		}
		//<command> ::= write <expr>
		//Command <-- Write(Expr)
		else if(cur_token.ttype == WRITE_SY) {
			AST_Node* curNode = new AST_Node(1);
			curNode->nodeKind = Com_N;
			curNode->comKind = write_cmd;
			match(WRITE_SY); //P

			//the only child
			curNode->child[0] = expr(); //P

			return curNode;
		}
		//<command> ::= if <expr> then <command-seq> [else <command-seq>] end if
		//Command <-- IfThenElse (Expr , ComSeq , ComSeq?)
		else if(cur_token.ttype == IF_SY) {
			AST_Node* curNode = new AST_Node(3);
			curNode->nodeKind = Com_N;
			curNode->comKind = ifThenElse_cmd;
			match(IF_SY); //P
			//first(left) child
			curNode->child[0] = expr(); //P

			match(THEN_SY); //P

			//second(middle) child
			curNode->child[1] = cmd_seq(); //P

			if (cur_token.ttype == ELSE_SY) {
				match(ELSE_SY); //P
				//third(right) child
				curNode->child[2] = cmd_seq(); //P
			}

			match(END_SY); //P
			match(IF_SY); //P

			return curNode;
		}
		//<command> ::= while <expr> do <command-seq> end while
		//Command <-- WhileDo(Expr , ComSeq)
		else if(cur_token.ttype == WHILE_SY) {
			AST_Node* curNode = new AST_Node(2);
			curNode->nodeKind = Com_N;
			curNode->comKind = whileDo_cmd;
			match(WHILE_SY); //P
			//first(left) child
			curNode->child[0] = expr(); //P

			match(DO_SY); //P

			//second(right) child
			curNode->child[1] = cmd_seq(); //P

			match(END_SY); //P
			match(WHILE_SY); //P

			return curNode;
		}
		//<command> ::= call <name> [(<name-list>)]
		//Command <-- Call(Id , Ids?)
		else if(cur_token.ttype == CALL_SY) {
			AST_Node* curNode = new AST_Node(2);
			curNode->nodeKind = Com_N;
			curNode->comKind = call_cmd;

			match(CALL_SY); //P

			//first(left) child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->name = cur_token.tname;
			curNode->child[0]->nodeKind = Id_N;
			match(ID_SY);

			if(cur_token.ttype == LPARN_SY) {
				match(LPARN_SY); //P
				curNode->child[1] = name_list(); //P
				match(RPARN_SY); //P
			}

			return curNode;

		}
		/*//<command> ::= <block>
		//Command <-- Block
		else if(cur_token.ttype == BEGIN_SY) {
			AST_Node* curNode = block(); //P

			return curNode;
		}*/
		else {
			syntax_error(cur_token); //P
			return NULL;
		}
	}

	//<expr> ::= <expr1> {or <expr1>}
	//Expr <-- Expr1+ (SB or)
	AST_Node* expr() {
		AST_Node* curNode = expr1(); //P

		while (cur_token.ttype == OR_SY) {
			match(OR_SY); //P
			AST_Node* temp = new AST_Node(2);
			temp->nodeKind = Expr_N;
			temp->exprKind = Or_expr;

			temp->name = "or";
			//first(left) child
			temp->child[0] = curNode;
			//second(right) child
			temp->child[1] = expr1(); //P

			curNode = temp;
		}

		return curNode;
	}

	//<expr1> ::= <expr2> {and <expr2>}
	//Expr1 <-- Expr2+ (SB and)
	AST_Node* expr1() {
		AST_Node* curNode = expr2(); //P

		while (cur_token.ttype == AND_SY) {
			match(AND_SY); //P
			AST_Node* temp = new AST_Node(2);
			temp->nodeKind = Expr_N;
			temp->exprKind = And_expr;

			temp->name = "and";
			//first(left) child
			temp->child[0] = curNode;
			//second(right) child
			temp->child[1] = expr2(); //P

			curNode = temp;
		}

		return curNode;
	}


	//<expr2> ::= <expr3> | not <expr>
	//Expr2 <-- Expr3 | Not(Expr)
	AST_Node* expr2() {
		if(cur_token.ttype == ID_SY || cur_token.ttype == NUMBER_SY || cur_token.ttype == LPARN_SY || cur_token.ttype == MINUS_SY) {
			AST_Node* curNode = expr3(); //P

			curNode->nodeKind = Expr_N;
			return curNode;
		}
		else if (cur_token.ttype == NOT_SY) {
			AST_Node* curNode = new AST_Node(1);
			match(NOT_SY); //P

			//the only child
			curNode->child[0] = expr(); //P

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
		AST_Node* curNode = expr4(); //P

		while(cur_token.ttype == LESS_SY || cur_token.ttype == LessEQ_SY || cur_token.ttype == NotEQ_SY
			|| cur_token.ttype == LARGE_SY || cur_token.ttype == LargeEQ_SY || cur_token.ttype == EQ_SY) {
				AST_Node* temp = relation(); //P

				//first(left) child
				temp->child[0] = curNode;

				//second(right) child
				temp->child[1] = expr4(); //P

				curNode = temp;
		}

		return curNode;
	}

	//<expr4> ::= <term> {<weak op> <term>}
	//Expr4 <-- Term+ (SB w_op)
	AST_Node* expr4() {
		AST_Node* curNode = term(); //P

		while(cur_token.ttype == PLUS_SY || cur_token.ttype == MINUS_SY) {

			AST_Node* temp = weak_op(); //P

			//first(left) child
			temp->child[0] = curNode;

			//second(right) child
			temp->child[1] = term(); //P

			curNode = temp;
		}

		return curNode;
	}

	//<term> ::= <element> {<strong op> <element>}
	//Term <-- Element+ (SB s_op)
	AST_Node* term() {
		AST_Node* curNode = element(); //P

		while (cur_token.ttype == MULT_SY || cur_token.ttype == DIV_SY) {
			AST_Node* temp = strong_op(); //P

			//first(left) child
			temp->child[0] = curNode;

			//second(right) child
			temp->child[1] = element(); //P

			curNode = temp;
		}

		return curNode;
	}

	//<element> ::= <numeral> | <name> | (<expr>) | -<element>
	//Element <-- Number | Id | Expr | -(Element)
	AST_Node* element() {
		if(cur_token.ttype == NUMBER_SY) {
			AST_Node* curNode = new AST_Node(0);
			curNode->nodeKind = Element_N;
			curNode->name = cur_token.tname;
			curNode->elementKind = Number_elem;
			match(NUMBER_SY); //P
			return curNode;
		}
		else if(cur_token.ttype == ID_SY) {
			AST_Node* curNode = new AST_Node(0);
			curNode->nodeKind = Element_N;
			curNode->name = cur_token.tname;
			curNode->elementKind = Id_elem;
			match(ID_SY); //P
			return curNode;
		}
		else if(cur_token.ttype == LPARN_SY) {
			match(LPARN_SY); //P
			AST_Node* curNode = expr(); //P
			curNode->nodeKind = Element_N;
			curNode->elementKind = expr_elem;
			match(RPARN_SY); //P
			return curNode;
		}
		else if(cur_token.ttype == MINUS_SY) {
			AST_Node* curNode = new AST_Node(1);
			curNode->nodeKind = Element_N;
			curNode->name = "-";
			curNode->elementKind = negative_elem;
			match(MINUS_SY); //P

			//the only child
			curNode->child[0] = element(); //P

			return curNode;
		}
		else {
			syntax_error(cur_token); //P
			return NULL;
		}
	}

	//<relation> ::= < | <= | <> | = | > | >=
	AST_Node* relation() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Expr_N;
		curNode->exprKind = Relation_expr;

		if(cur_token.ttype == LESS_SY) {
			curNode->name = "<";
			match(LESS_SY); //P
		}
		else if(cur_token.ttype == LessEQ_SY) {
			curNode->name = "<=";
			match(LESS_SY); //P
		}
		else if(cur_token.ttype == NotEQ_SY) {
			curNode->name = "<>";
			match(NotEQ_SY); //P
		}
		else if(cur_token.ttype == EQ_SY) {
			curNode->name = "=";
			match(EQ_SY); //P
		}
		else if(cur_token.ttype == LARGE_SY) {
			curNode->name = ">";
			match(LARGE_SY); //P
		}
		else if(cur_token.ttype == LargeEQ_SY) {
			curNode->name = "<=";
			match(LargeEQ_SY); //P
		}
		else {
			syntax_error(cur_token); //P
		}

		return curNode;
	}

	//<weak op> ::= + | –
	AST_Node* weak_op() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Expr_N;
		curNode->exprKind = WeakOp_expr;

		if(cur_token.ttype == PLUS_SY) {
			curNode->name = "+";
			match(PLUS_SY); //P
		}
		else if(cur_token.ttype == MINUS_SY) {
			curNode->name = "-";
			match(MINUS_SY); //P
		}
		else {
			syntax_error(cur_token); //P
		}

		return curNode;
	}

	//<strong op> ::= * | /
	AST_Node* strong_op() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Expr_N;
		curNode->exprKind = StrongOp_expr;

		if(cur_token.ttype == MULT_SY) {
			curNode->name = "*";
			match(MULT_SY); //P
		}
		else if(cur_token.ttype == DIV_SY) {
			curNode->name = "/";
			match(DIV_SY); //P
		}
		else {
			syntax_error(cur_token); //P
		}

		return curNode;
	}

};





//------------------------------------------ SECOND PASS -------------------------------------------------------------------------------------------------
SymbolType checkSemantic(AST_Node* node, vector<Symbol> _stypes) {

	SymbolType t = undefined; //will be returned at the end

	switch (node->nodeKind)
    {
    case Program_N:
    {
		checkSemantic(node->child[0]); //Block
	} break;
    case Block_N:
    {
		checkSemantic(node->child[0]); //Declarations (adding identifiers to the symbol table)
		checkSemantic(node->child[1]); //Commands
	} break;

    case Dec_N:
    {
        switch (node->decKind)
        {
        case var_D:
        {
			//drawNode(node);
			//many symbols with different names, same type, and same kind(variable)
			SymbolType stype = getSymbolTypeFromNodeTypeKind(node->child[0]->typeKind); //integer or boolean
			AST_Node* temp = node->child[1]; //Id or Ids
			while(temp->name == ",") { //in case of Ids
				Symbol sym(temp->child[1]->name,stype,variable);
				
				ST.insertSymbol(sym); //insert the declared symbol in the global symbol table
				temp = temp->child[0];
			}
			Symbol sym(temp->name,stype,variable);
			ST.insertSymbol(sym); //insert the declared symbol in the global symbol table

		} break;

        case proc_D:
        {
			//one symbol with symbol type(procedure_t) and symbol kind(procedure_t)
			Symbol sym(node->child[0]->name,procedure_t,procedure_k);
			vector<Symbol> stypes;
			if(node->child[1] != NULL) { //if there is a parameter list
				AST_Node* temp = node->child[1];
				while(temp->name == ";") {
					//~~~~~~~~~~~~~~ Paras ~~~~~~~~~~~~~~~~~~
					//many parameters with different names, same type, and same kind(variable)
					SymbolType stype = getSymbolTypeFromNodeTypeKind(temp->child[1]->child[0]->typeKind);
					AST_Node* temp2 = temp->child[1]->child[1]; //Id or Ids
					while(temp2->name == ",") { //in case of Ids
						Symbol sym(temp2->child[1]->name,stype,variable);
						stypes.push_back(sym); //insert the parameter in the parameter list of the procedure
						temp2 = temp2->child[0];
					}
					Symbol sym(temp2->name,stype,variable);
					stypes.push_back(sym); //insert the parameter in the parameter list of the procedure
					//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

					temp = temp->child[0];
				}
				//~~~~~~~~~~~~~~ Paras ~~~~~~~~~~~~~~~~~~
				//many parameters with different names, same type, and same kind(variable)
				SymbolType stype = getSymbolTypeFromNodeTypeKind(temp->child[0]->typeKind);
				AST_Node* temp2 = temp->child[1]; //Id or Ids
				while(temp2->name == ",") { //in case of Ids
					Symbol sym(temp2->child[1]->name,stype,variable);
					stypes.push_back(sym); //insert the parameter in the parameter list of the procedure
					temp2 = temp2->child[0];
				}
				Symbol sym(temp2->name,stype,variable);
				stypes.push_back(sym); //insert the parameter in the parameter list of the procedure
				//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			}

			reverse(stypes.begin(),stypes.end());
			sym.sparameters = stypes; //adding the parameter list to the procedure symbol before inserting it into symbol table

			ST.insertSymbol(sym);

			checkSemantic(node->child[2],stypes); //command
		} break;
        /*case undefined_D:
        {} break;
        default:
        {} break;*/
        }
    } break;

    case DecS_N:
    {
		AST_Node* temp = node;
		vector<AST_Node*> declarations;
		while(temp->name == " ") {
			//checkSemantic(temp->child[1]);
			declarations.push_back(temp->child[1]);
			temp = temp->child[0];
		}
		//checkSemantic(temp); //the last left node is a declaration
		declarations.push_back(temp);

		reverse(declarations.begin(),declarations.end());
		for(int i = 0; i < (int)declarations.size(); i++) {
			cout << "\t\t" << declarations[i]->name <<endl;
			checkSemantic(declarations[i]);
		}

	} break;

    case Com_N:
    {
        switch (node->comKind)
        {
        case assign_cmd:
        {
			
			Symbol lhs_sym;
			if(searchParameter_byName(node->child[0]->name,_stypes)) {
				cout << node->child[0]->name << " is found in the parameter list!" << endl;
				lhs_sym = getParameter_byName(node->child[0]->name,_stypes);
			}
			else if(ST.searchSymbol_byName(node->child[0]->name)) {
				cout << node->child[0]->name << " is found in the Symbol table!" << endl;
				lhs_sym = ST.getSymbol_byName(node->child[0]->name);
			}
			else {
				cout << "Semantic Error(1&2): " << node->child[0]->name << " is not declared!" << endl;
			}

			Symbol rhs_sym;
			rhs_sym.stype = checkSemantic(node->child[1],_stypes); //expr

			if(lhs_sym.skind != variable)
				cout << "Semantic Error(3): The name, " << node->child[0]->name << ", must be a variable!" << endl;
			if(lhs_sym.stype != rhs_sym.stype)
				cout << "Semantic Error(3): The l.h.s (" << lhs_sym.getSymbolType() << ") and the r.h.s (" << rhs_sym.getSymbolType() << ") must be of the same type!" << endl;

		} break;

        case read_cmd:
        {
			Symbol sym;
			if(searchParameter_byName(node->child[0]->name,_stypes)) {
				cout << node->child[0]->name << " is found in the parameter list!" << endl;
				sym = getParameter_byName(node->child[0]->name,_stypes);
			}
			else if(ST.searchSymbol_byName(node->child[0]->name)) {
				cout << node->child[0]->name << " is found in the Symbol table!" << endl;
				sym = ST.getSymbol_byName(node->child[0]->name);
			}
			else {
				cout << "Semantic Error(1&2): " << node->child[0]->name << " is not declared!" << endl;
			}

			if(sym.stype != integer || sym.skind != variable)
				cout << "Semantic Error(6): The name, " << node->child[0]->name << ", in read command must be an integer variable!" << endl;


		} break;

        case write_cmd:
        {
			if(searchParameter_byName(node->child[0]->name,_stypes)) {
				cout << node->child[0]->name << " is found in the parameter list!" << endl;
			}
			else if(ST.searchSymbol_byName(node->child[0]->name)) {
				cout << node->child[0]->name << " is found in the Symbol table!" << endl;
			}
			else {
				cout << "Semantic Error(1&2): " << node->child[0]->name << " is not declared!" << endl;
			}

		} break;

        case ifThenElse_cmd:
        {
			checkSemantic(node->child[0]); //expr
			checkSemantic(node->child[1]); //ComSeq
			if(node->child[2] != NULL) { //if there is else
				checkSemantic(node->child[2]); //ComSeq
			}
		} break;
        case whileDo_cmd:
        {
			checkSemantic(node->child[0]); //expr
			checkSemantic(node->child[1]); //ComSeq
		} break;
        case call_cmd:
        {
			Symbol sym;
			if(searchParameter_byName(node->child[0]->name,_stypes)) {
				cout << node->child[0]->name << " is found in the parameter list!" << endl;
				sym = getParameter_byName(node->child[0]->name,_stypes);
			}
			else if(ST.searchSymbol_byName(node->child[0]->name)) {
				cout << node->child[0]->name << " is found in the Symbol table!" << endl;
				sym = ST.getSymbol_byName(node->child[0]->name);
			}
			else {
				cout << "Semantic Error(1&2): " << node->child[0]->name << " is not declared!" << endl;
			}

			if(sym.stype != procedure_t || sym.skind != procedure_k)
				cout << "Semantic Erro(7): " << node->child[0]->name << " must be of type procedure!" << endl;

			vector<string> paras;
			if(node->child[1] != NULL) { //if there is parameter list
				AST_Node* temp = node->child[1];
				while(temp->name == ",") {
					paras.push_back(temp->child[1]->name);
					temp = temp->child[0];
				}
				paras.push_back(temp->name); //the last left node is an id
			}

			reverse(paras.begin(),paras.end());
			if(paras.size() == sym.sparameters.size()) {
				for(int i = 0; i < (int)sym.sparameters.size(); i++) {
					if(ST.searchSymbol_byName(paras[i])) {
						Symbol _sym = ST.getSymbol_byName(paras[i]);
						if (_sym.stype != sym.sparameters[i].stype) {
							cout << "Semantic Error(7): parameter No. " << i+1 << " must be of " << sym.sparameters[i].getSymbolType() <<" type!"<< endl;
						}
					}
					else {
						cout << "Semantic Error(1&2): " << paras[i] << " is not declared!" << endl;
					}
				}
			}
			else {
				cout << "Semantic Erro(7): " << node->child[0]->name << " must have " << sym.sparameters.size() << " parameters!" << endl;
			}

		} break;

		/*case block_cmd: ->>>>> SCOPE
        {} break;
        case undefined_cmd:
        {} break;
        default:
        {} break;*/
        }
    } break;

    case ComS_N:
	{
		vector<AST_Node*> commands;
		AST_Node* temp = node;
		while(temp->name == ";") {
			//checkSemantic(temp->child[1]);
			commands.push_back(temp->child[1]);
			temp = temp->child[0];
		}
		//checkSemantic(temp); //the last left node is a command
		commands.push_back(temp);

		reverse(commands.begin(),commands.end());
		for(int i = 0; i < (int)commands.size(); i++) {
			checkSemantic(commands[i]);
		}

	} break;

    /*case Para_N:
    {} break;
    case Id_N:
    {} break;
    case IdS_N:
    {} break;*/
	case Expr_N:
	{
		switch (node->exprKind)
		{
		case Or_expr:
		{
			SymbolType t = boolean; //because of 'or'
			SymbolType t1;
			AST_Node* temp = node;
			while(temp->exprKind == Or_expr) {
				t1 = checkSemantic(temp->child[1],_stypes);
				if(t1 != boolean) {
					t = undefined;
					cout<<"Semantic ERROR(3): Expression must be of type boolean!"<<endl;
				}
				temp = temp->child[0];
			}

			t1 = checkSemantic(temp,_stypes); //the last left node is an expr1
			if(t1 != boolean) {
				t = undefined;
				cout<<"Semantic ERROR(3): Expression must be of type boolean!"<<endl;
			}

			return t; //boolean or undefined(when one of the expr1's separated by 'or' is not boolean)
		} break;

		case And_expr:
		{
			SymbolType t = boolean; //because of 'and'
			SymbolType t1;
			AST_Node* temp = node;
			while(temp->exprKind == And_expr) {
				t1 = checkSemantic(temp->child[1],_stypes);
				if(t1 != boolean) {
					t = undefined;
					cout<<"Semantic ERROR(3): Expression must be of type boolean!"<<endl;
				}
				temp = temp->child[0];
			}

			t1 = checkSemantic(temp,_stypes); //the last left node is an expr1
			if(t1 != boolean) {
				t = undefined;
				cout<<"Semantic ERROR(3): Expression must be of type boolean!"<<endl;
			}

			return t; //boolean or undefined(when one of the expr2's separated by 'and' is not boolean)
		} break;

		case Not_expr:
		{
			SymbolType t = boolean; //because of 'not'
			SymbolType t1 = checkSemantic(node->child[0],_stypes);
			if(t1 != boolean) {
				t = undefined;
				cout<<"Semantic ERROR(3): Expression must be of type boolean!"<<endl;
			}

			return t; //boolean or undefined(when the expr after 'not' is not integer)

		} break;

		case Relation_expr:
		{
			SymbolType t = boolean; //because of 'relation'
			SymbolType t1;
			AST_Node* temp = node;
			while(temp->exprKind == Relation_expr) {
				t1 = checkSemantic(temp->child[1],_stypes);
				if(t1 != integer) {
					t = undefined;
					cout<<"Semantic ERROR(3): Expression must be of type integer!"<<endl;
				}
				temp = temp->child[0];
			}

			t1 = checkSemantic(temp,_stypes); //the last left node is an expr1
			if(t1 != integer) {
				t = undefined;
				cout<<"Semantic ERROR(3): Expression must be of type integer!"<<endl;
			}

			return t; //boolean or undefined(when one of the expr4's separated by 'relation' is not integer)
		} break;

		case WeakOp_expr:
		{
			SymbolType t = integer; //because of 'weak operation'
			SymbolType t1;
			AST_Node* temp = node;
			while(temp->exprKind == WeakOp_expr) {
				t1 = checkSemantic(temp->child[1],_stypes);
				if(t1 != integer) {
					t = undefined;
					cout<<"Semantic ERROR(3): Expression must be of type integer!"<<endl;
				}
				temp = temp->child[0];
			}

			t1 = checkSemantic(temp,_stypes); //the last left node is an expr1
			if(t1 != integer) {
				t = undefined;
				cout<<"Semantic ERROR(3): Expression must be of type integer!"<<endl;
			}

			return t; //integer or undefined(when one of the term's separated by 'weak op' is not integer)

		} break;

		case StrongOp_expr:
		{
			SymbolType t = integer; //because of 'weak operation'
			SymbolType t1;
			AST_Node* temp = node;
			while(temp->exprKind == StrongOp_expr) {
				t1 = checkSemantic(temp->child[1],_stypes);
				if(t1 != integer) {
					t = undefined;
					cout<<"Semantic ERROR(3): Expression must be of type integer!"<<endl;
				}
				temp = temp->child[0];
			}

			t1 = checkSemantic(temp,_stypes); //the last left node is an expr1
			if(t1 != integer) {
				t = undefined;
				cout<<"Semantic ERROR(3): Expression must be of type integer!"<<endl;
			}

			return t; //integer or undefined(when one of the term's separated by 'strong op' is not integer)

		} break;

		/*case undefined_expr:
		  {} break;
		default:
		  {} break;*/

		}

	} break;

    case Element_N:
    {
        switch (node->elementKind)
        {
        case Number_elem:
        {
			return integer; //The expression is a number, so its type is integer
		} break;
        case Id_elem:
        {
			Symbol sym;
			if(searchParameter_byName(node->name,_stypes)) {
				cout << node->name << " is found in the parameter list!" << endl;
				sym = getParameter_byName(node->name,_stypes);
			}
			else if(ST.searchSymbol_byName(node->name)) {
				cout << node->name << " is found in the Symbol table!" << endl;
				sym = ST.getSymbol_byName(node->name);
			}
			else {
				cout << "Semantic Error(1&2): " << node->name << " is not declared!" << endl;
			}
			return sym.stype; //The id type is the type of the expression

		} break;
        case expr_elem:
        {
			node->nodeKind = Expr_N; //Go to expr branch
			checkSemantic(node,_stypes);
		} break;
        case negative_elem:
        {
			SymbolType t = integer; //because of '-'
			SymbolType t1 = checkSemantic(node->child[0],_stypes);
			if(t1 != integer) {
				t = undefined;
				cout<<"Semantic ERROR(3): Expression must be of type integer!"<<endl;
			}

			return t;

		} break;

        /*case undefined_elem:
        {} break;
        default:
        {} break;*/

        }
    } break;
    
	/*case undefined_N:
    {} break;
    default:
    {} break;*/
    
	}
	SymbolType t0 = undefined;
	return t0;
}




void main() {
	//Scanner sc("test.txt");
	//sc.displayTokens();

	Parser p("test.txt");

	p.parse_VEN();

	system("pause");
}