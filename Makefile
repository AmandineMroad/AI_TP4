LIBS = `pkg-config opencv --libs --cflags`
OPT = -c -Wall

all: etiquetage
    @echo "All done"

etiquetage: etiquetage.o
	g++ etiquetage.o -o etiquetage ${LIBS}
	@echo "etiquetage -o OK"

etiquetage.o: etiquetage.cpp
	g++ ${OPT} etiquetage.cpp ${LIBS}
	@echo "etiquetage OK"

clean :
	rm *o etiquetage