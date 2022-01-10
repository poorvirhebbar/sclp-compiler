#include "symbol-table.hh"
#include "ast.hh"
#include "local-environment.hh"
template class Number_Ast<double>;
template class Number_Ast<int>;

Eval_Result & Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {

}

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer) {
    /* TODO: */
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env) {
    /* TODO: */
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result) {
    /* TODO: */
}


Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    string lhs_name = lhs->get_symbol_entry().get_variable_name();
    Table_Scope lhs_scope = lhs->get_symbol_entry().get_symbol_scope();

    Eval_Result *rhs_res = &(rhs->evaluate(eval_env, file_buffer));

    if(lhs_scope == local) {
        eval_env.put_variable_value(*rhs_res, lhs_name);
        print(file_buffer);
        file_buffer << "\n" << AST_SPACE;
        lhs->print_value(eval_env, file_buffer);
    }
    else if(lhs_scope == global) {
        interpreter_global_table.put_variable_value(*rhs_res, lhs_name);
        print(file_buffer);
        file_buffer << "\n" << AST_SPACE;
        lhs->print_value(interpreter_global_table, file_buffer);
    }
    file_buffer << "\n";
    return *rhs_res;
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer) {
    string name = this->variable_symbol_entry->get_variable_name();
    if(eval_env.get_variable_value(name)->get_result_enum() == int_result)
        file_buffer << name << " : " << eval_env.get_variable_value(name)->get_int_value();
    else
        file_buffer << name << " : " << eval_env.get_variable_value(name)->get_double_value();
    file_buffer<<"\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env) {
    string name = this->variable_symbol_entry->get_variable_name();
    return *eval_env.get_variable_value(name);
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result) {
    string name = this->variable_symbol_entry->get_variable_name();
    if(eval_env.does_variable_exist(name)) {
        eval_env.put_variable_value(result, name);
    }
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    string name = this->variable_symbol_entry->get_variable_name();
    Table_Scope var_scope = this->variable_symbol_entry->get_symbol_scope();

    if(var_scope == local && eval_env.is_variable_defined(name)) {
        return get_value_of_evaluation(eval_env);
    }

    else if(var_scope == global && interpreter_global_table.is_variable_defined(name))
    {
        return get_value_of_evaluation(interpreter_global_table);
    }
    cerr << "\ncs316: Error: Line "<<lineno<<": Variable should be defined before its use\n";
    exit(1);
    
        
}

