#include "type.hpp"
#include "node.hpp"

using namespace rattle::ast;
using std::vector;

void ElifNode::deleteAll() {
    for (vector<IfNode*>::iterator i = elifStmts.begin();
         i != elifStmts.end(); ++i)
    {
        (*i)->deleteAll();
    }
}

void BlockNode::deleteAll() {
    for (vector<StmtNode*>::iterator i = stmts.begin(); i != stmts.end(); ++i) {
        (*i)->deleteAll();
    }
}

void ParamsNode::deleteAll() {
    for (vector<TypedIdNode*>::iterator i = args.begin(); i != args.end(); ++i) {
        (*i)->deleteAll();
    }
}

void ArgsNode::deleteAll() {
    for (vector<ExprNode*>::iterator i = args.begin(); i != args.end(); ++i) {
        (*i)->deleteAll();
    }
}

void ArrayNode::deleteAll() {
    for (vector<ExprNode*>::iterator i = args.begin(); i != args.end(); ++i) {
        (*i)->deleteAll();
    }
}

void AssignNode::deleteAll() {
    expr->deleteAll();
    delete type;
}