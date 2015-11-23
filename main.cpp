#include "node.hpp"
#include "grammar.tab.h"

extern int yyparse();

int main(void) {
    yyparse();
    return 0;
}

