#include "symbol-table.hh"
#include "ast.hh"
#include "local-environment.hh"
#include "icode.hh"
#include "reg-alloc.hh"
template class Number_Ast<double>;
template class Number_Ast<int>;

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register) {

}

Code_For_Ast & Assignment_Ast::compile() {
	Code_For_Ast rhs_res = rhs->compile();
	Register_Addr_Opd *rhs_result = new Register_Addr_Opd(rhs_res.get_reg());	
	Code_For_Ast lhs_res = this->lhs->create_store_stmt(rhs_result->get_reg());
	Code_For_Ast *output = new Code_For_Ast(rhs_res.get_icode_list(), rhs_res.get_reg());
	output->append_ics(*(lhs_res.get_icode_list().front()));
	rhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	return *output;
}


Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
}


Code_For_Ast & Name_Ast::compile() {
	Mem_Addr_Opd *opd1  = new Mem_Addr_Opd(*this->variable_symbol_entry);
	Register_Descriptor *rd;
	Register_Addr_Opd *result;
	Move_IC_Stmt *load_stmt;
	if(this->variable_symbol_entry->get_data_type() == int_data_type) {
		rd = machine_desc_object.get_new_register<int_reg>();
		result = new Register_Addr_Opd(rd);
		load_stmt = new Move_IC_Stmt(load, opd1, result);
	}
	else if(this->variable_symbol_entry->get_data_type() == double_data_type) {
		rd = machine_desc_object.get_new_register<float_reg>();
		result = new Register_Addr_Opd(rd);
		load_stmt = new Move_IC_Stmt(load_d, opd1, result);
	}
	Code_For_Ast *output = new Code_For_Ast();
	output->set_reg(rd);
	output->append_ics(*load_stmt);
	return *output;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
	//TODO:
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register) {
	Register_Addr_Opd *result = new Register_Addr_Opd(store_register);
	Mem_Addr_Opd *opd1  = new Mem_Addr_Opd(*this->variable_symbol_entry);
	Move_IC_Stmt *store_stmt;
	Code_For_Ast *output = new Code_For_Ast();
	if(store_register->get_use_category() == int_reg) {
		store_stmt = new Move_IC_Stmt(store, result, opd1);
	}
	else if(store_register->get_use_category() == float_reg) {
		store_stmt = new Move_IC_Stmt(store_d, result, opd1);
	}
	output->set_reg(store_register);
	output->append_ics(*store_stmt);
	return *output;
}

template <class T>
Code_For_Ast & Number_Ast<T>::compile() {
	Const_Opd<T> *opd1  = new Const_Opd<T>(this->constant);
	Register_Descriptor *rd;
	Move_IC_Stmt *load_stmt;
	Register_Addr_Opd *result;
	if(this->get_data_type() == int_data_type) {
		rd = machine_desc_object.get_new_register<int_reg>();
		result = new Register_Addr_Opd(rd);
		load_stmt = new Move_IC_Stmt(imm_load, opd1, result);
	}
		
	else if(this->get_data_type() == double_data_type) {
		rd = machine_desc_object.get_new_register<float_reg>();
		result = new Register_Addr_Opd(rd);
		load_stmt = new Move_IC_Stmt(imm_load_d, opd1, result);
	}
	 
	Code_For_Ast *output = new Code_For_Ast();
	output->set_reg(rd);
	output->append_ics(*load_stmt);
	return *output;
}


template <class T>
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra) {
	//TODO:
}


