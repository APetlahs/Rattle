%{
    #include <stdlib.h>
    #include <stdio.h>
    #include "node.hpp"
    #include "operator.hpp"
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
    rattle::ast::ArrayNode *arrnode;
    rattle::ast::ExprNode *expr;
    rattle::ast::TypeNode *type;
    rattle::ast::TypeListNode *typelist;
    rattle::ast::TypedIdNode *typedId;
    rattle::ast::BinExprNode *binexp;
    rattle::ast::UniExprNode *uniexp;
    rattle::ast::StmtNode *stmt;
    rattle::ast::ReturnNode *returnStmt;
    rattle::ast::BlockNode *block;
    rattle::ast::ForNode *forblock;
    rattle::ast::WhileNode *whileblock;
    rattle::ast::IfNode *ifnode;
    rattle::ast::ElifNode *elifnode;
    rattle::ast::IfBlock *ifblock;
    rattle::ast::ParamsNode *params;
    rattle::ast::ArgsNode *args;
    rattle::ast::CallNode *call;
    rattle::ast::FuncDefNode *funcdef;
    rattle::ast::VarDefNode *vardef;
    rattle::ast::AssignNode *assign;
    rattle::ast::ModuleNode *module;
    rattle::Operator op;
    std::string *str;
    double fval;
    long ival;
}

%token CONST VAR FUNC RETURN
%token FOR WHILE IN IF ELIF ELSE
%token RARROW LARROW EQ GTE LTE NEQ
%token TRUE FALSE END_OF_FILE

%token <str> IDENT STRING
%token <ival> INT
%token <fval> FLOAT

%type <module> module
%type <block> code_block else_block statements
%type <stmt> statement
%type <vardef> definition
%type <assign> assignment
%type <funcdef> func_def
%type <whileblock> while_loop
%type <forblock> for_loop
%type <ifblock> if_elif_else
%type <ifnode> if_block
%type <elifnode> elif_blocks
%type <expr> expr literal_expr uni_expr bin_expr simple_expr lhs_expression array_access_expr dot_lookup_expr
%type <call> func_call_expr
%type <params> parameter_list parameter_loop
%type <args> args_list args_loop
%type <op> bin_operator uni_operator
%type <typedId> typed_var
%type <type> type
%type <typelist> type_list

%left AND OR NOT
%left '!' GTE LTE NEQ EQ '>' '<'
%left '+' '-'
%left '*' '/' '%'
%left '^'
%left BINARY
%left UNARY


%start module

%%

module:
    statements END_OF_FILE  { module = new ModuleNode($1); YYACCEPT; }
    | END_OF_FILE           { module = new ModuleNode( new BlockNode() ); YYACCEPT; }
    ;

code_block:
    '{' statements '}'      { $$ = $2; }
    | '{' '}'               { $$ = new BlockNode(); }
    ;

statements:
    statement               { $$ = new BlockNode(); $$->push($1); }
    | end_stmt /* empty */  { $$ = new BlockNode(); }
    | statements statement  { $$ = $1; $$->push($2); }
    | statements end_stmt   { $$ = $1; }
    ;

statement:
    expr end_stmt           { $$ = new StmtNode($1); }
    | definition end_stmt   { $$ = new StmtNode($1); }
    | assignment end_stmt   { $$ = new StmtNode($1); }
    | func_def              { $$ = new StmtNode($1); }
    | while_loop            { $$ = new StmtNode($1); }
    | for_loop              { $$ = new StmtNode($1); }
    | if_elif_else          { $$ = new StmtNode($1); }
    | RETURN expr end_stmt  { $$ = new ReturnNode($2); }
    ;

definition:
    CONST typed_var '=' expr    { $$ = new VarDefNode($2,$4); }
    | VAR typed_var '=' expr    { $$ = new VarDefNode($2,$4); }
    ;

func_def:
    FUNC IDENT '(' parameter_list ')' RARROW type
        code_block
                            { $$ = new FuncDefNode(*$2,$4,$7,$8); delete $2; }
    ;

while_loop:
    WHILE expr
        code_block
                            { $$ = new WhileNode($2,$3); }
    ;

for_loop:
    FOR typed_var IN expr
        code_block
                            { $$ = new ForNode($2,$4,$5); }
    ;

if_elif_else:
    if_block
    elif_blocks
    else_block
                            { $$ = new IfBlock($1,$2,$3); }
    ;

if_block:
    IF expr
        code_block
                            { $$ = new IfNode($2,$3); }
    ;

