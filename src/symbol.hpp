#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_
#include <map>
#include "type.hpp"

namespace rattle {
namespace visitor {

enum SymbolScope { Global, Closed, Local };

struct Symbol {
    std::string id;
    Type type;
    enum SymbolScope scope;
    Symbol();
    Symbol(std::string id, Type t, enum SymbolScope scope=Local);
    Symbol(const Symbol &other);
    Symbol &operator=(const Symbol &other);
};

class SymbolTable {
public:
    std::map<std::string, Symbol> sym;

    SymbolTable(): sym() {}
    void add(const Symbol &s);
    bool exists(const std::string &s);
    Symbol get(const std::string &s);
    void printAll();
};

} // namespace visitor
} // namespace rattle

#endif