Code_For_Ast & Plus_Ast::compile() {
	Code_For_Ast lhs_code =	this->lhs->compile(); 
	Code_For_Ast rhs_code =	this->rhs->compile(); 
	Register_Addr_Opd *lhs_result = new Register_Addr_Opd(lhs_code.get_reg());
	Register_Addr_Opd *rhs_result = new Register_Addr_Opd(rhs_code.get_reg());
	Register_Descriptor *rd;
	Register_Addr_Opd *result;
	Compute_IC_Stmt *plus_stmt;
	if(this->lhs->get_data_type() == int_data_type) {
		rd = machine_desc_object.get_new_register<int_reg>();
		result = new Register_Addr_Opd(rd);
		plus_stmt = new Compute_IC_Stmt(add, lhs_result, rhs_result, result);
	}
	else if(this->lhs->get_data_type() == double_data_type) {
		rd = machine_desc_object.get_new_register<float_reg>();
		result = new Register_Addr_Opd(rd);
		plus_stmt = new Compute_IC_Stmt(add_d, lhs_result, rhs_result, result);
	}
	Code_For_Ast *output = new Code_For_Ast(lhs_code.get_icode_list(), rd);
	output->get_icode_list().insert(output->get_icode_list().end(),rhs_code.get_icode_list().begin(),rhs_code.get_icode_list().end());
	output->set_reg(rd);
	output->append_ics(*plus_stmt);
	lhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	rhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	return *output;
}


Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
	//TODO:
}

Code_For_Ast & Minus_Ast::compile() {
	Code_For_Ast lhs_code =	this->lhs->compile(); 
	Code_For_Ast rhs_code =	this->rhs->compile(); 
	Register_Addr_Opd *lhs_result = new Register_Addr_Opd(lhs_code.get_reg());
	Register_Addr_Opd *rhs_result = new Register_Addr_Opd(rhs_code.get_reg());
	Register_Descriptor *rd;
	Register_Addr_Opd *result;
	Compute_IC_Stmt *minus_stmt;
	if(this->lhs->get_data_type() == int_data_type) {
		rd = machine_desc_object.get_new_register<int_reg>();
		result = new Register_Addr_Opd(rd);
		minus_stmt = new Compute_IC_Stmt(sub, lhs_result, rhs_result, result);
	}
	else if(this->lhs->get_data_type() == double_data_type) {
		rd = machine_desc_object.get_new_register<float_reg>();
		result = new Register_Addr_Opd(rd);
		minus_stmt = new Compute_IC_Stmt(sub_d, lhs_result, rhs_result, result);
	}
	Code_For_Ast *output = new Code_For_Ast(lhs_code.get_icode_list(), rd);
	output->get_icode_list().insert(output->get_icode_list().end(),rhs_code.get_icode_list().begin(),rhs_code.get_icode_list().end());
	output->set_reg(rd);
	output->append_ics(*minus_stmt);
	lhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	rhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	return *output;
}


Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
	//TODO:
}

Code_For_Ast & Divide_Ast::compile() {
	Code_For_Ast lhs_code =	this->lhs->compile(); 
	Code_For_Ast rhs_code =	this->rhs->compile(); 
	Register_Addr_Opd *lhs_result = new Register_Addr_Opd(lhs_code.get_reg());
	Register_Addr_Opd *rhs_result = new Register_Addr_Opd(rhs_code.get_reg());
	Register_Descriptor *rd;
	Register_Addr_Opd *result;
	Compute_IC_Stmt *divide_stmt;
	if(this->lhs->get_data_type() == int_data_type) {
		rd = machine_desc_object.get_new_register<int_reg>();
		result = new Register_Addr_Opd(rd);
		divide_stmt = new Compute_IC_Stmt(divd, lhs_result, rhs_result, result);
	}
	else if(this->lhs->get_data_type() == double_data_type) {
		rd = machine_desc_object.get_new_register<float_reg>();
		result = new Register_Addr_Opd(rd);
		divide_stmt = new Compute_IC_Stmt(div_d, lhs_result, rhs_result, result);
	}
	Code_For_Ast *output = new Code_For_Ast(lhs_code.get_icode_list(), rd);
	output->get_icode_list().insert(output->get_icode_list().end(),rhs_code.get_icode_list().begin(),rhs_code.get_icode_list().end());
	output->set_reg(rd);
	output->append_ics(*divide_stmt);
	lhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	rhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	return *output;
}

Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
	//TODO:
}

