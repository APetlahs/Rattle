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
                 Bool, Array, EmptyArray, Callable,
                 Object };

class Type {
public:
    enum TypeClass typeClass;
    Type *returnType;
    std::vector<Type> params;
    std::string className;

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
    bool isUndefined() const { return typeClass == Undefined; }
};

class MemberTable {
public:
    std::map<TypeClass, std::map<std::string, Type> > primitiveMembers;
    std::map<std::string, std::map<std::string, Type> > classMembers;

    MemberTable();
    bool hasMember(const std::string member, const Type &t);
    Type getMember(const std::string member, const Type &t);
    void bindMember(const std::string className, const std::string member, const Type &t);
};

} // namespace visitor
} // namespace rattle

#endif
