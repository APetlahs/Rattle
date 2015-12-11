#include <vector>
#include <map>
#include <iostream>
#include "node.hpp"
#include "visitor_varcheck.hpp"
using namespace rattle::visitor;

bool CheckVisitor::wasDefined(const std::string &id) {
    if (sym.exists(id)) {
        return true;
    } else {
        for(unsigned int i = globalSymStack.size(); i-- > 0;) {
            if (globalSymStack[i].exists(id)) return true;
        }
    }
    return false;
}

void CheckVisitor::visit(ast::BlockNode *node) {
    globalSymStack.push_back(sym);
    sym = SymbolTable();
    Visitor::visit(node);
    sym = globalSymStack.back();
    globalSymStack.pop_back();
}

void CheckVisitor::visit(ast::VarDefNode *node) {
    if (sym.exists(node->typedId->id)) {
        error = true;
        std::cerr << "variable " << node->typedId->id << " previously defined" << std::endl;
    } else {
        Symbol s = Symbol(node->typedId->id, Type(node->typedId->type));
        sym.add(s);
    }
    Visitor::visit(node);
}

void CheckVisitor::visit(ast::IdNode *node) {
    if (wasDefined(node->id)) {
        return;
    }
    error = true;
    std::cerr << "variable " << node->id << " undeclared" << std::endl;
}

void CheckVisitor::visit(ast::ForNode *node) {
    Visitor::visit(node);
}

void CheckVisitor::visit(ast::FuncDefNode *node) {
    #define params node->args->args
    if (sym.exists(node->id)) {
        error = true;
        std::cerr << "function " << node->id << " previously defined" << std::endl;
    }
    Symbol s = Symbol(node->id, Type(node->type));
    sym.add(s);
    globalSymStack.push_back(sym);
    sym = SymbolTable();
    for (std::vector<ast::TypedIdNode*>::iterator i = params.begin();
         i != params.end(); ++i)
    {
        Symbol s = Symbol((*i)->id, Type((*i)->type));
        sym.add(s);
    }
    Visitor::visit(node);
    sym = globalSymStack.back();
    globalSymStack.pop_back();
}

void CheckVisitor::visit(ast::CallNode *node) {
    if (!wasDefined(node->id)) {
        error = true;
        std::cerr << "function " << node->id << " undeclared" << std::endl;
    }
    Visitor::visit(node);
}

void CheckVisitor::visit(ast::AssignNode *node) {
    if (!wasDefined(node->id)) {
        error = true;
        std::cerr << "variable " << node->id << " undeclared" << std::endl;
    }
    Visitor::visit(node);
}

void CheckVisitor::visit(ast::IntNode *node) {
}

void CheckVisitor::visit(ast::FloatNode *node) {

}

void CheckVisitor::visit(ast::BinExprNode *node) {

}

void CheckVisitor::visit(ast::UniExprNode *node) {

}

