CXX = g++
CXXFLAGS = -Wall -MMD
EXEC = rattle
OBJECTS = grammar.o lexer.o main.o node.o visitor.o visitor_print.o \
          visitor_varcheck.o visitor_typecheck.o type.o symbol.o operator.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} -o ${EXEC} ${OBJECTS}

grammar.o: grammar.y
	bison -d --verbose grammar.y
	${CXX} ${CXXFLAGS} -c -o grammar.o grammar.tab.c

lexer.o: lexer.l
	flex lexer.l
	${CXX} ${CXXFLAGS} -c -o lexer.o lex.yy.c

-include ${DEPENDS}

.PHONY: clean

clean:
	rm -f ${OBJECTS} ${EXEC} ${DEPENDS}

