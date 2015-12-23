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
    CastMap caster;
    bool error;
public:

    virtual void visit(ast::IdNode *node);
    virtual void visit(ast::CallNode *node);
    virtual void visit(ast::VarDefNode *node);
    virtual void visit(ast::AssignNode *node);
    virtual void visit(ast::IntNode *node);
    virtual void visit(ast::FloatNode *node);
    virtual void visit(ast::StringNode *node);
    virtual void visit(ast::BinExprNode *node);
    virtual void visit(ast::UniExprNode *node);
};

} // namespace rattle
} // namespace visitor

#endif