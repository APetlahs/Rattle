#include <iostream>
#include "symbol.hpp"
#include "type.hpp"
using namespace rattle::visitor;

Symbol::Symbol(): id(), type(), local(true) {}

Symbol::Symbol(std::string id, Type t, bool local):
    id(id), type(t), local(true) {}

Symbol::Symbol(const Symbol &other): id(other.id), type(other.type) {}

Symbol &Symbol::operator=(const Symbol &other) {
    id = other.id;
    type = other.type;
    local = other.local;
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

void SymbolTable::printAll() {
    for (std::map<std::string, Symbol>::iterator i = sym.begin();
        i != sym.end(); ++i)
    {
        std::cout << i->first << " : "
                  <<(i->second.local ? "local" : "non-local")
                  << std::endl;
    }
}