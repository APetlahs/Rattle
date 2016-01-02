#ifndef _AST_VISITOR_HPP_
#define _AST_VISITOR_HPP_

namespace rattle {

// forward declarations
namespace ast {
    class ASTNode;
    class ExprNode;
    class IntNode;
    class FloatNode;
    class IdNode;
    class TypeNode;
    class TypedIdNode;
    class BinExprNode;
    class UniExprNode;
    class StmtNode;
    class ReturnNode;
    class BlockNode;
    class IfNode;
    class ElifNode;
    class IfBlock;
    class ForNode;
    class WhileNode;
    class ParamsNode;
    class ArgsNode;
    class CallNode;
    class ArrayNode;
    class FuncDefNode;
    class VarDefNode;
    class AssignNode;
    class ModuleNode;
}

namespace visitor {

/*
* Abstract visitor class
* for traversing the Rattle AST
*/
class Visitor {
public:
    virtual ~Visitor() {}
    virtual void visit(ast::ASTNode *node);
    virtual void visit(ast::ExprNode *node);
    virtual void visit(ast::IntNode *node);
    virtual void visit(ast::FloatNode *node);
    virtual void visit(ast::IdNode *node);
    virtual void visit(ast::TypeNode *node);
    virtual void visit(ast::TypedIdNode *node);
    virtual void visit(ast::BinExprNode *node);
    virtual void visit(ast::UniExprNode *node);
    virtual void visit(ast::StmtNode *node);
    virtual void visit(ast::ReturnNode *node);
    virtual void visit(ast::BlockNode *node);
    virtual void visit(ast::IfNode *node);
    virtual void visit(ast::ElifNode *node);
    virtual void visit(ast::IfBlock *node);
    virtual void visit(ast::ForNode *node);
    virtual void visit(ast::WhileNode *node);
    virtual void visit(ast::ParamsNode *node);
    virtual void visit(ast::ArgsNode *node);
    virtual void visit(ast::CallNode *node);
    virtual void visit(ast::ArrayNode *node);
    virtual void visit(ast::FuncDefNode *node);
    virtual void visit(ast::VarDefNode *node);
    virtual void visit(ast::AssignNode *node);
    virtual void visit(ast::ModuleNode *node);
};

} // namespace visitor
} // namespace rattle

#endif
