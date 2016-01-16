#include "operator.hpp"
#include "type.hpp"
using namespace rattle;
using namespace rattle::visitor;

static bool isMath(Operator op) {
    return (op == ADD)||(op == SUB)||(op == MUL)
         ||(op == DIV)||(op == POW)||(op == MOD);
}

static bool isLogic(Operator op) {
    return (op==AND)||(op==OR)||(op==NOT);
}

static bool isCmp(Operator op) {
    return (op==LT)||(op==GT)||(op==LTE)
         ||(op==GTE)||(op==EQ)||(op==NEQ);
}

static bool isAccessor(Operator op) {
    return op == ACCESS;
}

static Type castBinMath(const Operator op, const Type &t1, const Type &t2) {
    const enum TypeClass tc1 = t1.typeClass;
    const enum TypeClass tc2 = t2.typeClass;
    if (tc1 == Int && tc2 == Int) return Type(Int);
    if (tc1 == Float && tc2 == Int) return Type(Float);
    if (tc1 == Int && tc2 == Float) return Type(Float);
    return Type();
}

static Type castBinLogic(const Operator op, const Type &t1, const Type &t2) {
    if (t1.typeClass == Bool && t2.typeClass == Bool) {
        return Type(Bool);
    }
    return Type();
}

static Type castBinCmp(const Operator op, const Type &t1, const Type &t2) {
    const enum TypeClass tc1 = t1.typeClass;
    const enum TypeClass tc2 = t2.typeClass;
    if ((tc1 == Int && tc2 == Int)
        || (tc1 == Int && tc2 == Float)
        || (tc1 == Float && tc2 == Int)
        || (tc1 == Float && tc2 == Float))
    return Type(Bool);
    return Type();
}

static Type castAccessor(const Operator op, const Type &t1, const Type &t2) {
    const enum TypeClass tc1 = t1.typeClass;
    const enum TypeClass tc2 = t2.typeClass;
    if (tc1 == Array || tc1 == EmptyArray) {
        if (tc2 == Int) {
            return Type(*(t1.returnType));
        }
    } if (tc1 == Str) {
        if (tc2 == Int) {
            return Type(Str);
        }
    }
    return Type();
}

bool CastMap::compatible(const Operator op, const Type &t1, const Type &t2) {
    Type null_type = Type();
    return cast(op,t1,t2) != null_type;
}

bool CastMap::compatible(const Operator op, const Type &t) {
    Type null_type = Type();
    return cast(op,t) != null_type;
}

Type CastMap::cast(const Operator op, const Type &t1, const Type &t2) {
    if (t1.isUndefined() || t2.isUndefined()) return Type();
    if (isMath(op)) return castBinMath(op,t1,t2);
    if (isLogic(op)) return castBinLogic(op,t1,t2);
    if (isCmp(op)) return castBinCmp(op,t1,t2);
    if (isAccessor(op)) return castAccessor(op,t1,t2);
    // TODO : throw something
    return Type();
}

Type CastMap::cast(const Operator op, const Type &t) {
    if (isMath(op)) {
        switch (t.typeClass) {
            case Int: return Type(Int);
            case Float: return Type(Float);
            default: return Type();
        }
    } else if (isLogic(op)) {
        if (t.typeClass == Bool) {
            return Type(Bool);
        }
    }
    return Type();
}
