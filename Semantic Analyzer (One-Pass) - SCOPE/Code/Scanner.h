#include <fstream>
#include "Token.h"

class Scanner {
private:
	ifstream f;
	
	//check whether a token is a reserved word in the grammar
	Token checkReserved(string tname);
public:
	//Constructor
	Scanner(string fileName);
	
	//get a token from program file 
	Token getToken();
	
	//get all the tokens from program file token by token
	void displayTokens();
};