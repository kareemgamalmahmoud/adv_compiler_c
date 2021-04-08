#include "SymbolTable.h"

enum NodeKind {Program_N, Block_N, Dec_N, DecS_N, Com_N, ComS_N, Para_N, Id_N, IdS_N,
				Expr_N, Element_N, Negative_N, Type_N, undefined_N};
enum DecKind {var_D, proc_D, undefined_D};
enum ComKind {assign_cmd, read_cmd, write_cmd, ifThenElse_cmd, whileDo_cmd, call_cmd, block_cmd, undefined_cmd};
enum ExprKind {Or_expr, And_expr, Not_expr, Relation_expr, WeakOp_expr, StrongOp_expr, undefined_expr};
enum ElementKind {Number_elem, Id_elem, expr_elem, negative_elem, undefined_elem};
enum TypeKind {integer_type, boolean_type, undefined_type};

struct AST_Node {
	NodeKind nodeKind;
	DecKind decKind;
	ComKind comKind;
	ExprKind exprKind;
	ElementKind elementKind;
	TypeKind typeKind;

	string name;

	vector<AST_Node*> child;


	AST_Node (int num_of_children);

	void dispNode ();

};