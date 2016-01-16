#include <sstream>
#include "type.hpp"
#include "node.hpp"
using namespace rattle::visitor;

Type::Type():
    typeClass(Undefined), returnType(NULL), params() {}

Type::Type(const enum TypeClass type):
    typeClass(type), returnType(NULL), params() {}

Type::Type(const enum TypeClass type, Type *subType):
    typeClass(type), returnType(subType), params() {}

Type::Type(const enum TypeClass type, Type *subType, std::vector<Type> params):
    typeClass(type), returnType(subType), params(params) {}

Type::Type(ast::TypeNode *type):
    typeClass(Undefined), returnType(NULL), params()
{
    if (type->callable) {
        typeClass = Callable;
        returnType = new Type(type->subType);
        for (std::vector<ast::TypeNode*>::iterator i = type->params->types.begin();
            i != type->params->types.end(); ++i)
        {
            params.push_back(*i);
        }
    } else if (type->subType != NULL) {
        typeClass = Array;
        returnType = new Type(type->subType);
    } else {
        if (type->id == "int") typeClass = Int;
        else if (type->id == "float") typeClass = Float;
        else if (type->id == "str") typeClass = Str;
        else if (type->id == "bool") typeClass = Bool;
        // TODO: else throw something
    }
}

Type::Type(ast::FuncDefNode *func):
    typeClass(Callable)
{
    returnType = new Type(func->type);
    for (std::vector<ast::TypedIdNode*>::iterator i = func->args->args.begin();
         i != func->args->args.end(); ++i)
    {
        params.push_back(Type((*i)->type));
    }
}

Type::Type(const Type &other):
    typeClass(other.typeClass), params(other.params)
{
    returnType = other.returnType != NULL ? new Type(*(other.returnType)) : NULL;
}

Type::~Type() {
    delete returnType;
}

std::string Type::toStr() {
    std::stringstream ss;
    switch (typeClass) {
        case Null: return "null";
        case Int: return "int";
        case Float: return "float";
        case Str: return "str";
        case Bool: return "bool";
        case Array:
            ss << "Array[" << returnType->toStr() << "]";
            return ss.str();
        case EmptyArray:
            ss << "Array[]";
            return ss.str();
        case Callable:
            ss << "Callable(";
            for (std::vector<Type>::iterator i = params.begin();
                 i != params.end(); ++i)
            {
                ss << (*i).toStr();
                if (i+1 != params.end()) ss << ",";
            }
            ss << ")->" << returnType->toStr();
            return ss.str();
        case Undefined:
            return "Undefined";
        default:
            return "UnimplementedType"; // TODO: raise something
    }
}

Type &Type::operator=(const Type &other) {
    if (this == &other) return *this;
    typeClass = other.typeClass;
    returnType = other.returnType != NULL ? new Type(*(other.returnType)) : NULL;
    params = other.params;
    return *this;
}

bool Type::compatible(const Type &other) {
    if (isUndefined() || other.isUndefined())
        return false;

    if (*this == other)
        return true;

    switch(typeClass) {
        case Array:
            return other.typeClass == EmptyArray
            || (other.typeClass == Array
                && returnType->compatible(*(other.returnType)));
        case EmptyArray:
            return other.typeClass == Array || other.typeClass == EmptyArray;
        case Callable:
            if (other.typeClass != Callable) return false;
            if (!returnType->compatible((*other.returnType))) return false;
            if (params.size() != other.params.size()) return false;
            for (unsigned int i = params.size(); i-- > 0;) {
                Type t = params[i];
                if (!t.compatible(other.params[i])) return false;
            }
            return true;
        case Float:
            return other.typeClass == Int || other.typeClass == Float;
        default:
            return typeClass == other.typeClass;
    }
}

bool Type::operator==(const Type &other) const {
    if (typeClass != other.typeClass) return false;
    switch(typeClass) {
        case Undefined: return true;
        case Array: return returnType->compatible(*(other.returnType));
        case Callable:
            if (!returnType->compatible((*other.returnType))) return false;
            if (params.size() != other.params.size()) return false;
            for (unsigned int i = params.size(); i-- > 0;) {
                Type t = params[i];
                if (!t.compatible(other.params[i])) return false;
            }
            return true;
        default: return typeClass == other.typeClass;
    }
}

bool Type::operator!=(const Type &other) const {
    return !(*this == other);
}

bool Type::isIterable() const {
    switch (typeClass) {
        case Array: return true;
        case EmptyArray: return true;
        case Str: return true;
        default: return false;
    }
}

Type Type::getIterator() const {
    if (typeClass == Array) return *returnType;
    if (typeClass == Str) return Type(Str);
    return Type();
}
