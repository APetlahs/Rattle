#include <iostream>
#include <sstream>
#include <vector>
#include "visitor.hpp"
#include "node.hpp"
using namespace rattle::visitor;
using namespace rattle::ast;

#define VISIT(node) node->accept(this);

void Visitor::visit(ast::ASTNode *node) {
    VISIT(node);
}

void Visitor::visit(ast::IntNode *node) {}

void Visitor::visit(ast::FloatNode *node) {}

void Visitor::visit(ast::BoolNode *node) {}

void Visitor::visit(ast::StringNode *node) {}

void Visitor::visit(ast::IdNode *node) {}

void Visitor::visit(ast::TypeNode *node) {}

void Visitor::visit(ast::TypeListNode *node) {
    for (std::vector<TypeNode*>::iterator i = node->types.begin();
         i != node->types.end(); ++i)
    {
        VISIT((*i));
    }
}

void Visitor::visit(ast::ExprNode *node) {
    VISIT(node);
}

void Visitor::visit(ast::TypedIdNode *node) {
    VISIT(node->type);
}

void Visitor::visit(ast::BinExprNode *node) {
    VISIT(node->lhs);
    VISIT(node->rhs);
}

void Visitor::visit(ast::UniExprNode *node) {
    VISIT(node->expr);
}

void Visitor::visit(ast::StmtNode *node) {
    VISIT(node->stmt);
}

void Visitor::visit(ast::ReturnNode *node) {
    VISIT(node->stmt);
}

void Visitor::visit(ast::BlockNode *node) {
    for (std::vector<StmtNode*>::iterator i = node->stmts.begin();
         i != node->stmts.end(); ++i)
    {
        VISIT((*i));
    }
}

void Visitor::visit(ast::IfNode *node) {
    VISIT(node->cond);
    VISIT(node->body);
}

void Visitor::visit(ast::ElifNode *node) {
    for (std::vector<IfNode*>::iterator i = node->elifStmts.begin();
         i != node->elifStmts.end(); ++i)
    {
        VISIT((*i));
    }
}

void Visitor::visit(ast::IfBlock *node) {
    VISIT(node->ifStmt);
    VISIT(node->elifStmts);
    VISIT(node->elseStmt);
}

void Visitor::visit(ast::ForNode *node) {
    VISIT(node->cond);
    VISIT(node->body);
}

void Visitor::visit(ast::WhileNode *node) {
    VISIT(node->cond);
    VISIT(node->body);
}

void Visitor::visit(ast::ParamsNode *node) {
    for (std::vector<TypedIdNode*>::iterator i = node->args.begin();
         i != node->args.end(); ++i)
    {
        VISIT((*i));
    }
}

void Visitor::visit(ast::ArgsNode *node) {
    for (std::vector<ExprNode*>::iterator i = node->args.begin();
         i != node->args.end(); ++i)
    {
        VISIT((*i));
    }
}

void Visitor::visit(ast::CallNode *node) {
    VISIT(node->args);
}

void Visitor::visit(ast::ArrayNode *node) {
    for (std::vector<ExprNode*>::iterator i = node->args.begin();
         i != node->args.end(); ++i)
    {
        VISIT((*i));
    }
}

void Visitor::visit(ast::FuncDefNode *node) {
    VISIT(node->args);
    VISIT(node->type);
    VISIT(node->body);
}

void Visitor::visit(ast::VarDefNode *node) {
    VISIT(node->typedId);
    VISIT(node->expr);
}

void Visitor::visit(ast::AssignNode *node) {
    VISIT(node->id);
    VISIT(node->expr);
}

void Visitor::visit(ast::ModuleNode *node) {
    VISIT(node->block);
}
