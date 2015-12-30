#ifndef _VISITOR_PRINT_HPP_
#define _VISITOR_PRINT_HPP_

#include "visitor.hpp"

namespace rattle {
namespace visitor {

/*
* class for printing AST tree,
* for use in debugging purposes
*/
class PrintVisitor: public Visitor {
public:
    std::string prefix;
    PrintVisitor(): prefix() {}
    virtual ~PrintVisitor() {}

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
    virtual void visit(ast::FuncDefNode *node);
    virtual void visit(ast::VarDefNode *node);
    virtual void visit(ast::AssignNode *node);
    virtual void visit(ast::ModuleNode *node);

};

} // namespace visitor
} // namespace rattle


#endif