template <class T>
Eval_Result & Number_Ast<T>::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result *res;
    if(get_data_type() == int_data_type)
    {
        res = new Eval_Result_Value_Int();
    }
    else if(get_data_type() == double_data_type)
    {
        res = new Eval_Result_Value_Double();
    }
    res->set_value(constant);
    return *res;
}

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result *res;
    Eval_Result *lhs_res = &(lhs->evaluate(eval_env, file_buffer));
    Eval_Result *rhs_res = &(rhs->evaluate(eval_env, file_buffer));
    if(get_data_type() == int_data_type) {
        res = new Eval_Result_Value_Int();
        res->set_value(lhs_res->get_int_value() + rhs_res->get_int_value());
    }
    else {
        res = new Eval_Result_Value_Double();
        res->set_value(lhs_res->get_double_value() + rhs_res->get_double_value());
    }
    return *res;
}

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result *res;
    Eval_Result *lhs_res = &(lhs->evaluate(eval_env, file_buffer));
    Eval_Result *rhs_res = &(rhs->evaluate(eval_env, file_buffer));
    if(get_data_type() == int_data_type) {
        res = new Eval_Result_Value_Int();
        res->set_value(lhs_res->get_int_value() - rhs_res->get_int_value());
    }
    else {
        res = new Eval_Result_Value_Double();
        res->set_value(lhs_res->get_double_value() - rhs_res->get_double_value());
    }
    return *res;
}

Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result *res;
    Eval_Result *lhs_res = &(lhs->evaluate(eval_env, file_buffer));
    Eval_Result *rhs_res = &(rhs->evaluate(eval_env, file_buffer));
    if(get_data_type() == int_data_type) {
        res = new Eval_Result_Value_Int();
        if(rhs_res->get_int_value() == 0)
        {
            cerr << "cs316: Error: Line "<<lineno<<": Divide by zero\n";
            exit(EXIT_FAILURE);
        }
        else{
             res->set_value(lhs_res->get_int_value()/rhs_res->get_int_value());
        }
    }
    else {
        if(rhs_res->get_int_value() == 0)
        {
            cerr << "cs316: Error: Line "<<lineno<<": Divide by zero\n";
            exit(EXIT_FAILURE);
        }
        res = new Eval_Result_Value_Double();
        res->set_value(lhs_res->get_double_value()/rhs_res->get_double_value());
    }
    return *res;
}

Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result *res;
    Eval_Result *lhs_res = &(lhs->evaluate(eval_env, file_buffer));
    Eval_Result *rhs_res = &(rhs->evaluate(eval_env, file_buffer));
    if(get_data_type() == int_data_type) {
        res = new Eval_Result_Value_Int();
        res->set_value(lhs_res->get_int_value()*rhs_res->get_int_value());
    }
    else {
        res = new Eval_Result_Value_Double();
        res->set_value(lhs_res->get_double_value()*rhs_res->get_double_value());
    }
    return *res;
}


Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {
    Eval_Result *res;
    Eval_Result *lhs_res = &(lhs->evaluate(eval_env, file_buffer));
    if(get_data_type() == int_data_type) {
        res = new Eval_Result_Value_Int();
        res->set_value((-1)*lhs_res->get_int_value());
    }
    else {
        res = new Eval_Result_Value_Double();
        res->set_value((-1.0)*lhs_res->get_double_value()); /*you wrote get_double_value()*/
    }
    return *res;
}

Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result *res;
    Eval_Result *lhs_res = &(lhs->evaluate(eval_env, file_buffer));
    Eval_Result *rhs_res = &(rhs->evaluate(eval_env, file_buffer));
    Eval_Result *cond_res = &(cond->evaluate(eval_env, file_buffer));
    if(get_data_type() == int_data_type) {
        res = new Eval_Result_Value_Int();
        if(cond_res->get_int_value() == 1)
            res->set_value(lhs_res->get_int_value());
        else
            res->set_value(rhs_res->get_int_value());
    }
    else {
        res = new Eval_Result_Value_Double();
        if(cond_res->get_int_value() == 1)
            res->set_value(lhs_res->get_double_value());
        else
            res->set_value(rhs_res->get_double_value());
    }
    return *res;
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    /* TODO: */
}

Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    /* TODO: */
}

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result *res;
    Eval_Result *lhs_res = &(lhs_condition->evaluate(eval_env, file_buffer));
    Eval_Result *rhs_res = &(rhs_condition->evaluate(eval_env, file_buffer));

    if(lhs_res->get_result_enum() == int_result) {
        res = new Eval_Result_Value_Int();
        res->set_value(0);

        if (rel_op == less_equalto)
        {
            if(lhs_res->get_int_value() <= rhs_res->get_int_value())
                res->set_value(1);
        }
        else if (rel_op == less_than)
        {
            if(lhs_res->get_int_value() < rhs_res->get_int_value())
                res->set_value(1);
        }
        else if (rel_op == greater_than)
        {
            if(lhs_res->get_int_value() > rhs_res->get_int_value())
                res->set_value(1);
        }
        else if (rel_op == greater_equalto)
        {
            if(lhs_res->get_int_value() >= rhs_res->get_int_value())
                res->set_value(1);
        }
        else if(rel_op == equalto)
        {
            if(lhs_res->get_int_value() == rhs_res->get_int_value())
                res->set_value(1);
        }
        else if (rel_op == not_equalto)
        {
            if(lhs_res->get_int_value() != rhs_res->get_int_value())
                res->set_value(1);
        }
        else{
            cerr << "cs316: Error: Line "<<lineno<<": Unknown Symbol Encountered\n";
            exit(EXIT_FAILURE);
        }
    }
    
    else if(lhs_res->get_result_enum() == double_result) {
        res = new Eval_Result_Value_Int();
        res->set_value(0);

        if (rel_op == less_equalto)
        {
            if(lhs_res->get_double_value() <= rhs_res->get_double_value())
                res->set_value(1);
        }
        else if (rel_op == less_than)
        {
            if(lhs_res->get_double_value() < rhs_res->get_double_value())
                res->set_value(1);
        }
        else if (rel_op == greater_than)
        {
            if(lhs_res->get_double_value() > rhs_res->get_double_value())
                res->set_value(1);
        }
        else if (rel_op == greater_equalto)
        {
            if(lhs_res->get_double_value() >= rhs_res->get_double_value())
                res->set_value(1);
        }
        else if(rel_op == equalto)
        {
            if(lhs_res->get_double_value() == rhs_res->get_double_value())
                res->set_value(1);
        }
        else if (rel_op == not_equalto)
        {
            if(lhs_res->get_double_value() != rhs_res->get_double_value())
                res->set_value(1);
        }
        else{
            cerr << "cs316: Error: Line "<<lineno<<": Unknown Symbol Encountered\n";
            exit(EXIT_FAILURE);
        }
    }
    else {
        cerr << "cs316: Error: Line "<<lineno<<": Unknown data type encountered\n";
        exit(EXIT_FAILURE);
    }
    return *res;
}

Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result *res = new Eval_Result_Value_Int();
    Eval_Result *rhs_res = &(rhs_op->evaluate(eval_env, file_buffer));
    res->set_value(0);

    if (bool_op == _logical_not)
    {
        if(!(rhs_res->get_int_value()))
            res->set_value(1);
    }
    else if (bool_op == _logical_and)
    {
        Eval_Result *lhs_res = &(lhs_op->evaluate(eval_env, file_buffer));
        if(lhs_res->get_int_value() && rhs_res->get_int_value())
            res->set_value(1);
    }
    else if (bool_op == _logical_or)
    {
        Eval_Result *lhs_res = &(lhs_op->evaluate(eval_env, file_buffer));
        if(lhs_res->get_int_value() || rhs_res->get_int_value())
            res->set_value(1);
    }
    else{
        cerr << "cs316: Error: Line "<<lineno<<": Unknown Boolean Operator Encountered\n";
        exit(EXIT_FAILURE);
    }
    return *res;
}

Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result *res = new Eval_Result_Value_Int();
    Eval_Result *cond_res = &(cond->evaluate(eval_env, file_buffer));

    if (cond_res->get_int_value() == 1)
    {
        res = &(then_part->evaluate(eval_env, file_buffer));
    }
    else if(cond_res->get_int_value() == 0)
    {
        if(else_part != NULL)
            res = &(else_part->evaluate(eval_env, file_buffer));
    }
    else{
        cerr << "cs316: Error: Line "<<lineno<<": Unknown Error in Selection Statement Encountered\n";
        exit(EXIT_FAILURE);
    }
    return *res;
}

Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    if(is_do_form)
    {
        Eval_Result *lhs_res;
        do {
            body->evaluate(eval_env, file_buffer);
            lhs_res = &(cond->evaluate(eval_env, file_buffer));
        } while(lhs_res->get_int_value() == 1);
    }
    else
    {
        Eval_Result *lhs_res = &(cond->evaluate(eval_env, file_buffer));
        while(lhs_res->get_int_value() == 1){
            body->evaluate(eval_env, file_buffer);
            lhs_res = &(cond->evaluate(eval_env, file_buffer));
        }
    }
}

Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    for(list<Ast*>::iterator it = statement_list.begin(); it != statement_list.end(); it++) {
        (*it)->evaluate(eval_env,file_buffer);
    }
}

void Symbol_Table::create(Local_Environment & local_global_variables_table){
    list<Symbol_Table_Entry*>::iterator it;
    for(it = this->variable_table.begin(); it != this->variable_table.end(); it++) {
        Eval_Result *er;
        if((*it)->get_data_type() == int_data_type) {
            er = new Eval_Result_Value_Int();
            er->set_variable_status(0);
            if((*it)->get_symbol_scope() == global)
                er->set_value(0);
            local_global_variables_table.put_variable_value(*er, (*it)->get_variable_name());
        }
        else if((*it)->get_data_type() == double_data_type) {
            er = new Eval_Result_Value_Double();
            er->set_variable_status(0);
            if((*it)->get_symbol_scope() == global)
                er->set_value(0);
            local_global_variables_table.put_variable_value(*er, (*it)->get_variable_name());
        }
    }
}