Code_For_Ast & Mult_Ast::compile() {
	Code_For_Ast lhs_code =	this->lhs->compile(); 
	Code_For_Ast rhs_code =	this->rhs->compile(); 
	Register_Addr_Opd *lhs_result = new Register_Addr_Opd(lhs_code.get_reg());
	Register_Addr_Opd *rhs_result = new Register_Addr_Opd(rhs_code.get_reg());
	Register_Descriptor *rd;
	Register_Addr_Opd *result;
	Compute_IC_Stmt *mult_stmt;
	if(this->lhs->get_data_type() == int_data_type) {
		rd = machine_desc_object.get_new_register<int_reg>();
		result = new Register_Addr_Opd(rd);
		mult_stmt = new Compute_IC_Stmt(mult, lhs_result, rhs_result, result);
	}
	else if(this->lhs->get_data_type() == double_data_type) {
		rd = machine_desc_object.get_new_register<float_reg>();
		result = new Register_Addr_Opd(rd);
		mult_stmt = new Compute_IC_Stmt(mult_d, lhs_result, rhs_result, result);
	}
	Code_For_Ast *output = new Code_For_Ast(lhs_code.get_icode_list(), rd);
	output->get_icode_list().insert(output->get_icode_list().end(),rhs_code.get_icode_list().begin(),rhs_code.get_icode_list().end());
	output->set_reg(rd);
	output->append_ics(*mult_stmt);
	lhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	rhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	return *output;
}

Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
	//TODO:
}

Code_For_Ast & UMinus_Ast::compile() {
	Code_For_Ast lhs_code =	this->lhs->compile(); 
	Register_Addr_Opd *lhs_result = new Register_Addr_Opd(lhs_code.get_reg());
	Register_Descriptor *rd;
	Register_Addr_Opd *result;
	Move_IC_Stmt *uminus_stmt;
	if(this->lhs->get_data_type() == int_data_type) {
		rd = machine_desc_object.get_new_register<int_reg>();
		result = new Register_Addr_Opd(rd);
		uminus_stmt = new Move_IC_Stmt(uminus, lhs_result, result);
	}
	else if(this->lhs->get_data_type() == double_data_type) {
		rd = machine_desc_object.get_new_register<float_reg>();
		result = new Register_Addr_Opd(rd);
		uminus_stmt = new Move_IC_Stmt(uminus_d, lhs_result, result);
	}
	Code_For_Ast *output = new Code_For_Ast(lhs_code.get_icode_list(), rd);
	output->set_reg(rd);
	output->append_ics(*uminus_stmt);
	lhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	return *output;
}


Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
	//TODO:
}

Code_For_Ast & Conditional_Expression_Ast::compile() {
	
	Code_For_Ast cond_code = this->cond->compile();
	Label_IC_Stmt * label1 = new Label_IC_Stmt(j, this->get_new_label()); 

    Code_For_Ast lhs_code = this->lhs->compile();
	Code_For_Ast rhs_code = this->rhs->compile();
	Register_Addr_Opd *lhs_result = new Register_Addr_Opd(lhs_code.get_reg());
	Register_Addr_Opd *rhs_result = new Register_Addr_Opd(rhs_code.get_reg());
	Register_Addr_Opd *cond_result = new Register_Addr_Opd(cond_code.get_reg());

	Register_Descriptor *rd;
	if(this->lhs->get_data_type() == int_data_type) {
		rd = machine_desc_object.get_new_register<int_reg>();
	}
	else {
		rd = machine_desc_object.get_new_register<float_reg>();
	}
	Register_Addr_Opd *result = new Register_Addr_Opd(rd);


	Label_IC_Stmt * label2 = new Label_IC_Stmt(j, this->get_new_label()); 
	Control_Flow_IC_Stmt * jump_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label2->get_label());

	Register_Addr_Opd *zero_reg = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);
	Compute_IC_Stmt* lhs_or_stmt = new Compute_IC_Stmt(or_t, lhs_result, zero_reg, result);
	Compute_IC_Stmt* rhs_or_stmt = new Compute_IC_Stmt(or_t, rhs_result, zero_reg, result);

	Control_Flow_IC_Stmt * branch_stmt;
	if(this->lhs->get_data_type() == int_data_type) {
		branch_stmt = new Control_Flow_IC_Stmt(beq, cond_result, zero_reg, label1->get_label());
	}
	if(this->lhs->get_data_type() == double_data_type) {
		branch_stmt = new Control_Flow_IC_Stmt(bc1f, NULL, NULL, label1->get_label());
	}

	Code_For_Ast *output = new Code_For_Ast();
	output->set_reg(rd);
	output->get_icode_list().insert(output->get_icode_list().end(),cond_code.get_icode_list().begin(), cond_code.get_icode_list().end());
	output->append_ics(*branch_stmt);
	output->get_icode_list().insert(output->get_icode_list().end(),lhs_code.get_icode_list().begin(), lhs_code.get_icode_list().end());
	output->append_ics(*lhs_or_stmt);
	output->append_ics(*jump_stmt);
	output->append_ics(*label1);
	output->get_icode_list().insert(output->get_icode_list().end(),rhs_code.get_icode_list().begin(), rhs_code.get_icode_list().end());
	output->append_ics(*rhs_or_stmt);
	output->append_ics(*label2);

	lhs_result->get_reg()->reset_use_for_expr_result();  //Free the registers
	rhs_result->get_reg()->reset_use_for_expr_result();  //Free the registers
	cond_result->get_reg()->reset_use_for_expr_result(); //Free the registers
	return *output;
}


