#include <iostream>
#include "symbol.hpp"
#include "type.hpp"
using namespace rattle::visitor;

Symbol::Symbol(): id(), type(), scope(Local) {}

Symbol::Symbol(std::string id, Type t, enum SymbolScope scope):
    id(id), type(t), scope(scope) {}

Symbol::Symbol(const Symbol &other):
    id(other.id), type(other.type), scope(other.scope) {}

Symbol &Symbol::operator=(const Symbol &other) {
    id = other.id;
    type = other.type;
    scope = other.scope;
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
        std::cout << i->first << " : ";
        switch (i->second.scope) {
            case Local: std::cout << "local"; break;
            case Closed: std::cout << "closed"; break;
            case Global: std::cout << "global"; break;
        }
        std::cout << std::endl;
    }
}
