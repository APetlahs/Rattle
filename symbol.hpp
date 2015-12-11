#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <map>
#include "type.hpp"

namespace rattle {
namespace visitor {

struct Symbol {
    std::string id;
    Type type;
    Symbol(): id(), type() {}
    Symbol(std::string id, Type t): id(id), type(t) {}
    Symbol(const Symbol &other): id(other.id), type(other.type) {}
    Symbol &operator=(const Symbol &other) {
        id = other.id;
        type = other.type;
        return *this;
    }
};

class SymbolTable {
public:
    std::map<std::string, Symbol> sym;

    SymbolTable(): sym() {}
    void add(const Symbol &s);
    bool exists(const std::string &s);
    Symbol get(const std::string &s);

};

} // namespace visitor
} // namespace rattle

#endif