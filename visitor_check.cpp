#include <vector>
#include <map>
#include <iostream>
#include "node.hpp"
#include "visitor_check.hpp"
using namespace rattle::visitor;

bool CheckVisitor::varExists(std::string &var, std::map<std::string, ast::TypeNode*> dict) {
    return !(dict.find(var) == dict.end());
}

void printMap(std::map<std::string, rattle::ast::TypeNode*> sym) {
    for(std::map<std::string, rattle::ast::TypeNode*>::iterator it = sym.begin();
        it != sym.end(); ++it)
    {
        std::cout << it->first << ":" << *(it->second->id) << std::endl;
    }
}

void CheckVisitor::visit(ast::BlockNode *node) {
    // swap vars
    std::map<std::string, ast::TypeNode*> tempSym = sym;
    std::map<std::string, ast::TypeNode*> tempGlobalSym = globalSym;
    sym.insert(globalSym.begin(), globalSym.end());
    globalSym = sym;
    sym.clear();

    node->nonLocalVars = globalSym;
    Visitor::visit(node);

    // return symbols to previous state
    sym = tempSym;
    globalSym = tempGlobalSym;
}

void CheckVisitor::visit(ast::VarDefNode *node) {
    if (varExists(*(node->id->id), sym)) {
        error = true;
        std::cerr << "variable " << *(node->id->id) << " previously defined" << std::endl;
    } else {
        sym[*(node->id->id)] = node->id->type;
    }
    Visitor::visit(node);
}

void CheckVisitor::visit(ast::IdNode *node) {
    if (varExists(*(node->id), sym) || varExists(*(node->id), globalSym)) {
        return;
    }
    error = true;
    std::cerr << "variable " << *(node->id) << " undeclared" << std::endl;
}

void CheckVisitor::visit(ast::ForNode *node) {
    Visitor::visit(node);
}

void CheckVisitor::visit(ast::FuncDefNode *node) {
    #define params node->args->args
    if (varExists(*(node->id), sym)) {
        error = true;
        std::cerr << "function " << *(node->id) << " previously defined" << std::endl;
    }
    sym[*(node->id)] = node->type;
    std::map<std::string, ast::TypeNode*> tempSym = sym;
    for (std::vector<ast::TypedIdNode*>::iterator i = params.begin();
         i != params.end(); ++i)
    {
        sym.erase(*((*i)->id));
        sym[*((*i)->id)] = (*i)->type;
    }
    Visitor::visit(node);
    sym = tempSym;
}

void CheckVisitor::visit(ast::CallNode *node) {
    if (!(varExists(*(node->id), sym) || varExists(*(node->id), globalSym))) {
        error = true;
        std::cerr << "variable " << *(node->id) << " undeclared" << std::endl;
    }
    Visitor::visit(node);
}

void CheckVisitor::visit(ast::AssignNode *node) {
    if (!(varExists(*(node->id), sym) || varExists(*(node->id), globalSym))) {
        error = true;
        std::cerr << "variable " << *(node->id) << " undeclared" << std::endl;
    }
    Visitor::visit(node);
}