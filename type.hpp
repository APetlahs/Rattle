#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <vector>
#include "node.hpp"

namespace rattle  {
namespace visitor {

enum TypeClass { Null, Primitive, Array, Callable };

struct Type {
    TypeClass typeClass;
    std::string primitive;
    Type *returnType;
    std::vector<Type> params;

    Type();
    Type(ast::TypeNode *type);
    Type(ast::FuncDefNode *func);
    Type(const Type &other);
    ~Type();
    Type &operator=(const Type &other);
};

/*
class TypeValidator {
public:
    bool compatible(const Type &t1, const Type &t2);
    Type castBinOp(const ast::Operator op, const Type &t1, const Type &t2);
    Type castUniOp(const ast::Operator op, const Type &t1);
};
*/

} // namespace visitor
} // namespace rattle

#endif
