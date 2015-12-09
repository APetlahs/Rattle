#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <vector>
#include "node.hpp"

namespace rattle  {
namespace visitor {

enum TypeClass { Null, Primitive, Callable };

// TODO: add proper dtor, assignment op, and copy ctor
struct Type {
    TypeClass typeClass;
    std::string name;
    Type *returnType;
    std::vector<Type> params;

    Type(): typeClass(Null), name("") {}
    Type(ast::TypeNode* type);
    Type(ast::FuncDefNode* func);
};

class TypeValidator {
public:
    bool compatible(const Type &t1, const Type &t2);
    Type castBinOp(const ast::Operator op, const Type &t1, const Type &t2);
    Type castUniOp(const ast::Operator op, const Type &t1);
};

} // namespace visitor
} // namespace rattle

#endif