#ifndef _TYPE_HPP_
#define _TYPE_HPP_
#include <vector>
#include <map>
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
    std::map<std::string, Type> members;

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
    bool isIterable() const;
    Type getIterator() const;
    Type getMember(const std::string member);
};

} // namespace visitor
} // namespace rattle

#endif
