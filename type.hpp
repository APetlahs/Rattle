#ifndef _TYPE_HPP_
#define _TYPE_HPP_
#include <vector>
#include <string>

namespace rattle  {

namespace ast {
// forward declerations
class TypeNode;
class FuncDefNode;
}

namespace visitor {

enum TypeClass { Undefined, Primitive, Array, EmptyArray, Callable };
enum Primitive { Null, Int, Float, Str, Bool };

class Type {
public:
    enum TypeClass typeClass;
    enum Primitive primitive;
    Type *returnType;
    std::vector<Type> params;

    Type();
    Type(const enum Primitive type);
    Type(ast::TypeNode *type);
    Type(ast::FuncDefNode *func);
    Type(const Type &other);
    ~Type();

    std::string toStr();
    bool compatible(const Type &other);
    Type &operator=(const Type &other);
    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;
};

} // namespace visitor
} // namespace rattle

#endif
