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
            if (!node->type->params[i].compatible(*curType)) {
                error = true;
                std::cerr << "incompatible argument type" << std::endl;
            }
        }
    }
    curType = node->type->returnType;
}

void TypeCheckVisitor::visit(ast::ArrayNode *node) {
    Type *prevType = NULL;
    if (node->args.size() < 1) {
        curType = new Type(EmptyArray);
    } else {
        for (std::vector<ast::ExprNode*>::iterator i = node->args.begin(); i != node->args.end(); ++i)
        {
            Visitor::visit(*i);
            if (prevType == NULL) {
                prevType = curType;
            }
            if (!prevType->compatible(*curType)) {
                if (curType->compatible(*prevType)) {
                    prevType = curType;
                    continue;
                }
                error = true;
                std::cerr << "Type error: array contents must contain compatible type."
                          << std::endl;
                break;
            }
        }
        curType = new Type(Array, prevType);
    }
    node->type = curType;
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
    Visitor::visit(node->id);
    Type *t1 = curType;
    Visitor::visit(node->expr);
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

void TypeCheckVisitor::visit(ast::BoolNode *node) {
    curType = node->type;
}

void TypeCheckVisitor::visit(ast::StringNode *node) {
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
    Type *prevReturnType = returnType;
    returnType = new Type(node->type);
    Visitor::visit(node->body);
    delete returnType;
    returnType = prevReturnType;
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

void TypeCheckVisitor::visit(ast::ForNode *node) {
    Type var = Type(node->var->type);
    Visitor::visit(node->cond);
    Type *expr = curType;
    if (!expr->isIterable()) {
        std::cerr << expr->toStr() << " is not iterable." << std::endl;
        return;
    }
    if (expr->typeClass == EmptyArray) {
        delete expr;
        expr = new Type();
        expr->typeClass = Array;
        expr->returnType = new Type(var);
    }
    if (!var.compatible(expr->getIterator())) {
        error = true;
        std::cerr << "identifer '" << node->var->id << "' with type "
                  << var.toStr() << " is not compatible" << std::endl;
        return;
    }
    Visitor::visit(node->body);
}

void TypeCheckVisitor::visit(ast::WhileNode *node) {
    Visitor::visit(node->cond);
    Type t = Type(Bool);
    if (!t.compatible(*curType)) {
        error = true;
        std::cerr << "While-loop conditions must be a boolean. Got "
                  << curType->toStr() << " instead." << std::endl;
    }
    Visitor::visit(node->body);
}
