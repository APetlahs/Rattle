#include "node.hpp"
#include "visitor.hpp"
#include "grammar.tab.h"
using namespace rattle;

extern ast::ModuleNode *module;
extern int yyparse();

int main(void) {
    rattle::visitor::Visitor v = rattle::visitor::Visitor();
    yyparse();
    if(!module) return 1;
    module->accept(&v);
    module->deleteAll();
    return 0;
}

