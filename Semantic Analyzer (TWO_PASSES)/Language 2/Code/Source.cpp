#include "Scanner.h"
#include "AST_Node.h"


AST_Node* root;
SymbolTable ST;

SymbolType getSymbolTypeFromNodeTypeKind (TypeKind t) {
	switch (t) {
	case program_type: return Program; break;
	case int_type: return Int; break;
	case bool_type: return Bool; break;
	case char_type: return Char; break;
	default: return Undefined_type; break;
	}
}



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


void checkSemantic(AST_Node*);

class Parser {
public:

	Parser(string fileName) {
		sc = new Scanner(fileName);
	}

	void parsee() {
		cur_token = sc->getToken();
		root = program(); //P
		//drawNode(root);
		checkSemantic(root);
		//ST.disp();
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

	//<program> ::= program <id> is <block>
    //Program <-- Program(program, id , Block)
	AST_Node* program() {
		AST_Node* curNode = new AST_Node(3);
		curNode->nodeKind = Program_N;

		//first(left) child
		curNode->child[0] = new AST_Node(0);
		curNode->child[0]->name = "program";
		
		match(Program_SY); //P

		//second(middle) child
		curNode->child[1] = new AST_Node(0);
		curNode->child[1]->nodeKind = ID_N;
		curNode->child[1]->name = cur_token.tname;
		curNode->child[1]->typeKind = program_type;

		match(ID_SY); //P
		match(Is_SY); //P

		//third(right) child
		curNode->child[2] = block(); //P

		return curNode;
	}

	//<block> ::= <declseq> begin <com-seq> end
	//Block <-- Block(DecSeq, ComSeq)
	AST_Node* block() {
		AST_Node* curNode = new AST_Node(2);
		curNode->nodeKind = Block_N;
		
		//first(left) child
		curNode->child[0] = declSeq(); //P
		//drawNode(curNode->child[0]);

		match(Begin_SY); //P

		//second(right) child
		curNode->child[1] = comSeq(); //P
		//drawNode(curNode->child[1]);

		match(End_SY); //P
		return curNode;
	}

	//<declseq> ::= <decl> { , <decl> }
	//declseq> <-- Decl+ (SB ,)
	AST_Node* declSeq() {
		AST_Node* curNode = decl(); //P

		bool isDecS = false;
		while (cur_token.ttype == Comma_SY) {
			isDecS = true;
			match(Comma_SY); //P
			AST_Node* temp = new AST_Node(2);
			temp->name = ",";
			//first(left) child
			temp->child[0] = curNode;
			//second(right) child
			temp->child[1] = decl(); //P


			curNode = temp;
		}
		if(isDecS)
			curNode->nodeKind = DecS_N;

		return curNode;
	}

	//<decl> ::= <type> <id> | const <type> <id> = <expr>
	//Decl <-- var(type, id) | const(type, id , Expr ) )
	AST_Node* decl() {
		
		if (cur_token.ttype == Int_SY || cur_token.ttype == Bool_SY || cur_token.ttype == Char_SY) {
			AST_Node* curNode = new AST_Node(2);
			curNode->nodeKind = Dec_N;
			curNode->decKind = var_D;

			//first(left) child
			curNode->child[0] = type(); //P

			//second(right) child
			curNode->child[1] = new AST_Node(0);
			curNode->child[1]->nodeKind = ID_N;
			curNode->child[1]->name = cur_token.tname;

			match(ID_SY); //P

			return curNode;
		}
		else if (cur_token.ttype == Const_SY) {
			match(Const_SY); //P
			AST_Node* curNode = new AST_Node(3);
			curNode->nodeKind = Dec_N;
			curNode->decKind = const_D;

			//first(left) child
			curNode->child[0] = type(); //P
			
			//second(middle) child
			curNode->child[1] = new AST_Node(0);
			curNode->child[1]->nodeKind = ID_N;
			curNode->child[1]->name = cur_token.tname;

			match(ID_SY); //P
			match(Equal_SY); //P

			//third(right) child
			curNode->child[2] = expr(); //P

			return curNode;
		}
		else {
			syntax_error(cur_token); //P
			return NULL;
		}
	}

	//<type> ::= int | bool | char
	AST_Node* type() {
		AST_Node* curNode = new AST_Node(0);
		curNode->nodeKind = Type_N;
		if(cur_token.ttype == Int_SY) { 
			match(Int_SY); //P
			curNode->typeKind = int_type;
		}
		else if(cur_token.ttype == Bool_SY) {
			match(Bool_SY); //P
			curNode->typeKind = bool_type;
		}
		else if(cur_token.ttype == Char_SY) {
			match(Char_SY); //P
			curNode->typeKind = char_type;
		}
		else {
			syntax_error(cur_token);
			return NULL;
		}

		return curNode;
	}


	//<comSeq> ::= <com> { ; <com> }
	//comSeq <-- Com+ (SB ;)
	AST_Node* comSeq() {
		AST_Node* curNode = com(); //P

		bool isComS = false;
		while (cur_token.ttype == SemiColon_SY) {
			isComS = true;
			match(SemiColon_SY); //P
			AST_Node* temp = new AST_Node(2);
			temp->name = ";";
			//first(left) child
			temp->child[0] = curNode;
			//second(right) child
			temp->child[1] = com(); //P

			curNode = temp;
		}

		if(isComS) {
			curNode->nodeKind = ComS_N;
		}

		return curNode;
	}


	//<com> ::= if <expr> then <com-seq> end if | <id> := <expr> | write <expr> | read <id>
	//Com <-- IfThen (Expr , Com+ (SB ;)) | Assign(id , Expr) | Write(Expr) | Read(id)
	AST_Node* com() {
		if (cur_token.ttype == If_SY) {
			match(If_SY); //P
			AST_Node* curNode = new AST_Node(2);
			curNode->nodeKind = Com_N;
			curNode->comKind = ifThen_cmd;
			//first(left) child
			curNode->child[0] = expr(); //P

			match(Then_SY);

			//second(right) child
			curNode->child[1] = comSeq(); //P

			match(End_SY);
			match(If_SY);

			return curNode;
		}
		else if (cur_token.ttype == ID_SY) {
			AST_Node* curNode = new AST_Node(2);
			curNode->nodeKind = Com_N;
			curNode->comKind = assign_cmd;

			//first(left) child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->nodeKind = ID_N;
			curNode->child[0]->name = cur_token.tname;

			match(ID_SY); //P
			match(Assign_SY); //P

			//second(right) child
			curNode->child[1] = expr(); //P

			return curNode;

		}
		else if (cur_token.ttype == Write_SY) {
			match(Write_SY); //P
			AST_Node* curNode = new AST_Node(1);
			curNode->nodeKind = Com_N;
			curNode->comKind = write_cmd;

			//the only child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->nodeKind = ID_N;
			curNode->child[0]->name = cur_token.tname;

			match(ID_SY); //P

			return curNode;
		}
		else if (cur_token.ttype == Read_SY) {
			match(Read_SY); //P
			AST_Node* curNode = new AST_Node(1);
			curNode->nodeKind = Com_N;
			curNode->comKind = read_cmd;

			//the only child
			curNode->child[0] = new AST_Node(0);
			curNode->child[0]->nodeKind = ID_N;
			curNode->child[0]->name = cur_token.tname;

			match(ID_SY); //P

			return curNode;
		}
		else {
			syntax_error(cur_token); //P
			return NULL;
		}
	}


	//<expr> ::= num | true | false | character
	AST_Node* expr() {
		AST_Node* curNode = new AST_Node(0);
		curNode->nodeKind = Expr_N;

		if (cur_token.ttype == Number_SY) {
			curNode->exprKind = num_expr;
			curNode->typeKind = int_type;
			curNode->name = cur_token.tname;
			match(Number_SY); //P
		}
		else if (cur_token.ttype == True_SY) {
			curNode->exprKind = true_expr;
			curNode->typeKind = bool_type;
			match(True_SY); //P
		}
		else if (cur_token.ttype == False_SY) {
			curNode->exprKind = false_expr;
			curNode->typeKind = bool_type;
			match(False_SY); //P
		}
		else if (cur_token.ttype == Character_SY) {
			curNode->exprKind = character_expr;
			curNode->typeKind = char_type;
			match(Character_SY); //P
		}
		else {
			syntax_error(cur_token); //P
			return NULL;
		}

		return curNode;
	}

};



//------------------------------------------ SECOND PASS -------------------------------------------------------------
void checkSemantic(AST_Node* node) {
	switch(node->nodeKind) {
	case Program_N:
		{
			Symbol sym(node->child[1]->name,Program,Undefined_kind); //add identifier in the symbol table
 			ST.insertSymbol(sym);
			checkSemantic(node->child[2]);
		} break;
	case Block_N:
		{
			checkSemantic(node->child[0]); //declarations (adding identifiers to the symbol table)
			checkSemantic(node->child[1]); //commands
		} break;
	case Dec_N:
		{
			if(node->decKind == const_D) {
				Symbol sym(node->child[1]->name,getSymbolTypeFromNodeTypeKind(node->child[0]->typeKind),Constant);
				ST.insertSymbol(sym);
			}
			else if (node->decKind == var_D) {
				Symbol sym(node->child[1]->name,getSymbolTypeFromNodeTypeKind(node->child[0]->typeKind),Variable);
				ST.insertSymbol(sym);
			}
		} break;
	case Com_N:
		{
			if(node->comKind == ifThen_cmd) {
				if (node->child[0]->typeKind != bool_type) {
					cout << "Semantic Error(5): The type of the expression in if statement must be Boolean!" << endl;
				}
				checkSemantic(node->child[1]);
			}
			else if (node->comKind == assign_cmd) {
				if (!ST.searchSymbol_byName(node->child[0]->name)) {
					cout << "Semantic Error(3): Identifier '" << node->child[0]->name <<"' is not declared!" << endl;
				}
				else {
					Symbol LHS_sym = ST.getSymbol_byName(node->child[0]->name);
					if(LHS_sym.stype == Program) {
						cout << "Semantic Error(1): Program name "<< node->child[0]->name <<" must not be declared elsewhere in the program!" <<endl;
					}
					if (LHS_sym.stype != getSymbolTypeFromNodeTypeKind(node->child[1]->typeKind)) {
						Symbol RHS_sym;
						RHS_sym.stype = getSymbolTypeFromNodeTypeKind(node->child[1]->typeKind);
						cout << "Semantic Error(4): l.h.s (" << LHS_sym.dispSymbolType() << ") and r.h.s (" << RHS_sym.dispSymbolType() << ") must be of the same type!" << endl;   
					}
				}
			}
			else if (node->comKind == write_cmd) {
				if (!ST.searchSymbol_byName(node->child[0]->name)) {
					cout << "Semantic Error(3): Identifier '" << node->child[0]->name <<"' is not declared!" << endl;
				}
				else {
					Symbol sym = ST.getSymbol_byName(node->child[0]->name);
					if(sym.stype == Program) {
						cout << "Semantic Error(1): Program name "<< node->child[0]->name <<" must not be declared elsewhere in the program!" <<endl;
					}

				}
			}
			else if (node->comKind == read_cmd) {
				if (!ST.searchSymbol_byName(node->child[0]->name)) {
					cout << "Semantic Error(3): Identifier '" << node->child[0]->name <<"' is not declared!" << endl;
				}
				else {
					Symbol sym = ST.getSymbol_byName(node->child[0]->name);
					if(sym.stype == Program) {
						cout << "Semantic Error(1): Program name "<< node->child[0]->name <<" must not be declared elsewhere in the program!" <<endl;
					}
					if (sym.skind != Variable && (sym.stype != Int || sym.stype != Char)) {
						cout << "Semantic Error(6): Identifier '" << node->child[0]->name << "' in read statement must be declared as variable and of type int or char!" << endl;
					}
				}
			}
		} break;
	/*case Expr_N:
		break;
	case ID_N:
		break;*/
	case DecS_N:
		{
			AST_Node* temp = node;
			while (temp->name == ",") {
				checkSemantic(temp->child[1]);
				temp = temp->child[0];
			}
			checkSemantic(temp); //the last left node is a declaration
		} break;
	case ComS_N:
		{
			AST_Node* temp = node;
			while (temp->name == ";") {
				checkSemantic(temp->child[1]);
				temp = temp->child[0];
			}
			checkSemantic(temp); //the last left node is a command
		} break;
	/*case Type_N:
		break;
	default:
		break;*/
	}
}



void main () {

	root = new AST_Node(1);

	
	//Scanner sc("test.txt");
	//sc.displayTokens();

	Parser p("test.txt");
	p.parsee();
	//drawNode(root);
	//checkSemantic(root);

	system("pause");
}