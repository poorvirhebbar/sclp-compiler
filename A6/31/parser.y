%{
	#include "program.hh"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	extern "C" void yyerror(const char *s);
	extern int yylex(void);
	extern int yylineno;
	Symbol_Table* global_sym_table = new Symbol_Table();
	Symbol_Table* local_sym_table = new Symbol_Table();
	Symbol_Table* formal_sym_table = new Symbol_Table();

	/*current procedure */
	bool in_func_defn = false; //set when in function definition
	string current_procedure_name = "";
	Data_Type current_procedure_type = int_data_type;
%}

%union {
	int integer_value;
	double double_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Sequence_Ast * seq_ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	Procedure * procedure;
	list<Symbol_Table_Entry *> * symbol_entry_list;
	Data_Type data_type;
	Call_Ast* call_ast;
}

%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token <string_value> NAME
%token BBNUM  RETURN  INTEGER  FLOAT  ASSIGN  VOID  UMINUS
%token WHILE IF DO ELSE
%token PRINT
%token EQUAL LESS_THAN GREATER_THAN LESS_THAN_EQUAL GREATER_THAN_EQUAL NOT_EQUAL
%token AND OR NOT

%type <symbol_entry_list> variable_list	variable_declaration variable_declaration_list optional_variable_declaration_list
%type <procedure> procedure_definition procedure_declaration
%type <symbol_table> declaration_param_list formal_param_list optional_formal_param_list
%type <ast>	assignment_statement arith_expression
%type <ast> log_expression rel_expression iteration_statement statement selection_statement
%type <ast> sequence_statement
/*print*/
%type <ast> print_statement
%type <ast_list> statement_list actual_param_list optional_actual_param_list
%type <call_ast> call_stmt 
%type <ast> return_stmt
%type <data_type> type

%right ASSIGN
%right '?' ':'
%left OR
%left AND
%left EQUAL NOT_EQUAL
%left LESS_THAN GREATER_THAN LESS_THAN_EQUAL GREATER_THAN_EQUAL
%left '+' '-'
%left '*' '/'
%right UMINUS
%right NOT
%nonassoc NOT_ELSE
%nonassoc ELSE

%start program

%%

program					:	dec_def_list
							{
								(*global_sym_table).set_table_scope(global);
								program_object.set_global_table(*global_sym_table);
							}

dec_def_list			:	procedure_definition_list
							{
								/* check for parameterless void main function */
								if(!(program_object.is_procedure_exists("main") && program_object.get_procedure_prototype("main")->get_return_type() == void_data_type)) {
									cerr << "cs316: Error: Line "<<yylineno<<":  Main should be defined and its type should be void\n";
									exit(1);
								}
							}
							|	variable_declaration dec_def_list
							{
								/*this is done in variable declarations */

								// for(list<Symbol_Table_Entry*>::iterator it = (*$1).begin(); it != (*$1).end(); it++) {
								// 	(*it)->set_symbol_scope(global);
								// 	Symbol_Table x;
								// 	x.push_symbol(*it);
								// 	x.set_table_scope(global);
								// 	(*global_sym_table).append_list(x, (*it)->get_lineno());
								// }
							}
							|	procedure_declaration dec_def_list
							{
								/*no actions */
							}
							;

procedure_declaration	:	type NAME '(' declaration_param_list ')' ';'
							{	
								if(*$2 != "main")
									*$2 = *$2 + "_";
								$$ = new Procedure($1, *$2, yylineno);
								$$->set_formal_param_list(*$4);
								program_object.set_proc_to_map(*$2, $$);
							}
							|	type NAME '(' optional_formal_param_list ')' ';'
							{
								if(*$2 != "main")
									*$2 = *$2 + "_";
								$$ = new Procedure($1, *$2, yylineno);
								$$->set_formal_param_list(*$4);
								program_object.set_proc_to_map(*$2, $$);
							}
							|	VOID NAME '(' declaration_param_list ')' ';'
							{
								if(*$2 != "main")
									*$2 = *$2 + "_";
								$$ = new Procedure(void_data_type, *$2, yylineno);
								$$->set_formal_param_list(*$4);
								program_object.set_proc_to_map(*$2, $$);
							}
							|	VOID NAME '(' optional_formal_param_list ')' ';'
							{
								if(*$2 != "main")
									*$2 = *$2 + "_";
								$$ = new Procedure(void_data_type, *$2, yylineno);
								$$->set_formal_param_list(*$4);
								program_object.set_proc_to_map(*$2, $$);
							}	
							;

