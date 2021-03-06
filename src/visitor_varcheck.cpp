#include <vector>
#include <map>
#include <iostream>
#include "node.hpp"
#include "visitor_varcheck.hpp"
#include "type.hpp"
using namespace rattle::visitor;

CheckVisitor::CheckVisitor(): error(false) {
    globalSym = SymbolTable();
    sym = &globalSym;
}

void CheckVisitor::pushScope() {
    if (sym != NULL) {
        globalSymStack.push_back(sym);
    }
}

void CheckVisitor::popScope() {
    if (globalSymStack.size() > 0) {
        sym = globalSymStack.back();
        globalSymStack.pop_back();
    }
}

bool CheckVisitor::wasDefined(std::string const &id) {
    if (sym->exists(id)) {
        return true;
    } else {
        for(unsigned int i = globalSymStack.size(); i-- > 0;) {
            if (globalSymStack[i]->exists(id)) return true;
        }
    }
    return false;
}

Symbol CheckVisitor::getSymbol(std::string const &id) {
    if (sym->exists(id)) {
        return sym->get(id);
    } else {
        for(unsigned int i = globalSymStack.size(); i-- > 0;) {
            if (globalSymStack[i]->exists(id)) {
                Symbol s = globalSymStack[i]->get(id);
                s.scope = (i == 1)? Global : Closed;
                return s;
            }
        }
    }
    // TODO: maybe throw something
    return Symbol();
}

void CheckVisitor::visit(ast::BlockNode *node) {
    pushScope();
    sym = node->symbols;
    Visitor::visit(node);
    popScope();
}

void CheckVisitor::visit(ast::VarDefNode *node) {
    if (sym->exists(node->typedId->id)) {
        error = true;
        std::cerr << "variable " << node->typedId->id << " previously defined" << std::endl;
    } else {
        Visitor::visit(node);
        Type t = Type(node->typedId->type);
        if (t.isUndefined()) {
            error = true;
            std::cerr << "Invalid Type" << std::endl;
            return;
        }
        Symbol s = Symbol(node->typedId->id, t);
        sym->add(s);
    }
}

void CheckVisitor::visit(ast::IdNode *node) {
    if (wasDefined(node->id)) {
        node->type = new Type(getSymbol(node->id).type);
        node->scope = getSymbol(node->id).scope;
    } else {
        node->type = new Type();
        error = true;
        std::cerr << "variable " << node->id << " undeclared" << std::endl;
    }
}

void CheckVisitor::visit(ast::ForNode *node) {
    pushScope();
    Symbol s = Symbol(node->var->id, Type(node->var->type));
    sym->add(s);
    Visitor::visit(node);
    popScope();
}

void CheckVisitor::visit(ast::FuncDefNode *node) {
    #define params node->args->args
    if (sym->exists(node->id)) {
        error = true;
        std::cerr << "function " << node->id << " previously defined" << std::endl;
    }
    Symbol s = Symbol(node->id, Type(node));
    sym->add(s);
    pushScope();
    sym = node->body->symbols;
    for (std::vector<ast::TypedIdNode*>::iterator i = params.begin();
         i != params.end(); ++i)
    {
        Symbol s = Symbol((*i)->id, Type((*i)->type));
        sym->add(s);
    }
    Visitor::visit(node);
    popScope();
}

void CheckVisitor::visit(ast::IntNode *node) {
    node->type = new Type(Int);
}

void CheckVisitor::visit(ast::FloatNode *node) {
    node->type = new Type(Float);
}

void CheckVisitor::visit(ast::BoolNode *node) {
    node->type = new Type(Bool);
}

void CheckVisitor::visit(ast::NullNode *node) {
    node->type = new Type(Null);
}

void CheckVisitor::visit(ast::StringNode *node) {
    node->type = new Type(Str);
}

