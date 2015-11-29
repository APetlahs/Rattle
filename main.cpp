#include "node.hpp"
#include "visitor_print.hpp"
#include "grammar.tab.h"
using namespace rattle;

extern ast::ModuleNode *module;
extern int yyparse();

int main(void) {
    rattle::visitor::PrintVisitor v = rattle::visitor::PrintVisitor();
    yyparse();
    if(!module) return 1;
    module->accept(&v);
    module->deleteAll();
    return 0;
}