type					:	INTEGER
							{
								$$ = int_data_type;
							}
							|	FLOAT
							{
								$$ = double_data_type;
							}
							;

declaration_param_list		:	declaration_param_list ',' type
								{
									$$ = $1;
									string d = "dummy";
									Symbol_Table_Entry *x = new Symbol_Table_Entry(d, $3, yylineno);
									$$->push_symbol(x);
								}
								|	type
								{
									$$ = new Symbol_Table();
									string d = "dummy";
									Symbol_Table_Entry *x = new Symbol_Table_Entry(d, $1, yylineno);
									$$->push_symbol(x);
								}
								;


procedure_definition_list	:	procedure_definition
								{
									/*no actions */
								}
								| procedure_definition_list procedure_definition 
								{
									/*no actions */
								}
								;

procedure_definition		:	type NAME
								'(' optional_formal_param_list ')'
								{
									if(*$2 != "main")
										*$2 = *$2 + "_";
									current_procedure_name = *$2;
									current_procedure_type = $1;
									in_func_defn = true;
									formal_sym_table = $4;
									/*recursion */
									Procedure *proc = new Procedure($1, *$2, yylineno);
									$4->set_table_scope(formal);
									proc->set_formal_param_list(*$4);
									program_object.set_procedure(proc, yylineno);
									// (*local_sym_table).append_list(*$4, yylineno);
								} 
								'{' 
									local_variable_declaration_list statement_list
								'}' 
								{
									$$ = new Procedure($1, *$2, yylineno);
									$4->set_table_scope(formal);
									$$->set_formal_param_list(*$4);

									if(program_object.is_procedure_exists(*$2))
									{
										Procedure *declared_proc = program_object.get_procedure_prototype(*$2);

										if(declared_proc->is_proc_defined()) {
											cerr << "cs316: Error: Line "<<yylineno<<": procedure already defined\n";
											exit(1);
										}

										if($1 != declared_proc->get_return_type()) {
											cerr << "cs316: Error: Line "<<yylineno<<": return type in declaration and definition not matching\n";
											exit(1);
										}

										list<Symbol_Table_Entry *> decl_table = declared_proc->get_formal_param_list().get_table();
										list<Symbol_Table_Entry *> def_table = $4->get_table();

										list<Symbol_Table_Entry *>::iterator it1 = decl_table.begin();
										list<Symbol_Table_Entry *>::iterator it2 = def_table.begin();
										int i=0;
										while(it1 != decl_table.end() && it2 != def_table.end()) {
											if((*it1)->get_data_type() != (*it2)->get_data_type())
											{
												cerr << "cs316: Error: Line "<<yylineno<<": Declared and defined parameter types don't match\n";
												exit(1);
											}
											it1++;
											it2++;
										}
										if(decl_table.size() != def_table.size()) {
											declared_proc->get_formal_param_list().print(cerr);
											cerr << "cs316: Error: Line "<<yylineno<<": Declared and defined parameter counts don't match\n";
											exit(1);
										}

										/*paramters match */
									}
									else {
										program_object.set_proc_to_map(*$2, $$);
									}


									(*local_sym_table).set_table_scope(local);
									(*$$).set_local_list((*local_sym_table));
									(*$$).set_ast_list(*($9));
									// $$->set_proc_is_defined();
									local_sym_table = new Symbol_Table;
									program_object.set_procedure($$, yylineno);
								}

								|	VOID NAME
								'(' optional_formal_param_list ')'
								{
									if(*$2 != "main")
										*$2 = *$2 + "_";
									current_procedure_name = *$2;
									current_procedure_type = void_data_type;
									in_func_defn = true;
									formal_sym_table = $4;
									/*recursion */
									Procedure *proc = new Procedure(void_data_type, *$2, yylineno);
									$4->set_table_scope(formal);
									proc->set_formal_param_list(*$4);
									program_object.set_procedure(proc, yylineno);
									// (*local_sym_table).append_list(*$4, yylineno);
								} 
								'{' 
									local_variable_declaration_list statement_list
								'}' 
								{	
									$$ = new Procedure(void_data_type, *$2, yylineno);
									$4->set_table_scope(formal);
									$$->set_formal_param_list(*$4);

									if(program_object.is_procedure_exists(*$2))
									{
										Procedure *declared_proc = program_object.get_procedure_prototype(*$2);

										if(declared_proc->is_proc_defined()) {
											cerr << "cs316: Error: Line "<<yylineno<<": procedure already defined\n";
											exit(1);
										}

										if(void_data_type != declared_proc->get_return_type()) {
											cerr << "cs316: Error: Line "<<yylineno<<": return type in declaration and definition not matching\n";
											exit(1);
										}

										list<Symbol_Table_Entry *> decl_table = declared_proc->get_formal_param_list().get_table();
										list<Symbol_Table_Entry *> def_table = $4->get_table();

										list<Symbol_Table_Entry *>::iterator it1 = decl_table.begin();
										list<Symbol_Table_Entry *>::iterator it2 = def_table.begin();
										int i=0;
										while(it1 != decl_table.end() && it2 != def_table.end()) {
											if((*it1)->get_data_type() != (*it2)->get_data_type())
											{
												cerr << "cs316: Error: Line "<<yylineno<<": Declared and defined parameter types don't match\n";
												exit(1);
											}
											it1++;
											it2++;
										}
										if(decl_table.size() != def_table.size()) {
											cerr << "cs316: Error: Line "<<yylineno<<": Declared and defined parameter counts don't match\n";
											exit(1);
										}
									}
									else {
										program_object.set_proc_to_map(*$2, $$);
									}


									(*local_sym_table).set_table_scope(local);
									(*$$).set_local_list((*local_sym_table));
									(*$$).set_ast_list(*($9));
									local_sym_table = new Symbol_Table;
									program_object.set_procedure($$, yylineno);
								}

								;

