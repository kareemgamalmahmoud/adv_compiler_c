#include "Token.h"

//constructor
Token::Token() {
	tname = "";
}

//get the type (as string) from the token type (as enum)
string Token::getTokenType() {
	switch(ttype) {
	case BEGIN_SY:      return "begin"; break;
	case END_SY:        return "end"; break;
	case PROC_SY:       return "proc"; break;
	case INTEGER_SY:    return "integer"; break;
	case BOOL_SY:       return "boolean"; break;
	case READ_SY:       return "read"; break;
	case WRITE_SY:      return "write"; break;
	case IF_SY:         return "if"; break;
	case THEN_SY:       return "then"; break;
	case ELSE_SY:       return "else"; break;
	case WHILE_SY:      return "while"; break;
	case DO_SY:         return "do"; break;
	case CALL_SY:       return "call"; break;
	case OR_SY:         return "or"; break;
	case AND_SY:        return "and"; break;
	case NOT_SY:        return "not"; break;
	case COLON_SY:      return ":"; break;
	case SEMICOL_SY:    return ";"; break;
	case COMMA_SY:      return ","; break;
	case ASSIGN_SY:     return ":="; break;
	case LPARN_SY:      return "("; break;
	case RPARN_SY:      return ")"; break;
	case LessEQ_SY:     return "<="; break;
	case LESS_SY:       return "<"; break;
	case EQ_SY:         return "="; break;
	case LARGE_SY:      return ">"; break;
	case LargeEQ_SY:    return ">="; break;
	case NotEQ_SY:      return "<>"; break;
	case PLUS_SY:       return "+"; break;
	case MINUS_SY:      return "-"; break;
	case MULT_SY:       return "*"; break;
	case DIV_SY:        return "/"; break;
	case ID_SY:         return tname + " (Identifier)"; break;
	case END_SOURCE_SY: return "End of file"; break;
	default:            return "Undefined Token!"; break;
	}
}