Code_For_Ast & Relational_Expr_Ast::compile() {
	Code_For_Ast lhs_code =	this->lhs_condition->compile(); 
	Code_For_Ast rhs_code =	this->rhs_condition->compile(); 
	Register_Addr_Opd *lhs_result = new Register_Addr_Opd(lhs_code.get_reg());
	Register_Addr_Opd *rhs_result = new Register_Addr_Opd(rhs_code.get_reg());
	Register_Descriptor *rd = machine_desc_object.get_new_register<int_reg>();
	Register_Addr_Opd *result = new Register_Addr_Opd(rd);
	
	Compute_IC_Stmt * comp_stmt;

	result = new Register_Addr_Opd(rd);


	if(this->lhs_condition->get_data_type() == int_data_type) {
		if(this->rel_op == less_equalto) {
			comp_stmt = new Compute_IC_Stmt(sle, lhs_result, rhs_result, result);
		}
		else if(this->rel_op == less_than) {
			comp_stmt = new Compute_IC_Stmt(slt, lhs_result, rhs_result, result);
		}
		else if(this->rel_op == greater_than) {
			comp_stmt = new Compute_IC_Stmt(sgt, lhs_result, rhs_result, result);
		}
		else if(this->rel_op == greater_equalto) {
			comp_stmt = new Compute_IC_Stmt(sge, lhs_result, rhs_result, result);
		}
		else if(this->rel_op == equalto) {
			comp_stmt = new Compute_IC_Stmt(seq, lhs_result, rhs_result, result);
		}
		else if(this->rel_op == not_equalto) {
			comp_stmt = new Compute_IC_Stmt(sne, lhs_result, rhs_result, result);
		}
	}
	else {
		if(this->rel_op == less_equalto) {
			comp_stmt = new Compute_IC_Stmt(sle_d, lhs_result, rhs_result, NULL);
		}
		else if(this->rel_op == less_than) {
			comp_stmt = new Compute_IC_Stmt(slt_d, lhs_result, rhs_result, NULL);
		}
		else if(this->rel_op == equalto) {
			comp_stmt = new Compute_IC_Stmt(seq_d, lhs_result, rhs_result, NULL);
		}
		else if(this->rel_op == greater_than) {
			comp_stmt = new Compute_IC_Stmt(sgt_d, lhs_result, rhs_result, NULL);
		}
		else if(this->rel_op == greater_equalto) {
			comp_stmt = new Compute_IC_Stmt(sge_d, lhs_result, rhs_result, NULL);
		}
		else if(this->rel_op == not_equalto) {
			comp_stmt = new Compute_IC_Stmt(sne_d, lhs_result, rhs_result, NULL);
		}
	}
	lhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers
	rhs_result->get_reg()->reset_use_for_expr_result();	// Free the registers

	Code_For_Ast *output = new Code_For_Ast(lhs_code.get_icode_list(), rd);
	output->get_icode_list().insert(output->get_icode_list().end(),rhs_code.get_icode_list().begin(),rhs_code.get_icode_list().end());
	output->set_reg(rd);
	output->append_ics(*comp_stmt);
	return *output;
}