optional_formal_param_list	:	/*empty */
								{
									$$ = new Symbol_Table();
								}
								|	formal_param_list
								{
									$$ = $1;
								}
								;

formal_param_list			:	formal_param_list ',' type NAME 
								{
									*$4 = *$4 + "_";
									Symbol_Table_Entry *ste = new Symbol_Table_Entry(*$4, $3, yylineno);
									$$->push_symbol(ste);
								}
								|	type NAME
								{
									*$2 = *$2 + "_";
									$$ = new Symbol_Table();
									Symbol_Table_Entry *ste = new Symbol_Table_Entry(*$2, $1, yylineno);
									$$->push_symbol(ste);
								}
							;

local_variable_declaration_list		: 	optional_variable_declaration_list
										{
											for(list<Symbol_Table_Entry*>::iterator it = (*$1).begin(); it != (*$1).end(); it++) {
												(*it)->set_symbol_scope(local);
												Symbol_Table x;
												x.push_symbol(*it);
												x.set_table_scope(local);
												(*local_sym_table).append_list(x, (*it)->get_lineno());
											}
										}
										;
								
optional_variable_declaration_list	:	/* empty */ 
										{
											$$ = new list<Symbol_Table_Entry *>();
										}
										|	variable_declaration_list
										{
											$$ = $1;
										}
										;

variable_declaration_list			:	variable_declaration
										{
											$$ = $1;
										}
										|	variable_declaration_list variable_declaration
										{
											for(list<Symbol_Table_Entry*>::iterator it = (*$2).begin(); it != (*$2).end(); it++) {
												(*($1)).push_back(*it);
											}
											$$ = $1;
										}
										;

variable_declaration					:	type variable_list ';'
										{
											for(list<Symbol_Table_Entry*>::iterator it = (*$2).begin(); it != (*$2).end(); it++) {
												(*it)->set_data_type($1);
											}
											$$ = $2;

											if(! in_func_defn) {
												//global declarations, handle them here
												for(list<Symbol_Table_Entry*>::iterator it = (*$$).begin(); it != (*$$).end(); it++) {
													(*it)->set_symbol_scope(global);
													Symbol_Table x;
													x.push_symbol(*it);
													x.set_table_scope(global);
													(*global_sym_table).append_list(x, (*it)->get_lineno());
												}
											}
											//local declarations will be handled in function definition
										}
                                        ;

