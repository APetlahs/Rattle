#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "operator.hpp"
#include "visitor.hpp"

namespace rattle {

namespace visitor {
// forward decleration to avoid cyclic dependencies
class Type;
class SymbolTable;
};

namespace ast {

// accept method for visitor
#define ACCEPT() virtual void accept(rattle::visitor::Visitor *v) { v->visit(this); }

class ASTNode {
public:
    virtual ~ASTNode() {}
    // explicit dtor to delete all child nodes
    virtual void deleteAll() {};
    ACCEPT();
};

class ExprNode: public ASTNode {
public:
    // type annotation for type-checking pass
    visitor::Type *type;
    virtual ~ExprNode() {}
    ACCEPT();
};

class IntNode: public ExprNode {
public:
    long val;
    IntNode(const long val): val(val) {}
    ACCEPT();
};

class FloatNode: public ExprNode {
public:
    double val;
    FloatNode(const double val): val(val) {}
    ACCEPT();
};

class IdNode: public ExprNode {
public:
    std::string id;
    unsigned int scope; // annotated by var_check
    IdNode(std::string id): id(id) {}
    ACCEPT();
};

class BoolNode: public ExprNode {
public:
    bool val;
    BoolNode(const bool val): val(val) {}
    ACCEPT();
};

class NullNode: public ExprNode {
public:
    NullNode() {}
    ACCEPT();
};

class StringNode: public ExprNode {
public:
    std::string val;
    StringNode(std::string val): val(val) {}
    ACCEPT();
};

// forward declaration
class TypeListNode;

class TypeNode: public ASTNode {
public:
    std::string id;
    TypeNode *subType;
    TypeListNode *params;
    bool callable;
    bool isNull;
    TypeNode():
        subType(NULL), callable(false), isNull(true) {}
    TypeNode(std::string id):
        id(id), subType(NULL), callable(false), isNull(false) {}
    TypeNode(TypeNode* type):
        subType(type), callable(false), isNull(false) {}
    TypeNode(TypeListNode *params, TypeNode *rtype):
        subType(rtype), params(params), callable(true), isNull(false) {}
    ACCEPT();
};

class TypeListNode: public ASTNode {
public:
    std::vector<TypeNode*> types;
    TypeListNode(): types() {}
    void push(TypeNode *type) { types.push_back(type); }
    ACCEPT();
};

class TypedIdNode: public ASTNode {
public:
    std::string id;
    TypeNode *type;
    TypedIdNode( std::string id, TypeNode *type ): id(id), type(type) {}
    virtual void deleteAll() { type->deleteAll(); }
    ACCEPT();
};

class BinExprNode: public ExprNode {
public:
    ExprNode *lhs;
    Operator op;
    ExprNode *rhs;
    BinExprNode( ExprNode *lhs, Operator op, ExprNode *rhs ):
            lhs(lhs), op(op), rhs(rhs) {}
    virtual void deleteAll() {
        lhs->deleteAll();
        rhs->deleteAll();
    }
    ACCEPT();
};

class UniExprNode: public ExprNode {
public:
    Operator op;
    ExprNode *expr;
    UniExprNode( Operator op, ExprNode *expr ):
            op(op), expr(expr) {}
    virtual void deleteAll() { expr->deleteAll(); }
    ACCEPT();
};

class LookupNode: public ExprNode {
public:
    ExprNode *expr;
    std::string member;
    LookupNode(ExprNode *expr, std::string member): expr(expr), member(member) {}
    virtual void deleteAll() { expr->deleteAll(); }
    ACCEPT();
};

class StmtNode: public ASTNode {
public:
    ASTNode *stmt;
    StmtNode(ASTNode *stmt): stmt(stmt) {}
    virtual void deleteAll() { stmt->deleteAll(); }
    ACCEPT();
};

class ReturnNode: public StmtNode {
public:
    ReturnNode(ExprNode *expr): StmtNode::StmtNode(expr) {}
    ACCEPT();
};

class BlockNode: public ASTNode {
public:
    std::vector<StmtNode*> stmts;
    // This map stores symbols used in this code block.
    // it is initialized here but modifed by visitors.
    visitor::SymbolTable *symbols;
    BlockNode();
    ~BlockNode();
    void push(StmtNode *stmt) { stmts.push_back(stmt); }
    virtual void deleteAll();
    ACCEPT();
};

class IfNode: public ASTNode {
public:
    ExprNode *cond;
    BlockNode *body;
    IfNode(ExprNode *cond, BlockNode *body): cond(cond), body(body) {}
    virtual void deleteAll() {
        cond->deleteAll();
        body->deleteAll();
    }
    ACCEPT();
};

class ElifNode: public ASTNode {
public:
    std::vector<IfNode*> elifStmts;
    ElifNode(): elifStmts() {}
    void push(IfNode *ifStmt) { elifStmts.push_back(ifStmt); }
    virtual void deleteAll();
    ACCEPT();
};

class IfBlock: public ASTNode {
public:
    IfNode *ifStmt;
    ElifNode *elifStmts;
    BlockNode *elseStmt;
    IfBlock(IfNode *ifStmt, ElifNode *elifStmts, BlockNode *elseStmt):
        ifStmt(ifStmt), elifStmts(elifStmts), elseStmt(elseStmt) {}
    virtual void deleteAll() {
        ifStmt->deleteAll();
        elifStmts->deleteAll();
        if (elseStmt) {
            elseStmt->deleteAll();
        }
    }
    ACCEPT();
};

class ForNode: public ASTNode {
public:
    TypedIdNode *var;
    ExprNode *cond;
    BlockNode *body;
    ForNode(TypedIdNode *var, ExprNode *cond, BlockNode *body):
        var(var), cond(cond), body(body) {}
    virtual void deleteAll() {
        var->deleteAll();
        cond->deleteAll();
        body->deleteAll();
    }
    ACCEPT();
};

class WhileNode: public ASTNode {
public:
    ExprNode *cond;
    BlockNode *body;
    WhileNode(ExprNode *cond, BlockNode *body):
        cond(cond), body(body) {}
    virtual void deleteAll() {
        cond->deleteAll();
        body->deleteAll();
    }
    ACCEPT();
};

class ParamsNode: public ASTNode {
public:
    std::vector<TypedIdNode*> args;
    ParamsNode(): args() {}
    void push(TypedIdNode *node) { args.push_back(node); }
    virtual void deleteAll();
    ACCEPT();
};

class ArgsNode: public ASTNode {
public:
    std::vector<ExprNode*> args;
    ArgsNode(): args() {}
    void push(ExprNode *node) { args.push_back(node); }
    virtual void deleteAll();
    ACCEPT();
};

class ArrayNode: public ExprNode {
public:
    std::vector<ExprNode*> args;
    ArrayNode(ArgsNode *args): args(args->args) {}
    virtual void deleteAll();
    ACCEPT();
};

class CallNode: public ExprNode {
public:
    ExprNode *func;
    ArgsNode *args;
    CallNode(ExprNode *func, ArgsNode *args): func(func), args(args) {};
    virtual void deleteAll() {
        func->deleteAll();
        args->deleteAll();
    }
    ACCEPT();
};

class FuncDefNode: public ASTNode {
public:
    std::string id;
    ParamsNode *args;
    TypeNode *type;
    BlockNode *body;

    FuncDefNode(std::string id, ParamsNode *args,
                TypeNode *type, BlockNode *body):
            id(id), args(args), type(type), body(body) {}
    virtual void deleteAll() {
        args->deleteAll();
        body->deleteAll();
    }
    ACCEPT();
};

class VarDefNode: public ASTNode {
public:
    TypedIdNode *typedId;
    ExprNode *expr;

    VarDefNode(TypedIdNode *typedId, ExprNode *expr): typedId(typedId), expr(expr) {}
    virtual void deleteAll() {
        typedId->deleteAll();
        expr->deleteAll();
    }
    ACCEPT();
};

class AssignNode: public ASTNode {
public:
    ExprNode *id;
    ExprNode *expr;
    AssignNode(ExprNode* id, ExprNode *expr): id(id), expr(expr) {}
    virtual void deleteAll() {
        id->deleteAll();
        expr->deleteAll();
    }
    ACCEPT();
};

// enclosing class
class ModuleNode: public ASTNode {
public:
    BlockNode *block;
    ModuleNode(BlockNode *block): block(block) {}
    virtual void deleteAll() { block->deleteAll(); }
    ACCEPT();
};

} // namespace ast
} // namespace rattle

#endif
