#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <string>
#include <vector>
#include "visitor.hpp"

namespace rattle {
namespace ast {

namespace visit = rattle::visitor;

enum Operator { ADD,SUB,MUL,DIV,POW,
                MOD,AND,OR,NOT, LT,
                GT,LTE,GTE,EQ,NEQ };

// accept method for visitor
#define ACCEPT() virtual void accept() {}//virtual void accept(visit::Visitor *v) { v->visit(this); }

class ASTNode {
public:
    virtual ~ASTNode() {}
    // explicit dtor to delete all child nodes
    virtual void deleteAll() = 0;
    ACCEPT();
};

class ExprNode: public ASTNode {
public:
    virtual ~ExprNode() {}
    ACCEPT();
};

class IntNode: public ExprNode {
public:
    long val;
    IntNode(const long val): val(val) {}
    virtual void deleteAll() {}
    ACCEPT();
};

class FloatNode: public ExprNode {
public:
    double val;
    FloatNode(const double val): val(val) {}
    virtual void deleteAll() {}
    ACCEPT();
};

class IdNode: public ExprNode {
public:
    std::string *id;
    IdNode(std::string *id): id(id) {}
    virtual void deleteAll() { delete id; }
    ACCEPT();
};

class TypeNode: public ASTNode {
public:
    std::string *id;
    TypeNode(std::string *id): id(id) {}
    virtual void deleteAll() { delete id; }
    ACCEPT();
};

class TypedIdNode: public ASTNode {
public:
    std::string *id;
    TypeNode *type;
    TypedIdNode( std::string *id, TypeNode *type ): id(id), type(type) {}
    virtual void deleteAll() {
        delete id;
        type->deleteAll();
    }
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
            expr(expr), op(op) {}
    virtual void deleteAll() { expr->deleteAll(); }
    ACCEPT();
};

class StmtNode: public ASTNode {
public:
    virtual void deleteAll() {}
    ACCEPT();
};

class BlockNode: public ASTNode {
public:
    std::vector<StmtNode*> stmts;
    BlockNode(): stmts() {}
    void push(StmtNode *stmt) { stmts.push_back(stmt); }
    virtual void deleteAll();
    ACCEPT();
};

class IfNode: public StmtNode {
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

class IfBlock: public StmtNode {
public:
    std::vector<IfNode*> ifStmts;
    BlockNode *elseStmt;
    IfBlock(IfNode *ifStmt): ifStmts(1,ifStmt), elseStmt(NULL) {}
    void push(IfNode *elifStmt) { ifStmts.push_back(elifStmt); }
    void setElse(BlockNode *elseStmt) { this->elseStmt = elseStmt; }
    virtual void deleteAll();
    ACCEPT();
};

class ForNode: public StmtNode {
public:
    IdNode *var;
    ExprNode *cond;
    BlockNode *body;
    ForNode(IdNode *var, ExprNode *cond, BlockNode *body):
        var(var), cond(cond), body(body) {}
    virtual void deleteAll() {
        var->deleteAll();
        cond->deleteAll();
        body->deleteAll();
    }
    ACCEPT();
};

class WhileNode: public StmtNode {
public:
    ExprNode *cond;
    BlockNode *body;
    WhileNode(ExprNode *cond, BlockNode *body):
        cond(cond), body(body) {}
    virtual void deleteAll() {
        cond->deleteAll();
        body->deleteAll();
    }
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

class CallNode: public ExprNode {
public:
    IdNode *id;
    ArgsNode *args;
    CallNode(IdNode *id, ArgsNode *args): id(id), args(args) {};
    virtual void deleteAll() {
        id->deleteAll();
        args->deleteAll();
    }
    ACCEPT();
};

class DefNode: public ASTNode {
public:
    IdNode *id;
    ParamsNode *args;
    ExprNode *body;

    DefNode(IdNode *id, ParamsNode *args, ExprNode *body):
            id(id), args(args), body(body) {}
    virtual void deleteAll() {
        id->deleteAll();
        args->deleteAll();
        body->deleteAll();
    }
    ACCEPT();
};

class AssignNode: public ASTNode {
public:
    IdNode *id;
    ExprNode *expr;

    AssignNode(IdNode *id, ExprNode *expr): id(id), expr(expr) {}
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