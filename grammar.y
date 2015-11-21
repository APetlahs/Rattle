%{
    #include <stdlib.h>
    #include <stdio.h>
    //#include "node.hpp"

    // root node
    //ModuleNode *module;

    extern int yylex();
    void yyerror(const char *s) {
        fprintf(stderr, "Error: %s\n", s);
    }
%}

%union {
}

%token CONST VAR FUNC END RETURN
%token FOR WHILE IN IF ELIF ELSE
%token RARROW LARROW EQ GTE LTE NEQ
%token INT FLOAT IDENT END_OF_FILE

%left '&' '|' '~'
%left '!' GTE LTE NEQ EQ '>' '<'
%left '+' '-'
%left '*' '/' '%'
%left '^'

%start module

%%

module:
    code_block END_OF_FILE     { YYACCEPT; }//{ ModuleNode = new ModuleNode($1); YYACCEPT; }
    | END_OF_FILE              { YYACCEPT; }//{ ModuleNode = new ModuleNode( new BlockNode() ); YYACCEPT; }
    ;

code_block:
    statement               //{ $$ = new BlockNode(); $$->push($1); }
    | end_stmt /* empty stmt*/ //{ $$ = new BlockNode(); }
    | code_block statement  //{ $1->push($2); }
    | code_block end_stmt
    ;

statement:
    expr end_stmt
    | definition end_stmt
    | assignment end_stmt
    | func_def end_stmt
    | while_loop
    | for_loop
    | if_elif_else end_stmt
    | RETURN expr end_stmt
    ;

definition:
    CONST typed_var '=' expr
    | VAR typed_var '=' expr
    ;

assignment:
    IDENT '=' expr
    ;

func_def:
    FUNC IDENT '(' parameter_list ')' RARROW type ':'
        code_block
    END FUNC
    ;

while_loop:
    WHILE expr ':'
        code_block
    END WHILE

for_loop:
    FOR IDENT IN expr ':'
        code_block
    END FOR

if_elif_else:
    IF expr ':' code_block
    elif_block
    else_block
    END IF

elif_block:
    elif_block ELIF expr ':' code_block
    |
    ;

else_block:
    ELSE ':' code_block
    |
    ;

expr:
    terminal
    | math_expr
    | logical_expr
    | boolean_expr
    | func_call
    | '(' expr ')'
    ;

math_expr:
    expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '^' expr
    | expr '/' expr
    | expr '%' expr
    | '-' expr
    ;

logical_expr:
    expr '&' expr
    | expr '|' expr
    | '~' expr
    ;

boolean_expr:
    expr '>' expr
    | expr '<' expr
    | expr GTE expr
    | expr LTE expr
    | expr EQ expr
    | expr NEQ expr
    | '!' expr
    ;

func_call:
    IDENT '(' args_list ')'
    ;

parameter_list:
    parameter_loop
    | /* empty */
    ;

parameter_loop:
    typed_var
    | parameter_loop ',' typed_var
    ;

args_list:
    args_loop
    | /* empty */
    ;

args_loop:
    expr
    | args_loop ',' expr
    ;

terminal:
    IDENT
    | INT
    | FLOAT
    ;

typed_var:
    IDENT ':' type
    ;

type:
    IDENT
    | '[' type ']'
    | '(' type_list ')' RARROW type
    | '(' ')' RARROW type
    ;

type_list:
    type
    | type_list ',' type

end_stmt:
    '\n'
    | ';'
    ;

/*ws:
    '\n'
    | ws '\n'
    ;*/
%%
