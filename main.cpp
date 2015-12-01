#include "node.hpp"
#include "visitor_print.hpp"
#include "visitor_check.hpp"
#include "grammar.tab.h"
using namespace rattle;

extern ast::ModuleNode *module;
extern int yyparse();

int main(void) {
    rattle::visitor::PrintVisitor printer = rattle::visitor::PrintVisitor();
    rattle::visitor::CheckVisitor checker = rattle::visitor::CheckVisitor();
    yyparse();
    if(!module) return 1;
    module->accept(&printer);
    module->accept(&checker);
    module->deleteAll();
    return 0;
}
