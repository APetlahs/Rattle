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
    if (t1.typeClass == t2.typeClass) {
        if (t1.primitive == Int && t2.primitive == Int) return Type(Int);
        if (t1.primitive == Float && t2.primitive == Int) return Type(Float);
        if (t1.primitive == Int && t2.primitive == Float) return Type(Float);
    }
    return Type();
}

static Type castBinLogic(const Operator op, const Type &t1, const Type &t2) {
    if (t1.typeClass == Primitive && t2.typeClass == Primitive) {
        if (t1.primitive == Bool && t2.primitive == Bool) {
            return Type(Bool);
        }
    }
    return Type();
}

static Type castBinCmp(const Operator op, const Type &t1, const Type &t2) {
    if (t1.typeClass == t2.typeClass && t1.typeClass == Primitive) {
        if ((t1.primitive == Int && t2.primitive == Int)
            || (t1.primitive == Int && t2.primitive == Float)
            || (t1.primitive == Float && t2.primitive == Int)
            || (t1.primitive == Float && t2.primitive == Float))
        return Type(Bool);
    }
    return Type();
}

static Type castAccessor(const Operator op, const Type &t1, const Type &t2) {
    if (t1.typeClass == Array || t1.typeClass == EmptyArray) {
        if (t2.typeClass == Primitive && t2.primitive == Int) {
            return Type(*(t1.returnType));
        }
    } if (t1.typeClass == Primitive && t1.primitive == Str) {
        if (t2.typeClass == Primitive && t2.primitive == Int) {
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
    if (t1.typeClass == Undefined || t2.typeClass == Undefined) return Type();
    if (isMath(op)) return castBinMath(op,t1,t2);
    if (isLogic(op)) return castBinLogic(op,t1,t2);
    if (isCmp(op)) return castBinCmp(op,t1,t2);
    if (isAccessor(op)) return castAccessor(op,t1,t2);
    // TODO : throw something
    return Type();
}

Type CastMap::cast(const Operator op, const Type &t) {
    if (isMath(op)) {
        if (t.typeClass == Primitive) {
            switch (t.primitive) {
                case Int: return Type(Int);
                case Float: return Type(Float);
                default: return Type();
            }
        }
    } else if (isLogic(op)) {
        if (t.typeClass == Primitive && t.primitive == Bool) {
            return Type(Bool);
        }
    }
    return Type();
}
