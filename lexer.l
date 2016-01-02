%{

#include <string>
#include <stdio.h>
#include "node.hpp"
#include "operator.hpp"
#include "grammar.tab.h"
#define SAVE_TOKEN yylval.str = new std::string(yytext, yyleng)
#define SAVE_STRING yylval.str = new std::string(yytext+1,yyleng-2)

extern "C" int yywrap() {
        return 1;
}

extern void yyerror(const char *s);

%}

/* nounput to suppress "yyunput defined but not used" warning */
%option nounput

%%

"const"                 return CONST;
"var"                   return VAR;
"func"                  return FUNC;
"return"                return RETURN;
"for"                   return FOR;
"while"                 return WHILE;
"in"                    return IN;
"if"                    return IF;
"elif"                  return ELIF;
"else"                  return ELSE;
[a-zA-Z_][a-zA-Z_0-9]*  { SAVE_TOKEN; return IDENT; }
[0-9]+[.][0-9]+         { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+                  { yylval.ival = atoi(yytext); return INT; }
\"(\\.|[^\\"])*\"       { SAVE_STRING; return STRING; }
"->"                    return RARROW;
"<-"                    return LARROW;
"=="                    return EQ;
"!="                    return NEQ;
">="                    return GTE;
"<="                    return LTE;
[-+*/,:;!()<>\[\]|\{\}\&\%\$\^\=\~]  return *yytext;
([ \t\n\r])             ; /* skip whitespace */
<<EOF>>                 return END_OF_FILE;
.                       yyerror("invalid character");

%%
