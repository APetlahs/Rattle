#ifndef _OPERATOR_HPP_
#define _OPERATOR_HPP_
/*
* defines the rattle operators and type casting tables
*/

namespace rattle {
namespace visitor {
// forward declare
class Type;
}; // namespace visitor

enum Operator { ADD,SUB,MUL,DIV,POW,
                MOD,AND,OR,NOT,LT,
                GT,LTE,GTE,EQ,NEQ,
                ACCESS };

class CastMap {
public:
    bool compatible(const Operator op, const visitor::Type &t1, const visitor::Type &t2);
    bool compatible(const Operator op, const visitor::Type &t);
    visitor::Type cast(const Operator op, const visitor::Type &t1, const visitor::Type &t2);
    visitor::Type cast(const Operator op, const visitor::Type &t);
};

}; // namespace rattle

#endif