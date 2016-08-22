/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CONST = 258,
     VAR = 259,
     FUNC = 260,
     RETURN = 261,
     FOR = 262,
     WHILE = 263,
     IN = 264,
     IF = 265,
     ELIF = 266,
     ELSE = 267,
     RARROW = 268,
     LARROW = 269,
     EQ = 270,
     GTE = 271,
     LTE = 272,
     NEQ = 273,
     TRUE = 274,
     FALSE = 275,
     TNULL = 276,
     END_OF_FILE = 277,
     IDENT = 278,
     STRING = 279,
     INT = 280,
     FLOAT = 281,
     NOT = 282,
     OR = 283,
     AND = 284,
     BMINUS = 285,
     UMINUS = 286
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 18 "grammar.y"

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
    std::string *str;
    double fval;
    long ival;



/* Line 1685 of yacc.c  */
#line 115 "grammar.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


