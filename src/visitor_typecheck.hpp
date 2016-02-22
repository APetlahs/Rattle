#ifndef _VISITOR_TYPE_CHECK_
#define _VISITOR_TYPE_CHECK_
#include "type.hpp"
#include "operator.hpp"
#include "node.hpp"
#include "visitor.hpp"

namespace rattle {
namespace visitor {

class TypeCheckVisitor: public Visitor {
    Type *curType;
    Type *returnType;
    CastMap caster;
    MemberTable memberTable;
    bool inFunction;
    bool error;
public:
    TypeCheckVisitor():
        curType(), caster(), memberTable(),
        inFunction(false), error(false) {}
    virtual void visit(ast::IdNode *node);
    virtual void visit(ast::CallNode *node);
    virtual void visit(ast::ArrayNode *node);
    virtual void visit(ast::VarDefNode *node);
    virtual void visit(ast::FuncDefNode *node);
    virtual void visit(ast::AssignNode *node);
    virtual void visit(ast::IntNode *node);
    virtual void visit(ast::FloatNode *node);
    virtual void visit(ast::BoolNode *node);
    virtual void visit(ast::NullNode *node);
    virtual void visit(ast::StringNode *node);
    virtual void visit(ast::BinExprNode *node);
    virtual void visit(ast::UniExprNode *node);
    virtual void visit(ast::LookupNode *node);
    virtual void visit(ast::ReturnNode *node);
    virtual void visit(ast::ForNode *node);
    virtual void visit(ast::WhileNode *node);
};

} // namespace rattle
} // namespace visitor

#endif
