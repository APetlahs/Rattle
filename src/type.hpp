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

enum TypeClass { Undefined, Null, Int, Float, Str,
                 Bool, Array, EmptyArray, Callable };

class Type {
public:
    enum TypeClass typeClass;
    Type *returnType;
    std::vector<Type> params;
    std::map<std::string, Type> members;

    Type();
    Type(const enum TypeClass type);
    Type(const enum TypeClass type, Type *subType);
    Type(const enum TypeClass type, Type *subType, std::vector<Type> params);
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
    void addMember(const std::string member, Type &t);
    void addMembers(const std::map<std::string, Type> &members);
    bool isUndefined() const { return typeClass == Undefined; }
};

} // namespace visitor
} // namespace rattle

#endif
