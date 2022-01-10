#include "ast.hh"
#include "local-environment.hh"
#include <stdlib.h>
template class Number_Ast<double>;
template class Number_Ast<int>;

#include "symbol-table.hh"


Ast::Ast(){}

Ast::~Ast(){}

int Ast::labelCounter = 0;

Data_Type Ast::get_data_type(){
    return node_data_type;
}

void Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

bool Ast::is_value_zero() {
    return false;  /* TODO: I'm writing this to mitigate errors in check_ast. */
}

bool Ast::check_ast(){}

Symbol_Table_Entry & Ast::get_symbol_entry(){}


Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line){
    lhs = temp_lhs;
    rhs = temp_rhs;
    lineno = line;
    ast_num_child = binary_arity;
    node_data_type = lhs->get_data_type();
}

Assignment_Ast::~Assignment_Ast(){
    delete lhs;
    delete rhs;
}

bool Assignment_Ast::check_ast(){
    if((lhs->get_data_type() == rhs->get_data_type()) || rhs->is_value_zero()){
        node_data_type = lhs->get_data_type();
        return true;
    }
    else{
        cerr << "cs316: Error: Line "<<lineno<<": Data Type not compatabile in the assignment\n";
        exit(EXIT_FAILURE);
        return false;
    }
}

void Assignment_Ast::print(ostream & file_buffer){
    file_buffer << "\n         Asgn:\n            LHS (";
    lhs->print(file_buffer);
    file_buffer << ")\n            RHS (";
    rhs->print(file_buffer);
    file_buffer <<")";
}


Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line){
    variable_symbol_entry = &var_entry;
    set_data_type(var_entry.get_data_type());
    lineno = line;
}

Name_Ast::~Name_Ast(){}

Data_Type Name_Ast::get_data_type(){
    return node_data_type;
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry(){
    return *variable_symbol_entry;
}

void Name_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

void Name_Ast::print(ostream & file_buffer){
    file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}


template <class T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line){
    constant = number;
    node_data_type = constant_data_type;
    lineno = line;
}

template <class T>
Number_Ast<T>::~Number_Ast(){}

template <class T>
Data_Type Number_Ast<T>::get_data_type(){
    return node_data_type;
}

template <class T>
void Number_Ast<T>::set_data_type(Data_Type dt){
    node_data_type = dt;
}

template <class T>
bool Number_Ast<T>::is_value_zero(){
    if(constant == 0)
        return true;
    else
        return false;
}

template <class T>
void Number_Ast<T>::print(ostream & file_buffer){
    file_buffer << "Num : " << constant;
}


Data_Type Arithmetic_Expr_Ast::get_data_type(){
    return node_data_type;
}


void Arithmetic_Expr_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast(){
    if((ast_num_child == binary_arity) or (ast_num_child == ternary_arity)){
        if((lhs->get_data_type() == rhs->get_data_type()) || rhs->is_value_zero()){ /* TODO : Check the is_value_zero test case in updated implementation */
            node_data_type = lhs->get_data_type();
            return true;
        }
        else{
            cerr << "cs316: Error: Line "<<lineno<<": Data Type not compatabile in arithmetic expression\n";
            exit(EXIT_FAILURE);
            return false;
        }
    }
    else{
        return true;
    }
}


Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line){
    lhs = l;
    rhs = r;
    lineno = line;
    ast_num_child = binary_arity;
}

void Plus_Ast::print(ostream & file_buffer){
    file_buffer << "\n            Arith: PLUS\n               LHS (";
    lhs->print(file_buffer);
    file_buffer << ")\n               RHS (";
    rhs->print(file_buffer);
    file_buffer <<")";
}


Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line){
    lhs = l;
    rhs = r;
    lineno = line;
    ast_num_child = binary_arity;
}

void Minus_Ast::print(ostream & file_buffer){
    file_buffer << "\n            Arith: MINUS\n               LHS (";
    lhs->print(file_buffer);
    file_buffer << ")\n               RHS (";
    rhs->print(file_buffer);
    file_buffer <<")";
}


Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line){
    lhs = l;
    rhs = r;
    lineno = line;
    ast_num_child = binary_arity;
}

void Divide_Ast::print(ostream & file_buffer){
    file_buffer << "\n            Arith: DIV\n               LHS (";
    lhs->print(file_buffer);
    file_buffer << ")\n               RHS (";
    rhs->print(file_buffer);
    file_buffer <<")";
}


Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line){
    lhs = l;
    rhs = r;
    lineno = line;
    ast_num_child = binary_arity;
}

