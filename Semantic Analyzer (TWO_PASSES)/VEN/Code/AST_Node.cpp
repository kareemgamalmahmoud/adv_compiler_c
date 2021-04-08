#include "AST_Node.h"

string dispNodeKind(NodeKind nk) {
	switch(nk) {
	case Program_N: return "program"; break;
	case Block_N: return "block"; break;
	case Dec_N: return "declaration"; break;
	case DecS_N: return "DeclarationS"; break;
	case Com_N: return "command"; break;
	case ComS_N: return "CommandS"; break;
	case Para_N: return "Parameter"; break;
	case Id_N: return "identifier"; break;
	case IdS_N: return "identifierS"; break;
	case Expr_N: return "expression"; break;
	case Element_N: return "element"; break;
	case Negative_N: return "negative"; break;
	case Type_N: return "type"; break;
	case undefined_N:
	default: return "_"; break;
	}
}

string dispDecKind(DecKind dk) {
	switch(dk) {
	case var_D: return "variable"; break;
	case proc_D: return "procedure"; break;
	case undefined_D:
	default: return "_";  break;
	}
}

string dispComKind(ComKind ck) {
	switch(ck) {
	case assign_cmd: return "assignment"; break;
	case read_cmd: return "read"; break;
	case write_cmd: return "write"; break;
	case ifThenElse_cmd: return "if then else"; break;
	case whileDo_cmd: return "while do"; break;
	case call_cmd: return "call"; break;
	case undefined_cmd: 
	default: return "_"; break;
	}
}

string dispExprKind(ExprKind ek) {
	switch (ek)
	{
	case Or_expr: return "or"; break;
	case And_expr: return "and"; break;
	case Not_expr: return "not"; break;
	case Relation_expr: return "relation"; break;
	case WeakOp_expr: return "weak op"; break;
	case StrongOp_expr: return "strong op"; break;
	case undefined_expr:
	default: return "_"; break;
	}
}

string dispElmentKind(ElementKind ek) {
	switch(ek) {
	case Number_elem: return "number"; break;
	case Id_elem: return "id"; break;
	case expr_elem: return "expr"; break;
	case negative_elem: return "negative"; break;
	case undefined_elem:
	default: return "_"; break;
	}
}

string dispTypeKind(TypeKind tk) {
	switch(tk) {
	case integer_type: return "integer"; break;
	case boolean_type: return "boolean"; break;
	case undefined_type:
	default: return "_"; break;
	}
}

AST_Node::AST_Node (int num_of_children) {
	nodeKind = undefined_N;
	decKind = undefined_D;
	comKind = undefined_cmd;
	exprKind = undefined_expr;
	elementKind = undefined_elem;
	typeKind = undefined_type;

	name = "";

	for (int i = 0; i < num_of_children; i++) {
		child.push_back(NULL);
	}
}


void AST_Node::dispNode () {
	string N ,n, d, c, e1,e2, t;
	N = name == ""? "_":name;
	n = dispNodeKind(nodeKind);
	d = dispDecKind(decKind);
	c = dispComKind(comKind);
	e1 = dispExprKind(exprKind);
	e2 = dispElmentKind(elementKind);
	t = dispTypeKind(typeKind);
	cout << "<" << N << "|" << n << "|" << d << "|" << c << "|" << e1 << "|" << e2 << "|" << t << ">" << endl;
}