variable_list                       :	NAME	
										{
											$$ = new list<Symbol_Table_Entry *>();
											*$1 = *$1 + "_";
											Symbol_Table_Entry* a = new Symbol_Table_Entry(*$1, int_data_type,yylineno);
											(*$$).push_back(a);
										}
										| variable_list ',' NAME
										{
											*$3 = *$3 + "_";
											Symbol_Table_Entry* b = new Symbol_Table_Entry(*$3, int_data_type,yylineno);
											(*$1).push_back(b); /* TODO: Clarify from the TA */
											$$ = $1;
										}
										;


statement_list	        :	/* empty */ 
							{
								$$ = new list<Ast *>();
							}
							|	statement_list sequence_statement
							{
								(*$1).push_back($2);
								$$ = $1;
							}
							;

statement 				:	assignment_statement
							{
								$$ = $1;
							}
							|	iteration_statement  /*while, do-while*/
							{
								$$ = $1;
							}
							|	selection_statement  /*if then else*/
							{
								$$ = $1;
							}
							/*print*/
							|	print_statement
							{
								$$ = $1;
							}
							|	call_stmt
							{
								$$ = $1;
							}
							|	return_stmt
							{
								$$ = $1;
							}
							;


sequence_statement		:	'{' statement_list '}'
							{
								if((*$2).empty()) 	/*body empty */
								{
									yyerror("cs316: Error: Block of statements cannot be empty");
									exit(1);
								}
			
								Sequence_Ast* temp = new Sequence_Ast(yylineno);
								for(list<Ast*>::iterator it = (*$2).begin(); it != (*$2).end(); it++) {
									temp->ast_push_back(*it);
								}
								$$ = temp;
							}
							|
							statement
							{
								$$ = $1;
							}
							;

iteration_statement		:	WHILE '(' log_expression ')' sequence_statement
							{
								$$ = new Iteration_Statement_Ast($3, $5, yylineno, 0); 	/*last arg is bool do_form */
							}
							|	DO sequence_statement WHILE '(' log_expression ')' ';'
							{
								$$ = new Iteration_Statement_Ast($5, $2, yylineno, 1); 	/*last arg is bool do_form */	
							}
							;

selection_statement		:	IF '(' log_expression ')' sequence_statement %prec NOT_ELSE
							{
								$$ = new Selection_Statement_Ast($3, $5, NULL, yylineno);
							}
							|	IF '(' log_expression ')' sequence_statement ELSE sequence_statement
							{
								$$ = new Selection_Statement_Ast($3, $5, $7, yylineno);
							}
							;

log_expression			:	log_expression AND log_expression
							{
								$$ = new Logical_Expr_Ast($1, _logical_and, $3, yylineno);
								(*$$).check_ast();
							}
							|	log_expression OR log_expression
							{
								$$ = new Logical_Expr_Ast($1, _logical_or, $3, yylineno);
								(*$$).check_ast();
							}
							|	NOT log_expression
							{
								$$ = new Logical_Expr_Ast(NULL, _logical_not, $2, yylineno);
								(*$$).check_ast();
							}
							|	rel_expression
							{
								$$ = $1;
								(*$$).check_ast();
							}
							|	'(' log_expression ')'
							{
								$$ = $2;
							}
							;

rel_expression			:	arith_expression EQUAL arith_expression
							{
								$$ = new Relational_Expr_Ast($1, equalto, $3, yylineno);
								(*$$).check_ast();
							}
							|	arith_expression NOT_EQUAL arith_expression
							{
								$$ = new Relational_Expr_Ast($1, not_equalto, $3, yylineno);
								(*$$).check_ast();
							}
							|	arith_expression LESS_THAN arith_expression
							{
								$$ = new Relational_Expr_Ast($1, less_than, $3, yylineno);
								(*$$).check_ast();
							}
							|	arith_expression LESS_THAN_EQUAL arith_expression
							{
								$$ = new Relational_Expr_Ast($1, less_equalto, $3, yylineno);
								(*$$).check_ast();
							}	
							|	arith_expression GREATER_THAN arith_expression
							{
								$$ = new Relational_Expr_Ast($1, greater_than, $3, yylineno);
								(*$$).check_ast();
							}
							|	arith_expression GREATER_THAN_EQUAL arith_expression
							{
								$$ = new Relational_Expr_Ast($1, greater_equalto, $3, yylineno);
								(*$$).check_ast();
							}
							;



