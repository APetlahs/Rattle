#include "type.hpp"
#include "node.hpp"
using namespace rattle::visitor;

Type::Type(): typeClass(Null), primitive(), returnType(NULL), params() {}

Type::Type(ast::TypeNode *type):
    typeClass(Primitive), primitive(type->id),
    returnType(NULL), params()
{}

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
    typeClass = other.typeClass;
    primitive = other.primitive;
    returnType = other.returnType != NULL ? new Type(*(other.returnType)) : NULL;
    params = other.params;
    return *this;
}

/*
bool TypeValidator::compatible(const Type &t1, const Type &t2) {
    if (t1.typeClass == Callable && t2.typeClass == Callable) {
        if (t1.params.size() != t2.params.size()) {
            return false;
        }
        for (unsigned int i = 0; i < t1.params.size(); ++i) {
            if (!compatible(t1.params[i], t2.params[i])) return false;
        }
        return true;
    } else if (t1.typeClass == t2.typeClass) {
        return true;
    } else {
        return false;
    }
}

Type TypeValidator::castBinOp(const ast::Operator op, const Type &t1, const Type &t2) {
    Type t;
    return t;
}

Type TypeValidator::castUniOp(const ast::Operator op, const Type &t1) {
    Type t;
    return t;
}
*/