void Mult_Ast::print(ostream & file_buffer){
    file_buffer << "\n            Arith: MULT\n               LHS (";
    lhs->print(file_buffer);
    file_buffer << ")\n               RHS (";
    rhs->print(file_buffer);
    file_buffer <<")";
}

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line){
    lhs = l;
    rhs = r;
    lineno = line;
    ast_num_child = unary_arity;
}

void UMinus_Ast::print(ostream & file_buffer){
    file_buffer << "\n            Arith: UMINUS\n               LHS (";
    lhs->print(file_buffer);
    file_buffer <<")";
}

Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, int line){
    lhs = l;
    rhs = r;
    this->cond = cond;
    ast_num_child = ternary_arity;
    labelCounter++;
}

Conditional_Expression_Ast::~Conditional_Expression_Ast(){}

void Conditional_Expression_Ast::print(ostream & file_buffer){
    file_buffer << "\n         Cond:\n            IF_ELSE";
    cond->print(file_buffer);
    file_buffer << "\n            LHS (";
    lhs->print(file_buffer);
    file_buffer << ")\n            RHS (";
    rhs->print(file_buffer);
    file_buffer <<")";
}


Return_Ast::Return_Ast(int line){
    lineno = line;
    ast_num_child = zero_arity;
}

Return_Ast::~Return_Ast(){}

void Return_Ast::print(ostream & file_buffer){}


Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line){
    lhs_condition = lhs;
    rhs_condition = rhs;
    rel_op = rop;
    lineno = line;
    ast_num_child = binary_arity;
    // node_data_type = int_data_type;
}

Relational_Expr_Ast::~Relational_Expr_Ast(){}

Data_Type Relational_Expr_Ast::get_data_type(){
    return node_data_type;
}

void Relational_Expr_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast(){
    if(lhs_condition->get_data_type() == rhs_condition->get_data_type()){
        node_data_type = lhs_condition->get_data_type(); //for float comparisons
        // node_data_type = int_data_type;
        return true;
    }
    else{
        cerr << "cs316: Error: Line "<<lineno<<": Data Type not compatabile in relational expression\n";
        exit(EXIT_FAILURE);
        return false;
    }
}

void Relational_Expr_Ast::print(ostream & file_buffer){
    file_buffer << "\n            Condition: ";
    if (rel_op == less_equalto)
        file_buffer << "LE";
    else if (rel_op == less_than)
        file_buffer << "LT";
    else if (rel_op == greater_than)
        file_buffer << "GT";
    else if (rel_op == greater_equalto)
        file_buffer << "GE";
    else if(rel_op == equalto)
        file_buffer << "EQ";
    else if (rel_op == not_equalto)
        file_buffer << "NE";
    else{
        cerr << "cs316: Error: Line "<<lineno<<": Unknown Symbol Encountered\n";
        exit(EXIT_FAILURE);
    }
    file_buffer <<  "\n               LHS (";
    lhs_condition->print(file_buffer);
    file_buffer << ")\n               RHS (";
    rhs_condition->print(file_buffer);
    file_buffer <<")";
}


Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line){
    lhs_op = lhs;
    rhs_op = rhs;
    bool_op = bop;
    lineno = line;
    ast_num_child = binary_arity;
    // node_data_type = int_data_type;
}

Logical_Expr_Ast::~Logical_Expr_Ast(){}

Data_Type Logical_Expr_Ast::get_data_type(){
    return node_data_type;
}

void Logical_Expr_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

bool Logical_Expr_Ast::check_ast(){ /* TODO : Check it again */
    if(bool_op == _logical_not){
        if(rhs_op->check_ast()){
            this->node_data_type = rhs_op->get_data_type(); //for float comparisons
            return true;
        }
        else{
            cerr << "cs316: Error: Line "<<lineno<<": Data Type not compatabile in rhs of NOT logical expression\n";
            exit(EXIT_FAILURE);
            return false;
        }
    }
    else{
        if(rhs_op->get_data_type() == lhs_op->get_data_type()){
            this->node_data_type = rhs_op->get_data_type(); //for float comparisons
            return true;
        }
        else{
            cerr << "cs316: Error: Line "<<lineno<<": Data Type not compatabile in logical expression\n";
            exit(EXIT_FAILURE);
            return false;
        }
    }
}