Code_For_Ast & Logical_Expr_Ast::compile() {
	Register_Addr_Opd *reg_for_one;
	if(this->bool_op == _logical_not)
		reg_for_one = new Register_Addr_Opd(machine_desc_object.get_new_register<int_reg>()); 

	Code_For_Ast lhs_code;
	Register_Addr_Opd *lhs_result;
	if(this->bool_op != _logical_not) {
		lhs_code =	this->lhs_op->compile(); 
		lhs_result = new Register_Addr_Opd(lhs_code.get_reg());
	}
	Code_For_Ast rhs_code =	this->rhs_op->compile(); 
	Register_Addr_Opd *rhs_result = new Register_Addr_Opd(rhs_code.get_reg());


	Register_Descriptor *rd = machine_desc_object.get_new_register<int_reg>();
	Register_Addr_Opd *result = new Register_Addr_Opd(rd);
	
	Compute_IC_Stmt * comp_stmt;

	Move_IC_Stmt *load_one_stmt;
	Const_Opd<int> *constant;


	/* not is a special case. For !x, load 1, give operands x and a to not(in that order). NOT NULL.
	   not_t in assembly is sltu. That's the reason for this weird behaviour.
	*/
	if(this->bool_op == _logical_and) {
		comp_stmt = new Compute_IC_Stmt(and_t, lhs_result, rhs_result, result);
	}
	else if(this->bool_op == _logical_or) {
		comp_stmt = new Compute_IC_Stmt(or_t, lhs_result, rhs_result, result);
	}
	else if(this->bool_op == _logical_not) {
		constant = new Const_Opd<int>(1);
		load_one_stmt = new Move_IC_Stmt(imm_load, constant, reg_for_one);
		comp_stmt = new Compute_IC_Stmt(not_t, rhs_result, reg_for_one, result);
	}

	Code_For_Ast *output = new Code_For_Ast();
	output->set_reg(rd);
	if(this->bool_op == _logical_not) {
		output->append_ics(*load_one_stmt);
		reg_for_one->get_reg()->reset_use_for_expr_result(); //Free the registers
	}
	if(this->bool_op != _logical_not) {
		output->get_icode_list().insert(output->get_icode_list().end(),lhs_code.get_icode_list().begin(),lhs_code.get_icode_list().end());	
		lhs_result->get_reg()->reset_use_for_expr_result(); //Free the registers
	}
	output->get_icode_list().insert(output->get_icode_list().end(),rhs_code.get_icode_list().begin(),rhs_code.get_icode_list().end());
	output->append_ics(*comp_stmt);
	rhs_result->get_reg()->reset_use_for_expr_result(); //Free the registers
	return *output;
}

