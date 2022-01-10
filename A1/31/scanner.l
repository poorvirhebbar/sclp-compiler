%{
#include "parser.tab.h"
#include "storetokens.hh"
#include <stdlib.h>
#include <string>
%}

digit [0-9]
operator [-+*/]
whitespaces [\t ]
newline	[\n]
int_number [0-9]+
double_number {digit}+\.{digit}+
int_type int
float_type float
sl_comment \/[\/]+.*
name [a-zA-Z_][a-zA-Z0-9_]*
void_kw void
meta_char [{},;()]
char .
assign =

%%

{whitespaces} {}


{int_number}  { 
				/* cout<<yytext<<endl; */
				store_token_name("NUM",yytext,yylineno); 
				yylval.integer_value = atoi(yytext);
				return INTEGER_NUMBER;
			}

{double_number} { 
				/* cout<<yytext<<endl; */
		    	store_token_name("FNUM",yytext,yylineno);
				yylval.double_value = atof(yytext);
				return DOUBLE_NUMBER;
			}

{int_type}	{
				/* cout<<yytext<<endl; */
				store_token_name("INTEGER",yytext,yylineno); 
			   	return INTEGER;
			}

{float_type}	{
				/* cout<<yytext<<endl; */
				store_token_name("FLOAT",yytext,yylineno); 
			   	return FLOAT;
			}

{sl_comment}	{}

{assign}	{
				/* cout<<yytext<<endl; */
				store_token_name("ASSIGN_OP", yytext, yylineno);
				return ASSIGN;
			}

{void_kw} 	{ 
				/* cout<<yytext<<endl; */
				store_token_name("VOID",yytext,yylineno); 
			   	return VOID;
			}

{name}  	{
				/* cout<<yytext<<endl; */
				store_token_name("NAME", yytext, yylineno);
				yylval.string_value = new string;
				*yylval.string_value = yytext;
				return NAME;
			}

{operator} 	{
				/* cout<<yytext<<endl; */
				store_token_name("ARITHOP",yytext,yylineno);
				return yytext[0];
			}

{meta_char}		{
				/* cout<<yytext<<endl; */
				store_token_name("META CHAR",yytext,yylineno);
				return yytext[0];
			}

{newline} 	{
				yylineno++;
			}

{char}		{
				cout<<"Error\n";
				/* All other characters are ignored */ 
			}

%%