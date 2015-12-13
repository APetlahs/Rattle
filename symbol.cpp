#include "symbol.hpp"
#include "type.hpp"
using namespace rattle::visitor;

Symbol::Symbol(): id(), type() {}

Symbol::Symbol(std::string id, Type t): id(id), type(t) {}

Symbol::Symbol(const Symbol &other): id(other.id), type(other.type) {}

Symbol &Symbol::operator=(const Symbol &other) {
    id = other.id;
    type = other.type;
    return *this;
}

void SymbolTable::add(const Symbol &s) {
    sym[s.id] = s;
}

bool SymbolTable::exists(const std::string &s) {
    return sym.find(s) != sym.end();
}

Symbol SymbolTable::get(const std::string &s) {
    return sym.find(std::string(s))->second;
}