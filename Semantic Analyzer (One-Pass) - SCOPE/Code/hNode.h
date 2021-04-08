#include "Symbol.h"

struct hNode {
	Symbol sym;
	hNode* next;

	//Constructor
	hNode(Symbol _sym);
};
