LIBS = `pkg-config opencv --libs --cflags`
OPT = -c -Wall

all: etiquetage
	@echo "All done"

etiquetage: main.o CustomPixel.o
	g++ ${OPT} main.o CustomPixel.o -o etiquetage ${LIBS}
	@echo "etiquetage OK"

main.o: main.cpp
	g++ ${OPT} main.cpp ${LIBS}
	@echo "etiquetage OK"

CustomPixel.o: CustomPixel.cpp
	g++ ${OPT} CustomPixel.cpp ${LIBS}
	@echo "etiquetage OK"

clean :
	rm *o etiquetage