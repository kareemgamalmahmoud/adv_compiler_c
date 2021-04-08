#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum SymbolType {Program, Int, Bool, Char, Undefined_type}; //undefined for the identifiers that does NOT exist
enum SymbolKind {Variable, Constant, Undefined_kind}; //empty for the identifiers that does NOT exist

struct Symbol {
	string sname;
	SymbolType stype;
	SymbolKind skind;

	//Default Constructor
	Symbol ();

	//Constructor
	Symbol (string _name, SymbolType _type, SymbolKind _kind);
	
	//get the type (as string) from the symbol type (as enum)
	string dispSymbolType();

	//get the kind (as string) from the symbol kind (as enum)
	string dispSymbolKind();

	////display a symbol in one of the following forms
	//<name|type|kind>
	void dispSymbol();
};