#include "AST_Node.h"

string dispNodeKind(NodeKind nk) {
	switch(nk) {
	case Program_N: return "program"; break;
	case Block_N: return "block"; break;
	case Dec_N: return "declaration"; break;
	case Com_N: return "command"; break;
	case Expr_N: return "expression"; break;
	case ID_N: return "identifier"; break;
	case DecS_N: return "DeclarationS"; break;
	case ComS_N: return "CommandS"; break;
	case Type_N: return "type"; break;
	case undefined_N:
	default: return "undefined node"; break;
	}
}

string dispDecKind(DecKind dk) {
	switch(dk) {
	case var_D: return "variable"; break;
	case const_D: return "constant"; break;
	case undefined_D:
	default: return "undefined declaration";  break;
	}
}

string dispComKind(ComKind ck) {
	switch(ck) {
	case ifThen_cmd: return "if then"; break;
	case assign_cmd: return "assignment"; break;
	case write_cmd: return "write"; break;
	case read_cmd: return "read"; break;
	case undefined_cmd:
	default: return "undefined command"; break;
	}
}

string dispExprKind(ExprKind ek) {
	switch (ek) {
		case num_expr: return "number"; break;
		case true_expr: return "true"; break;
		case false_expr: return "false"; break;
		case character_expr: return "character"; break;
		case undefined_expr:
		default:  return "undefined expression"; break;
	}
}

string dispTypeKind(TypeKind tk) {
	switch(tk) {
	case program_type: return "program"; break;
	case int_type: return "int"; break;
	case bool_type: return "bool"; break;
	case char_type: return "char"; break;
	case Undefined_type:
	default: return "undefined type"; break;
	}
}


AST_Node::AST_Node (int num_of_children) {
	nodeKind = undefined_N;
	decKind = undefined_D;
	comKind = undefined_cmd;
	exprKind = undefined_expr;
	typeKind = undefined_type;

	name = "";

	for (int i = 0; i < num_of_children; i++) {
		child.push_back(NULL);
	}
}


void AST_Node::dispNode () {

	string N ,n, d, c, e, t;
	N = name == ""? "_":name;
	n = dispNodeKind(nodeKind) == "undefined node"? "_":dispNodeKind(nodeKind);
	d = dispDecKind(decKind) == "undefined declaration"? "_":dispDecKind(decKind);
	c = dispComKind(comKind) == "undefined command"? "_":dispComKind(comKind);
	e = dispExprKind(exprKind) == "undefined expression"? "_":dispExprKind(exprKind);
	t = dispTypeKind(typeKind) == "undefined type"? "_":dispTypeKind(typeKind);
	cout << "<" << N << "|" << n << "|" << d << "|" << c << "|" << e << "|" << t << ">" << endl;
}