Code_For_Ast & Selection_Statement_Ast::compile() {
	Code_For_Ast cond_code = this->cond->compile(); 
	Code_For_Ast then_code = this->then_part->compile(); 
	Code_For_Ast else_code;
	Label_IC_Stmt * label1 = new Label_IC_Stmt(j, this->get_new_label()); 
	Label_IC_Stmt * label2;
	Control_Flow_IC_Stmt *jump_stmt;

	if(this->else_part != NULL) {
		label2 = new Label_IC_Stmt(j, this->get_new_label());
		else_code = this->else_part->compile();
		jump_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label2->get_label());
	}

	Register_Addr_Opd *cond_result = new Register_Addr_Opd(cond_code.get_reg());
	Register_Descriptor *rd = machine_desc_object.get_new_register<int_reg>();
	Register_Addr_Opd *zero_reg = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);

	Control_Flow_IC_Stmt *rel_stmt;
	if(this->cond->get_data_type() == int_data_type) {
		rel_stmt = new Control_Flow_IC_Stmt(beq,  cond_result, zero_reg, label1->get_label());
	}
	else {
		list<Icode_Stmt*>::iterator it = cond_code.get_icode_list().end();
		it--;
		Tgt_Op opr = ((*it)->get_op()).get_op();
		if(opr == seq_d || opr == sle_d || opr == slt_d) {
			rel_stmt = new Control_Flow_IC_Stmt(bc1f, NULL, NULL, label1->get_label());
		}
		else {
			rel_stmt = new Control_Flow_IC_Stmt(bc1t, NULL, NULL, label1->get_label());	
		}
	}

	Code_For_Ast *output = new Code_For_Ast(cond_code.get_icode_list(), rd);
	output->append_ics(*rel_stmt);
	output->get_icode_list().insert(output->get_icode_list().end(),then_code.get_icode_list().begin(),then_code.get_icode_list().end());
	if(this->else_part != NULL) {
		output->append_ics(*jump_stmt);
	}
	output->append_ics(*label1);
	if(this->else_part != NULL) {
		output->get_icode_list().insert(output->get_icode_list().end(),else_code.get_icode_list().begin(),else_code.get_icode_list().end());
		output->append_ics(*label2);
	}
	
	output->set_reg(rd);
	cond_result->get_reg()->reset_use_for_expr_result(); //Free the registers
	rd->reset_use_for_expr_result();
	return *output;
}

Code_For_Ast & Iteration_Statement_Ast::compile() {
	Code_For_Ast cond_code = this->cond->compile(); 
	Label_IC_Stmt * label1 = new Label_IC_Stmt(j, this->get_new_label()); 
	Label_IC_Stmt * label2 = new Label_IC_Stmt(j, this->get_new_label()); 

	Code_For_Ast body_code = this->body->compile(); 

	Control_Flow_IC_Stmt * jump_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label2->get_label());

	Register_Addr_Opd *cond_result = new Register_Addr_Opd(cond_code.get_reg());
	Register_Descriptor *rd = machine_desc_object.get_new_register<int_reg>();
	Register_Addr_Opd *zero_reg = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);

	Control_Flow_IC_Stmt *control_stmt;
	if(this->cond->get_data_type() == int_data_type) {
		control_stmt = new Control_Flow_IC_Stmt(bne, cond_result, zero_reg, label1->get_label());
	}
	else {
		list<Icode_Stmt*>::iterator it = cond_code.get_icode_list().end();
		it--;
		Tgt_Op opr = ((*it)->get_op()).get_op();
		if(opr == seq_d || opr == sle_d || opr == slt_d) {
			control_stmt = new Control_Flow_IC_Stmt(bc1t, NULL, NULL, label1->get_label());	
		}
		else {
			control_stmt = new Control_Flow_IC_Stmt(bc1f, NULL, NULL, label1->get_label());
		}
	}

	jump_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, label2->get_label());

	Code_For_Ast *output = new Code_For_Ast();
	output->set_reg(rd);
	if(!this->is_do_form)
		output->append_ics(*jump_stmt);
	output->append_ics(*label1);
	output->get_icode_list().insert(output->get_icode_list().end(), body_code.get_icode_list().begin(), body_code.get_icode_list().end());
	output->append_ics(*label2);
	output->get_icode_list().insert(output->get_icode_list().end(), cond_code.get_icode_list().begin(), cond_code.get_icode_list().end());
	output->append_ics(*control_stmt);
	
	cond_result->get_reg()->reset_use_for_expr_result();
	rd->reset_use_for_expr_result();
	return *output;
}

Code_For_Ast & Sequence_Ast::compile() {
	list<Ast *>::iterator it;
	for(it = this->statement_list.begin(); it != this->statement_list.end(); it++) {
		Code_For_Ast temp = (*it)->compile();
		this->sa_icode_list.insert(this->sa_icode_list.end(), temp.get_icode_list().begin(),
		 temp.get_icode_list().end());
	}

	Code_For_Ast *output = new Code_For_Ast(this->sa_icode_list, NULL);
	return *output;
}

