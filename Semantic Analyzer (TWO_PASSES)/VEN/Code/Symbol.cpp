#include "Symbol.h"

//Default Constructor
Symbol::Symbol () {
	sname = "";
	stype = undefined;
	skind = empty;
	sparameters = vector<Symbol>(0);
}

//Constructor
Symbol::Symbol (string _name, SymbolType _type, SymbolKind _kind, vector<Symbol> _parameters) {
	sname = _name;
	stype = _type;
	skind = _kind;
	sparameters = _parameters;
	//Distinguish between 0-size sparameters which means the Symbol has no parametrs data member
	//and the one which means the Symbol has 0 parameters BY check if its stype is 'procedure' or something else
}

//get the type (as string) from the symbol type (as enum)
string Symbol::getSymbolType() {
	switch (stype) {
	case procedure_t: return "procedure"; break;
	case integer: return "integer"; break;
	case boolean: return "boolean"; break;
	default: return "undefined"; break;
	}
}

//get the kind (as string) from the symbol kind (as enum)
string Symbol::getSymbolKind() {
	switch (skind) {
	case procedure_k: return "procedure"; break;
	case variable: return "variable"; break;
	default: return "empty"; break;
	}
}

void Symbol::dispSymbol () {
	//display a symbol in one of the following forms
	//<name|type|kind>
	//<name|procedure()|kind>
	//<name|procedure(p1,p2,...)|kind>
	cout<<"<"<<sname<<"|"<<getSymbolType()<<"|";
	if(stype == procedure_t) {
		int num_params = (int)sparameters.size();
		if(num_params == 0) //procedure with at 0 parameters
			cout<<"()|";
		else { //procedure with at least one parameter
			Symbol param; //used to store each parameter
			cout << "(";
			for(int i = 0; i < num_params - 1; i++) {
				param.stype = sparameters[i].stype;
				cout<<param.getSymbolType()<<",";
			}
			param.stype = sparameters[num_params - 1].stype;
			cout<<param.getSymbolType()<<")|";
		}
	}
		
	cout<<getSymbolKind()<<">"<<endl;
}