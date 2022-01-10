%{
#include "parser.tab.h"
#include <stdlib.h>
#include <string>
%}

digit [0-9]
operator [-+*/]
whitespaces [\t ]
newline	[\n]
int_number [0-9]+
double_number ({digit}*\.{digit}+|{digit}+\.{digit}*)

int_type int
float_type float
sl_comment \/[\/]+.*
name [a-zA-Z_][a-zA-Z0-9_]*
void_kw void
meta_char [{},;()?:]
char .
assign =

/*control expressions*/
while_kw while
if_kw if
do_kw do
else_kw else

return_kw return

/*relational operators*/
eq ==
lt <
gt >
lte <=
gte >=
neq !=

/*logical operators*/
and &&
or ||
not !

/*print*/
print_kw print

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


 /* control keywords */

{while_kw}	{
				store_token_name("WHILE", yytext, yylineno);
				return WHILE;
			}

{if_kw}		{
				store_token_name("IF", yytext, yylineno);
				return IF;
			}

{do_kw}		{
				store_token_name("DO", yytext, yylineno);
				return DO;
			}
{else_kw}	{
				store_token_name("ELSE", yytext, yylineno);
				return ELSE;
			}

 /*print*/
{print_kw}	{
				store_token_name("PRINT", yytext, yylineno);
				return PRINT;
			}

 /*return*/
{return_kw} {
				store_token_name("RETURN", yytext, yylineno);
				return RETURN;
			}

{name}  	{
				/* cout<<yytext<<endl; */
				store_token_name("NAME", yytext, yylineno);
				yylval.string_value = new string;
				*yylval.string_value = yytext;
				return NAME;
			}
 

 /*relops*/

{eq}		{
				store_token_name("EQUAL", yytext, yylineno);
				return EQUAL;
			}

{lt}		{
				store_token_name("LESS_THAN", yytext, yylineno);
				return LESS_THAN;
			}

{gt}		{
				store_token_name("GREATER_THAN", yytext, yylineno);
				return GREATER_THAN;
			}

{lte}		{
				store_token_name("LESS_THAN_EQUAL", yytext, yylineno);
				return LESS_THAN_EQUAL;
			}

{gte}		{
				store_token_name("GREATER_THAN_EQUAL", yytext, yylineno);
				return GREATER_THAN_EQUAL;
			}

{neq}		{
				store_token_name("NOT_EQUAL", yytext, yylineno);
				return NOT_EQUAL;
			}

 /*logical ops*/
{and}		{
				store_token_name("AND", yytext, yylineno);
				return AND;
			}

"||"		{
				store_token_name("OR", yytext, yylineno);
				return OR;
			}

{not}		{
				store_token_name("NOT", yytext, yylineno);
				return NOT;
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
				
			}

{char}		{
				cerr<<"cs316: Error\n";
				exit(1);
				/* All other characters are ignored */ 
			}

%%