Code_For_Ast & Return_Ast::compile() {

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	Control_Flow_IC_Stmt *jump_stmt = new Control_Flow_IC_Stmt(j, NULL, NULL, "epilogue_" + this->proc_name);

	if(this->get_data_type() == void_data_type) {
		ic_list.push_back(jump_stmt);
		Code_For_Ast * output = new Code_For_Ast(ic_list, NULL);
		return *output;
	}

	Code_For_Ast return_code =	this->return_value->compile();
	Register_Addr_Opd *return_result = new Register_Addr_Opd(return_code.get_reg());

	Register_Addr_Opd *RAO;
	Move_IC_Stmt *load;
	Label_IC_Stmt * label = new Label_IC_Stmt(j, "epilogue_" + this->proc_name);

	if(this->get_data_type() == int_data_type) {
		RAO = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);
		load = new Move_IC_Stmt(mov, return_result, RAO);
	}

	if(this->get_data_type() == double_data_type) {
		RAO = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);
		load = new Move_IC_Stmt(move_d, return_result, RAO);
	}

	if(!return_code.get_icode_list().empty())
		ic_list = return_code.get_icode_list();

	ic_list.push_back(load);
	ic_list.push_back(jump_stmt);

	Code_For_Ast *output = new Code_For_Ast(ic_list, NULL);

	return_result->get_reg()->reset_use_for_expr_result(); 		/* Free the registers */
	// if(this->get_data_type() == int_data_type)
	// 	output->set_reg(machine_desc_object.spim_register_table[v1]);
	// else if(this->get_data_type() == double_data_type)
	// 	output->set_reg(machine_desc_object.spim_register_table[f0]);
	// cerr<<"output "<<output->get_reg()<<endl;
	return *output;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
	//TODO:
}

Code_For_Ast & Call_Ast::compile() {

	this->check_actual_formal_param(program_object.get_procedure_prototype(this->procedure_name)->get_formal_param_list());

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	Register_Addr_Opd * stack_opd = new Register_Addr_Opd(machine_desc_object.spim_register_table[sp]);

	list<Ast *>::iterator it;
	int offset = 0;

	for(it = this->actual_param_list.end(); it != this->actual_param_list.begin();) {
		--it;
		Code_For_Ast param_code = (*it)->compile();
		Register_Addr_Opd *param_result = new Register_Addr_Opd(param_code.get_reg());
		if(!param_code.get_icode_list().empty())
			ic_list.insert(ic_list.end(), param_code.get_icode_list().begin(), param_code.get_icode_list().end());
		if((*it)->get_data_type() == int_data_type){
			string x = "dummy";
			Symbol_Table_Entry	*arg = new Symbol_Table_Entry(x, int_data_type, (*it)->get_symbol_entry().get_lineno(), sp_ref);
			arg->set_start_offset(offset);
			offset -= 4; //reduce offset after setting in arg
			Mem_Addr_Opd *opd  = new Mem_Addr_Opd(*arg);
			Move_IC_Stmt *store_stmt = new Move_IC_Stmt(store, param_result, opd);
			ic_list.push_back(store_stmt);
			// DO INSERTION HERE
		}
		else if((*it)->get_data_type() == double_data_type){
			string x = "dummy";
			Symbol_Table_Entry	*arg = new Symbol_Table_Entry(x, int_data_type, (*it)->get_symbol_entry().get_lineno(), sp_ref);
			arg->set_start_offset(offset);
			offset -= 8; //reduce offset after setting in arg
			Mem_Addr_Opd *opd  = new Mem_Addr_Opd(*arg);
			Move_IC_Stmt *store_stmt = new Move_IC_Stmt(store_d, param_result, opd);
			ic_list.push_back(store_stmt);	
			// DO INSERTION HERE
		}	
		param_result->get_reg()->reset_use_for_expr_result(); 		/* Free the registers */
	}

	Const_Opd<int> * offset_opd = new Const_Opd<int>(-1*offset);

	if(offset != 0) {
		// you got the argument order wrong. the correct order - instr, opd1, opd2, result
		Compute_IC_Stmt* sub_stmt = new Compute_IC_Stmt(sub, stack_opd, offset_opd, stack_opd);
		ic_list.push_back(sub_stmt);
	}

	Control_Flow_IC_Stmt *jump_stmt = new Control_Flow_IC_Stmt(jal, NULL, NULL, this->procedure_name);
	ic_list.push_back(jump_stmt);

	if(offset != 0) {
		Compute_IC_Stmt* add_stmt = new Compute_IC_Stmt(add, stack_opd, offset_opd, stack_opd);
		ic_list.push_back(add_stmt);
	}


	Register_Descriptor* rd;
	Register_Addr_Opd *RAO, *reg_opd;
	Move_IC_Stmt *load;

	if(this->get_data_type() == int_data_type) {
		rd = machine_desc_object.get_new_register<int_reg>();
		RAO = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);
		reg_opd = new Register_Addr_Opd(rd);
		load = new Move_IC_Stmt(mov, RAO, reg_opd);
		ic_list.push_back(load);
	}

	if(this->get_data_type() == double_data_type) {
		rd = machine_desc_object.get_new_register<float_reg>();
		RAO = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);
		reg_opd = new Register_Addr_Opd(rd);
		load = new Move_IC_Stmt(move_d, RAO, reg_opd);
		ic_list.push_back(load);
	}

	Code_For_Ast *output = new Code_For_Ast(ic_list, rd);
	machine_desc_object.clear_local_register_mappings(); /* Need to do this failing which may result in weird issues */

	//*********Don't delete this comment**********
	//I hope I never have to debug someone else's code.
	//I did the thing commented out below, and that caused a segfault
	//because Name_Ast::create_store_stmt only checks for int_reg or float_reg
	//and this was going out as fn_result. It took me 2 hours to find.
	//All I actually had to do was change NULL to rd in Code_For_Ast *output above.

	// if(this->get_data_type() == int_data_type) {
	// 	output->set_reg(machine_desc_object.spim_register_table[v1]);
	// }
	// else if(this->get_data_type() == double_data_type) {
	// 	output->set_reg(machine_desc_object.spim_register_table[f0]);
	// }

	return *output;
}

Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {
	//TODO:
}

