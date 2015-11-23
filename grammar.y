%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "node.hpp"
    using namespace rattle::ast;

    // root node
    ModuleNode *module;

    extern int yylex();
    void yyerror(const char *s) {
        fprintf(stderr, "Error: %s\n", s);
    }
%}

%union {
    rattle::ast::IdNode *id;
    rattle::ast::IntNode *inode;
    rattle::ast::FloatNode *fnode;
    rattle::ast::ExprNode *expr;
    rattle::ast::TypeNode *type;
    rattle::ast::TypedIdNode *typedId;
    rattle::ast::BinExprNode *binexp;
    rattle::ast::UniExprNode *uniexp;
    rattle::ast::StmtNode *stmt;
    rattle::ast::BlockNode *block;
    rattle::ast::ForNode *forblock;
    rattle::ast::WhileNode *whileblock;
    rattle::ast::ParamsNode *params;
    rattle::ast::ArgsNode *args;
    rattle::ast::CallNode *call;
    rattle::ast::FuncDefNode *funcdef;
    rattle::ast::VarDefNode *vardef;
    rattle::ast::AssignNode *assign;
    rattle::ast::ModuleNode *module;
    rattle::ast::Operator op;
    std::string *str;
    double fval;
    long ival;
}

%token CONST VAR FUNC END RETURN
%token FOR WHILE IN IF ELIF ELSE
%token RARROW LARROW EQ GTE LTE NEQ
%token END_OF_FILE

%token <str> IDENT
%token <ival> INT
%token <fval> FLOAT

%type <module> module
%type <block> code_block
%type <stmt> statement
%type <vardef> definition
%type <assign> assignment
%type <funcdef> func_def
%type <whileblock> while_loop
%type <forblock> for_loop
%type <expr> expr terminal
%type <call> func_call
%type <params> parameter_list parameter_loop
%type <args> args_list args_loop
%type <op> bin_operator uni_operator
%type <typedId> typed_var
%type <type> type

%left '&' '|' '~'
%left '!' GTE LTE NEQ EQ '>' '<'
%left '+' '-'
%left '*' '/' '%'
%left '^'

%start module

%%

module:
    code_block END_OF_FILE  { module = new ModuleNode($1); YYACCEPT; }
    | END_OF_FILE           { module = new ModuleNode( new BlockNode() ); YYACCEPT; }
    ;

code_block:
    statement               { $$ = new BlockNode(); $$->push($1); }
    | end_stmt /* empty */  { $$ = new BlockNode(); }
    | code_block statement  { $1->push($2); }
    | code_block end_stmt
    ;

statement:
    expr end_stmt           { $$ = new StmtNode($1); }
    | definition end_stmt   { $$ = new StmtNode($1); }
    | assignment end_stmt   { $$ = new StmtNode($1); }
    | func_def end_stmt     { $$ = new StmtNode($1); }
    | while_loop            { $$ = new StmtNode($1); }
    | for_loop              { $$ = new StmtNode($1); }
  //  | if_elif_else end_stmt { $$ = new StmtNode($1); }
    | RETURN expr end_stmt  { $$ = new StmtNode($2); }
    ;

definition:
    CONST typed_var '=' expr    { $$ = new VarDefNode($2,$4); }
    | VAR typed_var '=' expr    { $$ = new VarDefNode($2,$4); }
    ;

assignment:
    IDENT '=' expr          { $$ = new AssignNode($1,$3); }
    ;

func_def:
    FUNC IDENT '(' parameter_list ')' RARROW type ':'
        code_block
    END FUNC
                            { $$ = new FuncDefNode($2,$4,$7,$9); }
    ;

while_loop:
    WHILE expr ':'
        code_block
    END WHILE
                            { $$ = new WhileNode($2,$4); }
    ;

for_loop:
    FOR IDENT IN expr ':'
        code_block
    END FOR
                            { $$ = new ForNode($2,$4,$6); }
    ;
/*
if_elif_else:
    if_block
    elif_blocks
    else_block
    END IF
    ;

if_block:
    IF expr ':' code_block
    ;

elif_blocks:
    elif_blocks ELIF expr ':' code_block
    |
    ;

else_block:
    ELSE ':' code_block     { $$ = new BlockNode($3); }
    |
    ;
*/
expr:
    terminal                    { $$ = $1; }
    | expr bin_operator expr    { $$ = new BinExprNode($1,$2,$3); }
    | uni_operator expr         { $$ = new UniExprNode($1,$2); }
    | func_call                 { $$ = $1; }
    | '(' expr ')'              { $$ = $2; }
    ;

func_call:
    IDENT '(' args_list ')'     { $$ = new CallNode($1,$3); }
    ;

parameter_list:
    parameter_loop              { $$ = $1; }
    | /* empty */               { $$ = new ParamsNode(); }
    ;

parameter_loop:
    typed_var                       { $$ = new ParamsNode(); $$->push($1); }
    | parameter_loop ',' typed_var  { $1->push($3); $$ = $1; }
    ;

args_list:
    args_loop       { $$ = $1; }
    | /* empty */   { $$ = new ArgsNode(); }
    ;

args_loop:
    expr                    { $$ = new ArgsNode(); $$->push($1); }
    | args_loop ',' expr    { $1->push($3); $$ = $1; }
    ;

bin_operator:
    EQ      { $$ = rattle::ast::EQ; }
    | GTE   { $$ = rattle::ast::GTE; }
    | LTE   { $$ = rattle::ast::LTE; }
    | NEQ   { $$ = rattle::ast::NEQ; }
    | '&'   { $$ = rattle::ast::AND; }
    | '|'   { $$ = rattle::ast::OR; }
    | '>'   { $$ = rattle::ast::GT; }
    | '<'   { $$ = rattle::ast::LT; }
    | '+'   { $$ = rattle::ast::ADD; }
    | '-'   { $$ = rattle::ast::SUB; }
    | '*'   { $$ = rattle::ast::MUL; }
    | '/'   { $$ = rattle::ast::DIV; }
    | '%'   { $$ = rattle::ast::MOD; }
    | '^'   { $$ = rattle::ast::POW; }
    ;

uni_operator:
    '~'     { $$ = rattle::ast::NOT; }
    | '!'   { $$ = rattle::ast::NOT; }
    | '-'   { $$ = rattle::ast::SUB; }
    ;

terminal:
    IDENT   { $$ = new IdNode($1); }
    | INT   { $$ = new IntNode($1); }
    | FLOAT { $$ = new FloatNode($1); }
    ;

typed_var:
    IDENT ':' type  { $$ = new TypedIdNode($1,$3); }
    ;

type:
    IDENT                           { $$ = new TypeNode($1); }
    //| '[' type ']' /* lists */
    //| '(' type_list ')' RARROW type
    //| '(' ')' RARROW type
    ;

//type_list:
//    type
//    | type_list ',' type

end_stmt:
    '\n'
    | ';'
    ;

/*ws:
    '\n'
    | ws '\n'
    ;*/
%%