assignment_statement	:	NAME ASSIGN arith_expression ';'
				
							{
								*$1 = *$1 + "_";
								if(!(*local_sym_table).is_empty() && (*local_sym_table).variable_in_symbol_list_check(*$1)){ 
									Ast* lhs1 = new Name_Ast(*$1, (*local_sym_table).get_symbol_table_entry(*$1), yylineno);
									$$ = new Assignment_Ast(lhs1,$3,yylineno);
									(*$$).check_ast();
								}
								/*formal list check */
								else if(!(*formal_sym_table).is_empty() && (*formal_sym_table).variable_in_symbol_list_check(*$1)){ 
									// formal_sym_table->print(cerr);
									Ast* lhs1 = new Name_Ast(*$1, (*formal_sym_table).get_symbol_table_entry(*$1), yylineno);
									$$ = new Assignment_Ast(lhs1,$3,yylineno);
									(*$$).check_ast();
								}
								else if(!(*global_sym_table).is_empty() && (*global_sym_table).variable_in_symbol_list_check(*$1)){
									Ast* lhs2 = new Name_Ast(*$1, (*global_sym_table).get_symbol_table_entry(*$1), yylineno);
									$$ = new Assignment_Ast(lhs2,$3,yylineno);
									(*$$).check_ast();
								}
								else{
									cerr<<"cs316: Error: "<<yylineno<<": "<<"Variable has not been declared/defined\n";
									exit(1);
								}
							}
							|	NAME ASSIGN call_stmt
							{
								*$1 = *$1 + "_";
								if(!(*local_sym_table).is_empty() && (*local_sym_table).variable_in_symbol_list_check(*$1)){ 
									Ast* lhs1 = new Name_Ast(*$1, (*local_sym_table).get_symbol_table_entry(*$1), yylineno);
									$$ = new Assignment_Ast(lhs1,$3,yylineno);
									(*$$).check_ast();
								}
								/*formal list check */
								else if(!(*formal_sym_table).is_empty() && (*formal_sym_table).variable_in_symbol_list_check(*$1)){ 
									// formal_sym_table->print(cerr);
									Ast* lhs1 = new Name_Ast(*$1, (*formal_sym_table).get_symbol_table_entry(*$1), yylineno);
									$$ = new Assignment_Ast(lhs1,$3,yylineno);
									(*$$).check_ast();
								}
								else if(!(*global_sym_table).is_empty() && (*global_sym_table).variable_in_symbol_list_check(*$1)){
									Ast* lhs2 = new Name_Ast(*$1, (*global_sym_table).get_symbol_table_entry(*$1), yylineno);
									$$ = new Assignment_Ast(lhs2,$3,yylineno);
									(*$$).check_ast();
								}
								else{
									cerr<<"cs316: Error: "<<yylineno<<": "<<"Variable has not been declared/defined\n";
									exit(1);
								}
							}
							;

print_statement			:	PRINT NAME ';'
							{
								*$2 = *$2 + "_";
								if(!(*local_sym_table).is_empty() && (*local_sym_table).variable_in_symbol_list_check(*$2)){ 
									Ast* var = new Name_Ast(*$2, (*local_sym_table).get_symbol_table_entry(*$2), yylineno);
									$$ = new Print_Ast(var, yylineno);
								}
								else if(!(*global_sym_table).is_empty() && (*global_sym_table).variable_in_symbol_list_check(*$2)){
									Ast* var = new Name_Ast(*$2, (*global_sym_table).get_symbol_table_entry(*$2), yylineno);
									$$ = new Print_Ast(var, yylineno);
								}
								else{
									yyerror("cs316: Error: Variable has not been declared/defined");
									exit(1);
								}
							}

actual_param_list		:	actual_param_list ',' arith_expression
							{
								$$->push_back($3);
							}
							|	arith_expression
							{
								$$ = new list<Ast*>();
								$$->push_back($1);
							}

optional_actual_param_list	:	actual_param_list
								{
									$$ = $1;
								}
								|	/*empty */
								{
									$$ = new list<Ast*>();
								}
								;

