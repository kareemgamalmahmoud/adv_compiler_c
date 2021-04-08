#include "SymbolTable.h"

enum NodeKind {Program_N, Block_N, Dec_N, Com_N, Expr_N, ID_N, DecS_N, ComS_N, Type_N, undefined_N};
enum DecKind {var_D, const_D, undefined_D};
enum ComKind {ifThen_cmd, assign_cmd, write_cmd, read_cmd, undefined_cmd};
enum ExprKind {num_expr, true_expr, false_expr, character_expr, undefined_expr};
enum TypeKind {program_type, int_type, bool_type, char_type, undefined_type};

struct AST_Node {
	NodeKind nodeKind;
	DecKind decKind;
	ComKind comKind;
	ExprKind exprKind;
	TypeKind typeKind;

	string name;

	vector<AST_Node*> child;


	AST_Node (int num_of_children);

	void dispNode ();

};