elif_blocks:
    elif_blocks ELIF expr
        code_block
                            { $1->push( new IfNode($3,$4) ); }
    |                       { $$ = new ElifNode(); }
    ;

else_block:
    ELSE code_block         { $$ = $2; }
    |                       { $$ = NULL; }
    ;

assignment:
    lhs_expression '=' expr     { $$ = new AssignNode($1,$3); }
    ;

expr:
    simple_expr             { $$ = $1; }
    | uni_expr              { $$ = $1; }
    | bin_expr              { $$ = $1; }
    ;

lhs_expression:
    IDENT                   { $$ = new IdNode(*$1); delete $1; }
    | array_access_expr     { $$ = $1; }
  //| dotted_name
    ;

array_access_expr:
    simple_expr '[' expr ']'    { $$ = new BinExprNode($1, rattle::ACCESS, $3); }
    ;

simple_expr:
    literal_expr            { $$ = $1; }
    | func_call_expr        { $$ = $1; }
    | array_access_expr     { $$ = $1; }
    | '(' expr ')'          { $$ = $2; }
    | dot_lookup_expr       { $$ = $1; }
    ;

uni_expr:
    uni_operator expr
    %prec UNARY             { $$ = new UniExprNode($1,$2); }
    ;

bin_expr:
    expr bin_operator expr
    %prec BINARY            { $$ = new BinExprNode($1,$2,$3); }
    ;

func_call_expr:
    IDENT '(' args_list ')' { $$ = new CallNode(*$1,$3); delete $1; }
    ;

dot_lookup_expr:
    simple_expr '.' IDENT   { $$ = new LookupNode($1,*$3); delete $3; }
    ;

parameter_list:
    parameter_loop          { $$ = $1; }
    | /* empty */           { $$ = new ParamsNode(); }
    ;

parameter_loop:
    typed_var                       { $$ = new ParamsNode(); $$->push($1); }
    | parameter_loop ',' typed_var  { $1->push($3); $$ = $1; }
    ;

args_list:
    args_loop               { $$ = $1; }
    | /* empty */           { $$ = new ArgsNode(); }
    ;

args_loop:
    expr                    { $$ = new ArgsNode(); $$->push($1); }
    | args_loop ',' expr    { $1->push($3); $$ = $1; }
    ;

bin_operator:
      EQ                    { $$ = rattle::EQ; }
    | GTE                   { $$ = rattle::GTE; }
    | LTE                   { $$ = rattle::LTE; }
    | NEQ                   { $$ = rattle::NEQ; }
    | AND                   { $$ = rattle::AND; }
    | OR                    { $$ = rattle::OR; }
    | '>'                   { $$ = rattle::GT; }
    | '<'                   { $$ = rattle::LT; }
    | '+'                   { $$ = rattle::ADD; }
    | '-'                   { $$ = rattle::SUB; }
    | '*'                   { $$ = rattle::MUL; }
    | '/'                   { $$ = rattle::DIV; }
    | '%'                   { $$ = rattle::MOD; }
    | '^'                   { $$ = rattle::POW; }
    ;

uni_operator:
    NOT                     { $$ = rattle::NOT; }
    | '!'                   { $$ = rattle::NOT; }
    | '-'                   { $$ = rattle::SUB; }
    ;

literal_expr:
    IDENT                   { $$ = new IdNode(*$1); delete $1; }
    | INT                   { $$ = new IntNode($1); }
    | FLOAT                 { $$ = new FloatNode($1); }
    | TRUE                  { $$ = new BoolNode(true); }
    | FALSE                 { $$ = new BoolNode(false); }
    | STRING                { $$ = new StringNode(*$1); delete $1; }
    | '[' args_list ']'     { $$ = new ArrayNode($2); }
    ;

typed_var:
    IDENT ':' type          { $$ = new TypedIdNode(*$1,$3); delete $1; }
    ;

type:
    IDENT                               { $$ = new TypeNode(*$1); delete $1; }
    | '[' type ']'                      { $$ = new TypeNode($2); }
    | '(' type_list ')' RARROW type     { $$ = new TypeNode($2, $5); }
    | '(' ')' RARROW type               { $$ = new TypeNode(new TypeListNode(), $4); }
    ;

type_list:
    type                    { $$ = new TypeListNode(); $$->push($1); }
    | type_list ',' type    { $$ = $1; $$->push($3); }
    ;

end_stmt:
    ';'
    ;
%%
