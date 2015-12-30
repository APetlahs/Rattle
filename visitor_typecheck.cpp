#include <iostream>
#include "operator.hpp"
#include "type.hpp"
#include "node.hpp"
#include "visitor_typecheck.hpp"
using namespace rattle;
using namespace rattle::visitor;

void TypeCheckVisitor::visit(ast::IdNode *node) {
    curType = node->type;
}

void TypeCheckVisitor::visit(ast::CallNode *node) {
    std::vector<ast::ExprNode*> args = node->args->args;
    if (node->type->typeClass != Undefined && args.size() != node->type->params.size()) {
        std::cerr << "Invalid number of arguments" << std::endl;
        error = true;
        return;
    } else {
        for(unsigned int i = 0; i < args.size(); ++i) {
            Visitor::visit(args[i]);
            if (!curType->compatible(node->type->params[i])) {
                error = true;
                std::cerr << "incompatible argument type" << std::endl;
            }
        }
    }
    curType = node->type->returnType;
}

void TypeCheckVisitor::visit(ast::VarDefNode *node) {
    Type *t1 = new Type(node->typedId->type);
    Visitor::visit(node->expr);
    Type *t2 = curType;
    if (t2 == NULL) {
        std::cerr << "type is null!" << std::endl;
        error = true;
        return;
    }
    if (!t1->compatible(*t2)) {
        error = true;
        std::cerr << "Type error: " << t1->toStr() << " "
                  << t2->toStr() << " are not compatible in declaration."
                  << std::endl;
    }
    delete t1;
}

void TypeCheckVisitor::visit(ast::AssignNode *node) {
    Type *t1 = node->type;
    Visitor::visit(node);
    Type *t2 = curType;
    if (!t1->compatible(*t2)) {
        error = true;
        std::cerr << "Type error: " << t1->toStr() << " "
                  << t2->toStr() << " are not compatible in assignment."
                  << std::endl;
    }
}

void TypeCheckVisitor::visit(ast::IntNode *node) {
    curType = node->type;
}

void TypeCheckVisitor::visit(ast::FloatNode *node) {
    curType = node->type;
}

void TypeCheckVisitor::visit(ast::BinExprNode *node) {
    Visitor::visit(node->lhs);
    Type *t1 = curType;
    Visitor::visit(node->rhs);
    Type *t2 = curType;
    if (t1 == NULL || t2 == NULL) {
        error = true;
        std::cerr << "types are null!" << std::endl;
        node->type = new Type();
    }
    else if (caster.compatible(node->op, *t1, *t2)) {
        node->type = new Type(caster.cast(node->op,*t1,*t2));
    } else {
        error = true;
        std::cerr << "Type error: " << t1->toStr() << " "
                  << t2->toStr() << " are not compatible."
                  << std::endl;
        node->type = new Type();
    }
    curType = node->type;
}

void TypeCheckVisitor::visit(ast::UniExprNode *node) {
    Visitor::visit(node->expr);
    Type *t = curType;
    if (t == NULL) {
        error = true;
        std::cerr << "type is null!" << std::endl;
        node->type = new Type();
    }
    else if (caster.compatible(node->op, *t)) {
        node->type = new Type(caster.cast(node->op,*t));
    } else {
        error = true;
        std::cerr << "Type error: " << t->toStr();
        node->type = new Type();
    }
    curType = node->type;
}

void TypeCheckVisitor::visit(ast::FuncDefNode *node) {
    bool prevFunctionStatus = inFunction;
    inFunction = true;
    returnType = new Type(node->type);
    Visitor::visit(node->body);
    delete returnType;
    inFunction = prevFunctionStatus;
}

void TypeCheckVisitor::visit(ast::ReturnNode *node) {
    if (!inFunction) {
        error = true;
        std::cerr << "return statement can't occur outside a function!" << std::endl;
        return;
    }
    Visitor::visit(node->stmt);
    if (returnType && !returnType->compatible(*curType)) {
        error = true;
        std::cerr << "return type not compatible" << std::endl;
        return;
    }
}
