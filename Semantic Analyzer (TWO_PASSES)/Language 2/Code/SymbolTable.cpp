#include "SymbolTable.h"

//Hash function
int SymbolTable::hFunction(string _sname) {
	int indx = 0;
	for(int i = 0; i < (int)_sname.length(); i++)
		indx += int(_sname[i]);

	indx %= MAX_SIZE;

	return indx;
}

//Constructor
SymbolTable::SymbolTable() {
	hTable = new hNode*[MAX_SIZE];

	for(int i = 0; i < MAX_SIZE; i++) {
		hTable[i] = NULL;
	}
}

//insert asymbol in a symbol table
void SymbolTable::insertSymbol(Symbol _sym) {

	if(searchSymbol_byName(_sym.sname))
		cout<<"Semantic ERROR(1): The name "<<_sym.sname<<" is declared more than once!"<<endl;

	hNode* newNode = new hNode(_sym);
	int indx = hFunction(_sym.sname);

	if(hTable[indx] == NULL)
		hTable[indx] = newNode;
	else {
		hNode* entry = hTable[indx];
		while(entry->next != NULL)
			entry = entry->next;

		entry->next = newNode;
	}
}

//search for a symbol in a symbol table
bool SymbolTable::searchSymbol_byName(string _sname) {
	int indx = hFunction(_sname);
	hNode* entry = hTable[indx];
	while(entry != NULL) {
		if(entry->sym.sname == _sname)
			return true;
		entry = entry->next;
	}

	return false;
}

//retrieve a symbol from a symbol table given its name
Symbol SymbolTable::getSymbol_byName(string _sname) {
	int indx = hFunction(_sname);
	hNode* entry = hTable[indx];
	while(entry != NULL) {
		if(entry->sym.sname == _sname)
			return entry->sym;
		entry = entry->next;
	}

	Symbol sym0; //empty symbol
	return sym0;
}

//union two symbol tables stable1 and stable2 in the first one table1
void SymbolTable::unionSymbolTables(SymbolTable stable2) {
	hNode* entry2;
	for(int i = 0; i < MAX_SIZE; i++) {
		entry2 = stable2.hTable[i];
		while(entry2 != NULL) {
			insertSymbol(entry2->sym);
			entry2 = entry2->next;
		}
	}
}

//is there an intersection between two symbol tables
bool SymbolTable::intersectSymbolTables(SymbolTable stable2) {
	hNode* entry2;
	for(int i = 0; i < MAX_SIZE; i++) {
		entry2 = stable2.hTable[i];
		while(entry2 != NULL) {
			if(searchSymbol_byName(entry2->sym.sname))
				return true;
			entry2 = entry2->next;
		}
	}

	return false;
}


//Display
void SymbolTable::disp () {
	hNode* entry;
	for(int i = 0; i < MAX_SIZE; i++) {
		entry = hTable[i];
		cout << i+1 << ")" << endl;
		while(entry != NULL) {
			entry->sym.dispSymbol();
			entry = entry->next;
		}
	}
} 