void Logical_Expr_Ast::print(ostream & file_buffer){
    file_buffer << "\n            Condition: ";
    if (bool_op == _logical_not)
        file_buffer << "NOT";
    else if (bool_op == _logical_and)
        file_buffer << "AND";
    else if (bool_op == _logical_or)
        file_buffer << "OR";
    else{
        cerr << "cs316: Error: Line "<<lineno<<": Unknown Boolean Operator Encountered\n";
        exit(EXIT_FAILURE);
    }
    if(bool_op == _logical_not)
    {
        file_buffer << "\n               RHS (";
        rhs_op->print(file_buffer);
        file_buffer <<")";
    }
    else
    {
        file_buffer <<  "\n               LHS (";
        lhs_op->print(file_buffer);
        file_buffer << ")\n               RHS (";
        rhs_op->print(file_buffer);
        file_buffer <<")";
    }
}

Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line){
    node_data_type = void_data_type;
    this->cond = cond;
    this->then_part = then_part;
    this->else_part = else_part;
    lineno = line;
    ast_num_child = ternary_arity;
}

Selection_Statement_Ast::~Selection_Statement_Ast(){}

Data_Type Selection_Statement_Ast::get_data_type(){
    return node_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

bool Selection_Statement_Ast::check_ast(){
    if(else_part){
        if(then_part->check_ast() && else_part->check_ast() && cond->check_ast())
            return true;
        else{
            cerr << "cs316: Error: Line "<<lineno<<": Check Ast failed in Selection statement\n";
            exit(EXIT_FAILURE);
            return false;
        }
    }
    else{
        if(then_part->check_ast() && cond->check_ast())
            return true;
        else{
            cerr << "cs316: Error: Line "<<lineno<<": Check Ast failed in Selection statement\n";
            exit(EXIT_FAILURE);
            return false;
        }
    }
    return false;
}

void Selection_Statement_Ast::print(ostream & file_buffer){
    file_buffer << "\n         IF : \n         CONDITION (";
    cond->print(file_buffer);
    file_buffer <<")\n         THEN (";
    then_part->print(file_buffer);
    file_buffer <<")";
    if(else_part)
    {
        file_buffer <<"\n         ELSE (";
        else_part->print(file_buffer);
        file_buffer <<")";
    }
}


Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form){
    node_data_type = void_data_type;
    this->cond = cond;
    this->body = body;
    is_do_form = do_form;
    lineno = line;
    ast_num_child = binary_arity;
}

Iteration_Statement_Ast::~Iteration_Statement_Ast(){}

Data_Type Iteration_Statement_Ast::get_data_type(){
    return node_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

bool Iteration_Statement_Ast::check_ast(){
    if(cond->check_ast() && body->check_ast())
        return true;
    else{
        cerr << "cs316: Error: Line "<<lineno<<": Check Ast failed in Iteration statement\n";
        exit(EXIT_FAILURE);
        return false;
    }
}

void Iteration_Statement_Ast::print(ostream & file_buffer){

    if(is_do_form){

        file_buffer <<"\n" << AST_SPACE << "DO (";
        body->print(file_buffer);
        file_buffer << ")\n" << AST_SPACE <<"WHILE CONDITION (";
        cond->print(file_buffer);
        file_buffer <<")";
    }
    else{
        file_buffer << "\n         WHILE : ";
        file_buffer << "\n         CONDITION (";
        cond->print(file_buffer);
        file_buffer <<")\n         BODY (";
        body->print(file_buffer);
        file_buffer <<")";
    }
}

Sequence_Ast::Sequence_Ast(int line){
    lineno = line;
    labelCounter++;
}

Sequence_Ast::~Sequence_Ast(){}

void Sequence_Ast::ast_push_back(Ast * ast){
    statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer){
    for(list<Ast*>::iterator it = statement_list.begin(); it != statement_list.end(); it++) {
        file_buffer << "\n" << AST_NODE_SPACE;
        (*it)->print(file_buffer);
    }
}

/*print*/
Print_Ast::Print_Ast(Ast *v, int line) {
    this->var = v;
    this->lineno = line;
}

Print_Ast::~Print_Ast() {

}

void Print_Ast::print(ostream & file_buffer) {
    file_buffer << "\n         Print :\n             (";
    this->var->print(file_buffer);
    file_buffer << ") ";
    // no endline?

}
