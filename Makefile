CXX = g++
CXXFLAGS = -Wall -MMD
EXEC = rattle
OBJECTS = main.o grammar.o lexer.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} -o ${EXEC} ${OBJECTS}

grammar.o: grammar.y
	bison -d grammar.y
	${CXX} ${CXXFLAGS} -c -o grammar.o grammar.tab.c

lexer.o: lexer.l
	flex lexer.l
	${CXX} ${CXXFLAGS} -c -o lexer.o lex.yy.c

-include ${DEPENDS}

.PHONY: clean

clean:
	rm -f ${OBJECTS} ${EXEC} ${DEPENDS} grammar.tab.h grammar.tab.c lex.yy.c

