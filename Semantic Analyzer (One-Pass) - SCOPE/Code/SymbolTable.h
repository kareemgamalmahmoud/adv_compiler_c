#include "hNode.h"

const int MAX_SIZE = 11;

class SymbolTable {
private:
	hNode** hTable; //array of pointers to linked lists

	//Hash function
	int hFunction(string _sname);
public:

	//Constructor
	SymbolTable();

	//insert a symbol in a symbol table
	void insertSymbol(Symbol _sym);
	
	//insert a list of symbol with the same type in a symbol table
	//(e.g. integer x, y, z will be inserted as (x,integer,variable), (y,integer,variable), (z,integer,variable))
	void insertSymbolList(SymbolType _stype, vector<string> _snames);
	
	//search for a symbol in a symbol table
	bool searchSymbol_byName(string _sname);
	
	//retrieve a symbol from a symbol table given its name
	Symbol getSymbol_byName(string _sname);
	
	//union two symbol tables stable1 and stable2 in the first one table1
	void unionSymbolTables(SymbolTable stable2);
	
	//is there an intersection between two symbol tables
	bool intersectSymbolTables(SymbolTable stable2);

	//Display
	void disp(); 

	//empty a symbol table
	void makeEmpty();
};