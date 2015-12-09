#include "symbol.hpp"
using namespace rattle::visitor;

void SymbolTable::add(const Symbol &s) {
    sym[s.id] = s;
}

bool SymbolTable::exists(const std::string &s) {
    return sym.find(s) != sym.end();
}

Symbol SymbolTable::get(const std::string &s) {
    return sym.find(std::string(s))->second;
}