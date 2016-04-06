#include <iostream>
#include <string>
#include "node.hpp"
#include "type.hpp"
#include "symbol.hpp"
#include "visitor_print.hpp"
using namespace rattle::visitor;

// visit child, while also increasing the print indentation
#define VISIT(node) std::string oldPrefix = prefix; \
                    prefix.append(" "); \
                    Visitor::visit(node); \
                    prefix = oldPrefix;

std::string printType(const std::string prefix, Type *t) {
    if (t != NULL) return prefix + "Type: " + t->toStr();
    return prefix + "Type: no type!";
}

void PrintVisitor::visit(ast::IntNode *node) {
    std::cout << prefix << "IntNode: " << node << std::endl;
    std::cout << prefix << node->val << std::endl;
    std::cout << printType(prefix, node->type) << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::FloatNode *node) {
    std::cout << prefix << "FloatNode: " << node << std::endl;
    std::cout << prefix << node->val << std::endl;
    std::cout << printType(prefix, node->type) << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::BoolNode *node) {
    std::cout << prefix << "BoolNode: " << node << std::endl;
    std::cout << prefix << (node->val ? "True" : "False") << std::endl;
    std::cout << printType(prefix, node->type) << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::StringNode *node) {
    std::cout << prefix << "StringNode: " << node << std::endl;
    std::cout << prefix << node->val << std::endl;
}

void PrintVisitor::visit(ast::NullNode *node) {
    std::cout << prefix << "NullNode: " << node << std::endl;
}

void PrintVisitor::visit(ast::ArrayNode *node) {
    std::cout << prefix << "ArrayNode: " << node << std::endl;
    std::cout << prefix << node->type->toStr() << std::endl;
    std::cout << printType(prefix, node->type) << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::IdNode *node) {
    std::cout << prefix << "IdNode: " << node << std::endl;
    std::cout << prefix << node->id << std::endl;
    std::cout << printType(prefix, node->type) << std::endl;
    enum SymbolScope scope = (enum SymbolScope) node->scope;
    std::cout << prefix << "scope: ";
    switch (scope) {
        case Local: std::cout << "Local"; break;
        case Closed: std::cout << "Closed"; break;
        case Global: std::cout << "Global"; break; 
    }
    std::cout << std::endl;                 
    VISIT(node);
}

void PrintVisitor::visit(ast::TypeNode *node) {
    Type t(node);
    std::cout << prefix << "TypeNode: " << node << std::endl;
    std::cout << prefix << node->id << std::endl;
    std::cout << printType(prefix, &t) << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::TypeListNode *node) {
    std::cout << prefix << "TypeListNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::TypedIdNode *node) {
    std::cout << prefix << "TypedIdNode: " << node << std::endl;
    std::cout << prefix << node->id << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::BinExprNode *node) {
    std::cout << prefix << "BinExprNode: " << node << std::endl;
    std::cout << prefix << "operator: " << node->op << std::endl;
    std::cout << printType(prefix, node->type) << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::UniExprNode *node) {
    std::cout << prefix << "UniExprNode: " << node << std::endl;
    std::cout << prefix << "operator: " << node->op << std::endl;
    std::cout << printType(prefix, node->type) << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::LookupNode *node) {
    std::cout << prefix << "LookupNode: " << node << std::endl;
    std::cout << prefix << "member: " << node->member << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::StmtNode *node) {
    std::cout << prefix << "StmtNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::ReturnNode *node) {
    std::cout << prefix << "ReturnNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::BlockNode *node) {
    std::cout << prefix << "BlockNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::IfNode *node) {
    std::cout << prefix << "IfNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::ElifNode *node) {
    std::cout << prefix << "ElifNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::IfBlock *node) {
    std::cout << prefix << "IfBlock: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::ForNode *node) {
    std::cout << prefix << "ForNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::WhileNode *node) {
    std::cout << prefix << "WhileNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::ParamsNode *node) {
    std::cout << prefix << "ParamsNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::ArgsNode *node) {
    std::cout << prefix << "ArgsNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::CallNode *node) {
    std::cout << prefix << "CallNode: " << node << std::endl;
    std::cout << printType(prefix, node->type) << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::FuncDefNode *node) {
    std::cout << prefix << "FuncDefNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::VarDefNode *node) {
    std::cout << prefix << "VarDefNode: " << node << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::AssignNode *node) {
    std::cout << prefix << "AssignNode: " << node << std::endl;
    std::cout << prefix << node->id << std::endl;
    VISIT(node);
}

void PrintVisitor::visit(ast::ModuleNode *node) {
    std::cout << prefix << "ModuleNode: " << node << std::endl;
    VISIT(node);
}
