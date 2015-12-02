#ifndef _VISITOR_CHECK_HPP_
#define _VISITOR_CHECK_HPP_

#include <vector>
#include <map>
#include <string>
#include "visitor.hpp"

namespace rattle {
namespace visitor {

/*
* visitor for checking varible declarations
*/
class CheckVisitor : public Visitor {
    std::map<std::string, ast::TypeNode*> sym;
    std::map<std::string, ast::TypeNode*> globalSym;
    bool error;

    bool varExists(std::string &var, std::map<std::string, ast::TypeNode*> dict);

public:
    CheckVisitor(): sym(), globalSym(), error(false) {}

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