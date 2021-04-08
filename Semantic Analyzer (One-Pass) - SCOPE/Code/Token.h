#include <iostream>
#include <string>

using namespace std;

enum TokenType {/*resrved words*/   BEGIN_SY, END_SY, PROC_SY, INTEGER_SY, BOOL_SY, READ_SY, WRITE_SY, IF_SY, THEN_SY, ELSE_SY, WHILE_SY,
							        DO_SY, CALL_SY, OR_SY, AND_SY, NOT_SY,
		        /*identefier*/      ID_SY,
	            /*allowed symbols*/ COLON_SY, SEMICOL_SY, COMMA_SY, ASSIGN_SY, LPARN_SY, RPARN_SY, LessEQ_SY, LESS_SY, EQ_SY, LARGE_SY, LargeEQ_SY,
								    NotEQ_SY, PLUS_SY, MINUS_SY, MULT_SY, DIV_SY, NUMBER_SY,
	            /*end of the file*/ END_SOURCE_SY,
		        /*a word out of the WREN grammar*/ ERROR_SY };


struct Token {
	string tname;
	TokenType ttype;

	//constructor
	Token();

	//get the type (as string) from the token type (as enum)
	string getTokenType();
};