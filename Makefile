LIBS = `pkg-config opencv --libs --cflags`
OPT = -c -Wall

all: etiquetage
	@echo "All done"

etiquetage: etiquetage.o CustomPixel.o
	g++ etiquetage.o CustomPixel.o -o etiquetage ${LIBS}
	@echo "programme OK"

etiquetage.o: etiquetage.cpp
	g++ ${OPT} etiquetage.cpp ${LIBS}
	@echo "etiquetage OK"

CustomPixel.o: CustomPixel.cpp
	g++ ${OPT} CustomPixel.cpp ${LIBS}
	@echo "CustomPixel OK"

clean :
	rm *o etiquetage
	
cleanAndBuild : clean etiquetage