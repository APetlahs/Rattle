#include "type.hpp"
#include "node.hpp"
using namespace rattle::visitor;

Type::Type():
    typeClass(Null), primitive(), returnType(NULL), params() {}

Type::Type(const enum Primitive type):
    typeClass(Primitive), primitive(type), returnType(NULL), params() {}

Type::Type(ast::TypeNode *type):
    typeClass(Primitive), returnType(NULL), params()
{
    if (type->id == "int") primitive = Int;
    else if (type->id == "float") primitive = Float;
    else if (type->id == "str") primitive = Str;
    else if (type->id == "bool") primitive = Bool;
    // TODO: else throw something
}

Type::Type(ast::FuncDefNode *func):
    typeClass(Callable), primitive()
{
    returnType = new Type(func->type);
    for (std::vector<ast::TypedIdNode*>::iterator i = func->args->args.begin();
         i != func->args->args.end(); ++i)
    {
        params.push_back(Type((*i)->type));
    }
}

Type::Type(const Type &other):
    typeClass(other.typeClass), primitive(other.primitive),
    params(other.params)
{
    returnType = other.returnType != NULL ? new Type(*(other.returnType)) : NULL;
}

Type::~Type() {
    delete returnType;
}

Type &Type::operator=(const Type &other) {
    if (this == &other) return *this;
    typeClass = other.typeClass;
    primitive = other.primitive;
    returnType = other.returnType != NULL ? new Type(*(other.returnType)) : NULL;
    params = other.params;
    return *this;
}

bool Type::compatible(const Type &other) {
    return *this == other;
}

bool Type::operator==(const Type &other) const {
    if (typeClass != other.typeClass) return false;
    switch(typeClass) {
        case Null: return true;
        case Primitive: return primitive == other.primitive;
        case Array: return returnType->compatible(*(other.returnType));
        case Callable:
            if (!returnType->compatible((*other.returnType))) return false;
            if (params.size() != other.params.size()) return false;
            for (unsigned int i = params.size(); i-- > 0;) {
                Type t = params[i];
                if (!t.compatible(other.params[i])) return false;
            }
            return true;
        default: return false;
    }
}

bool Type::operator!=(const Type &other) const {
    return !(*this == other);
}
