#include "Token.h"

//constructor
Token::Token() {
	tname = "";
}

//get the type (as string) from the token type (as enum)
string Token::getTokenType() {
	switch(ttype) {
	case Program_SY:    return "program"; break;
	case Is_SY:			return "is"; break;
	case Begin_SY:      return "begin"; break;
	case End_SY:        return "end"; break;
	case Int_SY:		return "int"; break;
	case Bool_SY:       return "bool"; break;
	case Read_SY:       return "read"; break;
	case Write_SY:      return "write"; break;
	case If_SY:         return "if"; break;
	case Const_SY:      return "const"; break;
	case Char_SY:       return "char"; break;
	case Character_SY:  return "character"; break;
	case True_SY:       return "true"; break;
	case False_SY:      return "false"; break;
	case Then_SY:       return "then"; break;
	case Number_SY:     return "Number"; break;
	case SemiColon_SY:  return ";"; break;
	case Comma_SY:      return ","; break;
	case Assign_SY:     return ":="; break;
	case Equal_SY:      return "="; break;
	case ID_SY:         return tname + " (Identifier)"; break;
	case END_SOURCE_SY: return "End of File"; break;
	default:            return "Undefined Token"; break;
	}
}