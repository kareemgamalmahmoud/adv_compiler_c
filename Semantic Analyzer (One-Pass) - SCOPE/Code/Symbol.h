#include <iostream>
#include <string>
#include <vector>
using namespace std;

enum SymbolType {procedure_t, integer, boolean, undefined}; //undefined for the identifiers that does NOT exist
enum SymbolKind {procedure_k, variable, empty}; //empty for the identifiers that does NOT exist

struct Symbol {
	string sname;
	SymbolType stype;
	SymbolKind skind;

	vector<Symbol> sparameters; //when its stype is 'procedure'


	//Default Constructor
	Symbol ();

	//Constructor
	Symbol (string _name, SymbolType _type, SymbolKind _kind, vector<Symbol> _parameters = vector<Symbol>(0));
	
	//get the type (as string) from the symbol type (as enum)
	string getSymbolType();

	//get the kind (as string) from the symbol kind (as enum)
	string getSymbolKind();

	////display a symbol in one of the following forms
	//<name|type|kind>
	//<name|procedure()|kind>
	//<name|procedure(p1,p2,...)|kind>
	void dispSymbol();
};