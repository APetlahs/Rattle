#ifndef _VISITOR_VAR_CHECK_HPP_
#define _VISITOR_VAR_CHECK_HPP_

#include <vector>
#include <map>
#include <string>
#include "visitor.hpp"
#include "type.hpp"
#include "symbol.hpp"

namespace rattle {
namespace visitor {

/*
* visitor for checking varible declarations
* and building a symbol table
*/
class CheckVisitor : public Visitor {
    SymbolTable sym;
    std::vector<SymbolTable> globalSymStack;
    bool error;

    bool wasDefined(const std::string &id);
public:
    CheckVisitor(): sym(), error(false) {}

    virtual void visit(ast::BlockNode *node);
    virtual void visit(ast::IdNode* node);
    virtual void visit(ast::FuncDefNode *node);
    virtual void visit(ast::ForNode *node);
    virtual void visit(ast::CallNode *node);
    virtual void visit(ast::VarDefNode *node);
    virtual void visit(ast::AssignNode *node);
};

}
} // namespace rattle

#endif
