#include "node.hpp"

using namespace rattle::ast;
using std::vector;

void IfBlock::deleteAll() {
    for (vector<IfNode*>::iterator i = ifStmts.begin();
         i != ifStmts.end(); ++i)
    {
        (*i)->deleteAll();
    }
    if(elseStmt) elseStmt->deleteAll();
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