Code_For_Ast & Print_Ast::compile() {
	Mem_Addr_Opd *variable  = new Mem_Addr_Opd(this->var->get_symbol_entry());
	Register_Descriptor *rd1;
	Register_Descriptor *rd2;
	Move_IC_Stmt *load1;
	Move_IC_Stmt *load2;
	Register_Addr_Opd *RAO1;
	Register_Addr_Opd *RAO2;
	Const_Opd<int> *constant;
	Print_IC_Stmt *print_stmt = new Print_IC_Stmt();

	if(this->var->get_data_type() == int_data_type) {
		constant = new Const_Opd<int>(1);
		rd1 = new Register_Descriptor(v0, "v0", int_num, int_reg);
		rd2 = new Register_Descriptor(a0, "a0", int_num, argument);
		RAO1 = new Register_Addr_Opd(rd1);
		load1 = new Move_IC_Stmt(imm_load, constant, RAO1);

		RAO2 = new Register_Addr_Opd(rd2);
		load2 = new Move_IC_Stmt(load, variable, RAO2);
	}
	else if(this->var->get_data_type() == double_data_type) {
		constant = new Const_Opd<int>(3);
		rd1 = new Register_Descriptor(v0, "v0", int_num, int_reg);
		rd2 = new Register_Descriptor(f12, "f12", float_num, float_reg);
		RAO1 = new Register_Addr_Opd(rd1);
		load1 = new Move_IC_Stmt(imm_load, constant, RAO1);

		RAO2 = new Register_Addr_Opd(rd2);
		load2 = new Move_IC_Stmt(load_d, variable, RAO2);
	}

    Code_For_Ast *output = new Code_For_Ast();
	output->set_reg(rd1);
	output->append_ics(*load1);
	output->append_ics(*load2);
	output->append_ics(*print_stmt);

	RAO1->get_reg()->reset_use_for_expr_result();
	RAO2->get_reg()->reset_use_for_expr_result();
	return *output;
}