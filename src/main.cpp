#include "node.hpp"
#include "visitor_print.hpp"
#include "visitor_varcheck.hpp"
#include "visitor_typecheck.hpp"
#include "grammar.tab.h"
using namespace rattle;

extern ast::ModuleNode *module;
extern int yyparse();

int main(void) {
    //rattle::visitor::PrintVisitor printer = rattle::visitor::PrintVisitor();
    rattle::visitor::CheckVisitor checker1 = rattle::visitor::CheckVisitor();
    rattle::visitor::TypeCheckVisitor checker2 = rattle::visitor::TypeCheckVisitor();
    yyparse();
    if(!module) return 1;
    module->accept(&checker1);
    module->accept(&checker2);
    //module->accept(&printer);
    module->deleteAll();
    return 0;
}