call_stmt				:	NAME '(' optional_actual_param_list ')' ';'
							{
								if(*$1 != "main")
										*$1 = *$1 + "_";
								/* What does this check exactly ? */
								if(! program_object.is_procedure_exists(*$1)) {
									cerr << "cs316: Error: Line "<<yylineno<<": Unknown procedure.\n";
									exit(1);
								}
								$$ = new Call_Ast(*$1, yylineno);
								$$->set_actual_param_list(*$3);
								Procedure *proc = program_object.get_procedure_prototype(*$1);
								$$->set_data_type(proc->get_return_type());
								$$->check_actual_formal_param(proc->get_formal_param_list());
								// /*debug */
								// proc->get_formal_param_list().print(cerr);
								// /*debug */
							}
							;

return_stmt				:	RETURN arith_expression ';'
							{
								if(!(current_procedure_type == (*$2).get_data_type())) {
									cerr << "cs316: Error: Line "<<yylineno<<":  Return datatype didn't match with the definition datatype\n";
									exit(1);
								}
								$$ = new Return_Ast($2, current_procedure_name, yylineno);
								$$->set_data_type(current_procedure_type);
								// $$->print(cerr);
							}
							|	RETURN ';'
							{
								if(!(current_procedure_type == void_data_type)) {
									cerr << "cs316: Error: Line "<<yylineno<<":  Return datatype didn't match with the definition datatype\n";
									exit(1);
								}
								$$ = new Return_Ast(NULL, current_procedure_name, yylineno);
								$$->set_data_type(current_procedure_type);
							}

arith_expression		: 	INTEGER_NUMBER	
							{
								$$ = new Number_Ast<int>($1, int_data_type, yylineno);
							}
							| DOUBLE_NUMBER 
							{
								$$ = new Number_Ast<double>($1, double_data_type, yylineno);
							}
							| NAME
							{
								*$1 = *$1 + "_";
								if(!(*local_sym_table).is_empty() && (*local_sym_table).variable_in_symbol_list_check(*($1))){
									$$ = new Name_Ast(*$1, (*local_sym_table).get_symbol_table_entry(*$1), yylineno);
								}
								/*formal list check */
								else if(!(*formal_sym_table).is_empty() && (*formal_sym_table).variable_in_symbol_list_check(*$1)){ 
									// formal_sym_table->print(cerr);
									$$ = new Name_Ast(*$1, (*formal_sym_table).get_symbol_table_entry(*$1), yylineno);
									// cerr<<$$->get_symbol_entry().get_variable_name()<<endl;
								}
								else if(!(*global_sym_table).is_empty() && (*global_sym_table).variable_in_symbol_list_check(*$1)){
									$$ = new Name_Ast(*$1, (*global_sym_table).get_symbol_table_entry(*$1), yylineno);
								}
								else{
									yyerror("cs316: Error: Variable has not been declared/defined");
									exit(1);
								}
							}
							| arith_expression '+' arith_expression
							{
								$$ = new Plus_Ast($1, $3, yylineno);
								(*$$).check_ast();
								(*$$).set_data_type((*$1).get_data_type());
							}
							| arith_expression '-' arith_expression
							{
								$$ = new Minus_Ast($1, $3, yylineno);
								(*$$).check_ast();
								(*$$).set_data_type((*$1).get_data_type());
							}
							| arith_expression '*' arith_expression
							{
								$$ = new Mult_Ast($1, $3, yylineno);
								(*$$).check_ast();
								(*$$).set_data_type((*$1).get_data_type());
							}
							| arith_expression '/' arith_expression
							{
								$$ = new Divide_Ast($1, $3, yylineno);
								(*$$).check_ast();
								(*$$).set_data_type((*$1).get_data_type());
							}
							| '-' arith_expression %prec UMINUS
							{
								$$ = new UMinus_Ast($2,NULL,yylineno);
								(*$$).check_ast();
								(*$$).set_data_type((*$2).get_data_type());
							}
							|	log_expression '?' arith_expression ':' arith_expression	/*ternary operator*/
							{
								$$ = new Conditional_Expression_Ast($1, $3, $5, yylineno);
								(*$1).check_ast();
								(*$$).check_ast();

							}
							| '('arith_expression')'
							{
								$$ = $2;
							}
							;


%%