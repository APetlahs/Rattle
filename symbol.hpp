#ifndef _SYMBOL_HPP_
#define _SYMBOL_HPP_

#include <map>
#include "type.hpp"

namespace rattle {
namespace visitor {

struct Symbol {
    std::string id;
    Type type;
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