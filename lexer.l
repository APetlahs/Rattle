%{

#include <string>
#include <stdio.h>
#include "node.hpp"
#include "grammar.tab.h"
#define SAVE_TOKEN yylval.str = new std::string(yytext, yyleng)

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
"end"                   return END;
"return"                return RETURN;
"for"                   return FOR;
"while"                 return WHILE;
"in"                    return IN;
"if"                    return IF;
"elif"                  return ELIF;
"else"                  return ELSE;
[a-zA-Z_][a-zA-Z_0-9]*  { SAVE_TOKEN; return IDENT; }
[0-9]+[.][0-9]+         /*yylval.num = atof(yytext);*/ return FLOAT;
[0-9]+                  /*yylval.num = atoi(yytext);*/ return INT;
"->"                    return RARROW;
"<-"                    return LARROW;
"=="                    return EQ;
"!="                    return NEQ;
">="                    return GTE;
"<="                    return LTE;
[-+*/,:;!()<>\[\]|\{\}\&\%\$\^\=\n\~]  return *yytext;
([ \t]|[:blank:])       ; /* skip whitespace */
<<EOF>>                 return END_OF_FILE;
.                       yyerror("invalid character");

%%