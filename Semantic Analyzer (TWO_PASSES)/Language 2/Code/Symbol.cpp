#include "Symbol.h"

//Default Constructor
Symbol::Symbol () {
	sname = "";
	stype = Undefined_type;
	skind = Undefined_kind;
}

//Constructor
Symbol::Symbol (string _name, SymbolType _type, SymbolKind _kind) {
	sname = _name;
	stype = _type;
	skind = _kind;
}

//get the type (as string) from the symbol type (as enum)
string Symbol::dispSymbolType() {

	switch (stype) {
	case Program: return "program"; break;
	case Int: return "integer"; break;
	case Bool: return "boolean"; break;
	case Char: return "char"; break;
	default: return "undefined"; break;
	}
}

//get the kind (as string) from the symbol kind (as enum)
string Symbol::dispSymbolKind() {
	switch (skind) {
	case Variable: return "variable"; break;
	case Constant: return "constant"; break;
	default: return "undefined"; break;
	}
}

void Symbol::dispSymbol () {
	//display a symbol in one of the following forms
	//<name|type|kind>
	cout<<"<"<<sname<<"|"<<dispSymbolType()<<"|"<<dispSymbolKind()<<">"<<endl;
}