#include "Scanner.h"
#include "SymbolTable.h"


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
	
	//<declaration-seq> ::= <declaration> {<declaration>}
	void dec_seq(SymbolTable & _ST) {
		dec(_ST);
		while (cur_token.ttype == INTEGER_SY || cur_token.ttype == BOOL_SY || cur_token.ttype == PROC_SY)
			dec(_ST);
	}
	
	//<declaration> ::= <type> <name-list> |
	//proc <name> [(<parameter-list>)] = <command>
	void dec(SymbolTable & _ST) {
		if (cur_token.ttype == INTEGER_SY || cur_token.ttype == BOOL_SY) {
			//insert list of symbols with different names but same type in the symbol table
			SymbolType _stype = type(); //their type is the same
			vector<string> _snames = name_list(); //but different names

			_ST.insertSymbolList(_stype,_snames); //in this function every symbol has the kind variable 
												  //AND it checks if any of the symbol names is already in the symbol table

		}
		else if(cur_token.ttype == PROC_SY) {
			//insert a symbol with kind and type 'procedure' and name of the ID  in the symbol table
			Symbol sym;
			sym.stype = procedure_t; //its type is 'procedure'
			sym.skind = procedure_k; //its kind is 'procedure'
			match(PROC_SY);
			sym.sname = cur_token.tname; //its name is the id name
			match(ID_SY);
			if(cur_token.ttype == LPARN_SY) {
				match(LPARN_SY);
				sym.sparameters = para_list(); //its parameters
				match(RPARN_SY);
			}

			_ST.insertSymbol(sym); //in this function it checks if the symbol name is already in the symbol table

			match(EQ_SY);

			cmd(_ST,sym.sparameters); //because every symbol appears inside a command MUST be declared
			                          //in the parameters of the procedure (if there is one) OR in the symbol table
		}
		else {
			syntax_error(cur_token);
		}
	}
	
	//<type> ::= integer | Boolean
	SymbolType type() {
		if(cur_token.ttype == INTEGER_SY) {
			match(INTEGER_SY);
			return integer;
		}

		if(cur_token.ttype == BOOL_SY) {
			match(BOOL_SY);
			return boolean;
		}

		syntax_error(cur_token);
	}

	//------------------------------------------
	//search for a symbol in a parameter list of a procedure
	bool searchParameter_byName(string _pname, vector<Symbol> _stypes) {
		int size = _stypes.size();
		if(size == 0)
			return true;

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
	vector<Symbol> para_list() {
		vector<Symbol> syms;
		SymbolType _stype;
		_stype = type();
		vector<string> _snames;
		_snames = name_list();
		for(int i = 0; i < (int)_snames.size(); i++) {
			Symbol s(_snames[i],_stype,variable);
			if(searchParameter_byName(s.sname,syms) && i != 0) {//i != 0 because when the parameter list is empty the search function returns true
				cout<<"Semantic ERROR(1): The parameter "<<_snames[i]<<" appears more than once in the same parameter list!"<<endl;
				continue;
			}
			syms.push_back(s);
		}

		while(cur_token.ttype == SEMICOL_SY) {
			match(SEMICOL_SY);
			_stype = type();
			_snames = name_list();
			for(int i = 0; i < (int)_snames.size(); i++) {
				Symbol s(_snames[i],_stype,variable);
				if(searchParameter_byName(s.sname,syms)) {
					cout<<"Semantic ERROR(1): The parameter "<<_snames[i]<<" appears more than once in the same parameter list!"<<endl;
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

		while(cur_token.ttype == COMMA_SY) {
			match(COMMA_SY);
			snames.push_back(cur_token.tname);
			match(ID_SY);
		}

		return snames;
	}
	
	//<command-seq> ::= <command> { ; <command>}
	void cmd_seq(SymbolTable _ST) {
		//Command here MUST have symbols only declared in the symbol table
		cmd(_ST);

		while(cur_token.ttype == SEMICOL_SY) {
			match(SEMICOL_SY);
			cmd(_ST);
		}
	}

	//<command> ::= <name> := <expr> |
	//read <name> |
	//write <expr> |
	//if <expr> then <command-seq> [else <command-seq>] end if |
	//while <expr> do <command-seq> end while |
	//call <name> [(<name-list>)] |
	//<block>
	void cmd(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		//<command> ::= <name> := <expr>
		if(cur_token.ttype == ID_SY) {
			if(!_ST.searchSymbol_byName(cur_token.tname) && !searchParameter_byName(cur_token.tname,_stypes)) //here symbol MUST be declared in the symbol
																											  //table OR parameters list of a procedure
				cout<<"Semantic ERROR(1 & 2): The name "<<cur_token.tname<<" is not declared!"<<endl;

			Symbol s = _ST.getSymbol_byName(cur_token.tname);

			match(ID_SY);
			match(ASSIGN_SY);
			SymbolType LHS_type = s.stype;
			SymbolType RHS_type = expr(_ST,_stypes);

			if(LHS_type != RHS_type)
				cout<<"Semantic ERROR(3): The l.h.s and r.h.s of an assignment expression MUST be of the same type: "<< s.getSymbolType() <<"!"<<endl;

		}
		//<command> ::= read <name>
		else if(cur_token.ttype == READ_SY) {
			match(READ_SY);
			if(!_ST.searchSymbol_byName(cur_token.tname) && !searchParameter_byName(cur_token.tname,_stypes)) //here symbol MUST be declared in the symbol
																											  //table OR parameters list of a procedure
				cout<<"Semantic ERROR(1 & 2): The name "<<cur_token.tname<<" is not declared!"<<endl;

			Symbol s = _ST.getSymbol_byName(cur_token.tname);
			if (s.stype != integer || s.skind != variable)
				cout<<"Semantic ERROR(6): The name after 'read' "<<cur_token.tname<<" MUST be an integer variable!"<<endl;

			match(ID_SY);
		}
		//<command> ::= write <expr>
		else if(cur_token.ttype == WRITE_SY) {
			match(WRITE_SY);
			expr(_ST);
		}
		//<command> ::= if <expr> then <command-seq> [else <command-seq>] end if
		else if(cur_token.ttype == IF_SY) {
			match(IF_SY);
			expr(_ST);
			match(THEN_SY);
			cmd_seq(_ST);
			if (cur_token.ttype == ELSE_SY) {
				match(ELSE_SY);
				cmd_seq(_ST);
			}

			match(END_SY);
			match(IF_SY);
		}
		//<command> ::= while <expr> do <command-seq> end while 
		else if(cur_token.ttype == WHILE_SY) {
			match(WHILE_SY);
		}
		//<command> ::= call <name> [(<name-list>)] 
		else if(cur_token.ttype == CALL_SY) {
			match(CALL_SY);
			if(!_ST.searchSymbol_byName(cur_token.tname) && !searchParameter_byName(cur_token.tname,_stypes)) //here symbol MUST be declared in the symbol
																											  //table OR parameters list of a procedure
				cout<<"Semantic ERROR(1): The name "<<cur_token.tname<<" is not declared!"<<endl;

			Symbol s = _ST.getSymbol_byName(cur_token.tname);
			if (s.stype != procedure_t && s.skind != procedure_k)
				cout<<"Semantic ERROR(7): The name after 'call' "<<cur_token.tname<<" MUST be a procedure!"<<endl;

			match(ID_SY);

			if(cur_token.ttype == LPARN_SY) {
				match(LPARN_SY);
				vector<string> _parametersNames = name_list();
				match(RPARN_SY);

				if(s.sparameters.size() == _parametersNames.size()) {
					Symbol para;
					for(int i = 0; i < (int)_parametersNames.size(); i++) {
						para = ST.getSymbol_byName(_parametersNames[i]);
						if(para.stype == undefined) {
							cout<<"Semantic ERROR(7): parameter No. "<<i+1<<" is not declared!"<<endl;
						}

						if(para.stype != s.sparameters[i].stype) {
							cout<<"Semantic ERROR(7): parameter No. "<<i+1<<" must be "<<s.getSymbolType()<<endl;
						}
					}
				}
				else
					cout<<"Semantic ERROR(7): There must be "<<s.sparameters.size()<<" parameters!"<<endl;
			}

		}
		//<command> ::= <block>

	}

	//<expr> ::= <expr1> {or <expr1>}
	SymbolType expr(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t, t1, t2;
		t1 = expr1(_ST,_stypes);
		t = t1;
		bool flag = false;
		while (cur_token.ttype == OR_SY) {
			match(OR_SY);
			t = boolean; //Now it must be boolean and t1 MUST be boolean or then t will be undefined
			t2 = expr1(_ST,_stypes);
			if(t1 != boolean || t2 != boolean) {
				flag = true;
				cout<<"Semantic ERROR(3): Expression must be of type boolean!"<<endl;
			}
		}

		if(flag)
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
			t2 = expr2(_ST,_stypes);
			if(t1 != boolean || t2 != boolean) {
				flag = true;
				cout<<"Semantic ERROR(3): Expression must be of type boolean!"<<endl;
			}
		}

		if(flag)
			t = undefined;

		return t; //if one of the expressions between the and's is not boolean then the whole expression has undefined type
	}


	//<expr2> ::= <expr3> | not <expr>
	SymbolType expr2(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t;
		if(cur_token.ttype == ID_SY || cur_token.ttype == NUMBER_SY || cur_token.ttype == LPARN_SY || cur_token.ttype == MINUS_SY)
			t = expr3(_ST,_stypes);
		else if (cur_token.ttype == NOT_SY) {
			match(NOT_SY);
			t = boolean;
			t = expr(_ST,_stypes); //it must return boolean since 'not' appeared
			if(t != boolean)
				cout<<"Semantic ERROR(3): Expression must be of type boolean!"<<endl;

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
		while(cur_token.ttype == LESS_SY || cur_token.ttype == LessEQ_SY || cur_token.ttype == NotEQ_SY
			|| cur_token.ttype == LARGE_SY || cur_token.ttype == LargeEQ_SY || cur_token.ttype == EQ_SY) {
				t = boolean; //because 'relation' appeared
				relation();
				t2 = expr4(_ST, _stypes);
				if(t1 != integer || t2 != integer) {
					flag = true;
					cout<<"Semantic ERROR(3): Expression must be of type integer!"<<endl;
				}
		}

		if(flag)
			t = undefined;

		return t;
	}

	//<expr4> ::= <term> {<weak op> term}
	SymbolType expr4(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t, t1, t2;
		t1 = term(_ST, _stypes);
		t = t1;
		bool flag = false;
		while(cur_token.ttype == PLUS_SY || cur_token.ttype == MINUS_SY) {
			t = integer; //Now it must be integer and t1 MUST be integer becasue 'weak op' appeared
			weak_op();
			t2 = term(_ST, _stypes);
			if(t1 != integer || t2 != integer) {
				flag = true;
				cout<<"Semantic ERROR(3): Expression must be of type integer!"<<endl;
			}
		}
		
		if(flag)
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
			if(t1 != integer || t2 != integer) {
				flag = true;
				cout<<"Semantic ERROR(3): Expression must be of type integer!"<<endl;
			}
		}

		if(flag)
			t = undefined;

		return t;
	}
	//<element> ::= [-] <elem>
	SymbolType element(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t;
		if(cur_token.ttype == MINUS_SY)
			match(MINUS_SY);

		t = elem(_ST, _stypes);
		return t;
	}

	//<elem> ::= <numeral> | <name> | (<expr>)
	SymbolType elem(SymbolTable _ST, vector<Symbol> _stypes = vector<Symbol>(0)) {
		SymbolType t;
		if(cur_token.ttype == NUMBER_SY) {
			t = integer;
			match(NUMBER_SY);
		}
		else if(cur_token.ttype == ID_SY) {
			//Because of thi here we had to pass the symbolTable and list of parameters for all the previous functions
			if(_ST.searchSymbol_byName(cur_token.tname)) { //here symbol MUST be declared in the symbol table OR parameters list of a procedure
				Symbol s = ST.getSymbol_byName(cur_token.tname);
				t = s.stype;
			}
			else if (searchParameter_byName(cur_token.tname,_stypes)) {
				Symbol s = getParameter_byName(cur_token.tname,_stypes);
				t = s.stype;
			}
			else {
				t = undefined;
				cout<<"Semantic ERROR(1 & 2): The name "<<cur_token.tname<<" is not declared!"<<endl;
			}
			match(ID_SY);
		}
		else if(cur_token.ttype == LPARN_SY) {
			match(LPARN_SY);
			t = expr(_ST,_stypes);
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
		switch(cur_token.ttype) {
		case PLUS_SY: match(PLUS_SY); break;
		case MINUS_SY: match(MINUS_SY); break;
		default: syntax_error(cur_token); break;
		}
	}
	//<strong op> ::= * | /
	void strong_op() {
		switch(cur_token.ttype) {
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

	p.parse_VEN();
	system("pause");
}