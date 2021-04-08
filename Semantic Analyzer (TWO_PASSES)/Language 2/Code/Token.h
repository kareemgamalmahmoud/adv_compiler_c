#include <iostream>
#include <string>

using namespace std;

enum TokenType {/*resrved words*/   Program_SY, Is_SY, Begin_SY, End_SY, Const_SY, Read_SY, Write_SY, Int_SY, Bool_SY, Char_SY, True_SY, False_SY, Character_SY, If_SY, Then_SY,
		        /*identefier*/      ID_SY,
	            /*allowed symbols*/ SemiColon_SY, Comma_SY, Number_SY, Equal_SY, Assign_SY,
	            /*end of the file*/ END_SOURCE_SY,
		        /*a word out of the grammar*/ ERROR_SY };


struct Token {
	string tname;
	TokenType ttype;

	//constructor
	Token();

	//get the type (as string) from the token type (as enum)
	string getTokenType();
};