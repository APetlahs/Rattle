#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <string>
#include <vector>
#include <map>
#include "visitor.hpp"

namespace rattle {

namespace visitor {
// forward decleration to avoid cyclic dependencies
class Type;
};

namespace ast {

enum Operator { ADD,SUB,MUL,DIV,POW,
                MOD,AND,OR,NOT, LT,
                GT,LTE,GTE,EQ,NEQ };

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
    IdNode(std::string id): id(id) {}
    ACCEPT();
};

class TypeNode: public ASTNode {
public:
    std::string id;
    TypeNode(std::string id): id(id) {}
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

class StmtNode: public ASTNode {
public:
    ASTNode *stmt;
    StmtNode(ASTNode *stmt): stmt(stmt) {}
    virtual void deleteAll() { stmt->deleteAll(); }
    ACCEPT();
};

class BlockNode: public ASTNode {
public:
    std::vector<StmtNode*> stmts;
    // This map stores the non-local symbols available to this code block.
    // It is mutated by Visitors.
    std::map<std::string, TypeNode*> nonLocalVars;
    BlockNode(): stmts(), nonLocalVars() {}
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
        elseStmt->deleteAll();
    }
    ACCEPT();
};

class ForNode: public ASTNode {
public:
    std::string var;
    ExprNode *cond;
    BlockNode *body;
    ForNode(std::string var, ExprNode *cond, BlockNode *body):
        var(var), cond(cond), body(body) {}
    virtual void deleteAll() {
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
    std::string id;
    ArgsNode *args;
    CallNode(std::string id, ArgsNode *args): id(id), args(args) {};
    virtual void deleteAll() { args->deleteAll(); }
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
    std::string id;
    ExprNode *expr;

    AssignNode(std::string id, ExprNode *expr): id(id), expr(expr) {}
    virtual void deleteAll() { expr->deleteAll(); }
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
