#Make file for mac
FRAMEWORK=-lglew -lglfw3 -framework GLUT -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenCL
LIB=-L /usr/local/lib
INC=-I /usr/local/include -I /usr/include -I ../

OPTIONS=$(FRAMEWORK) $(INC) $(LIB)

all: main

main: glutils4.o clutils.o fluid.o main.o
	g++ glutils4.o clutils.o fluid.o main.o -o main $(OPTIONS)

main.o: main.cpp
	g++ main.cpp -c $(INC)

clutils.o: clutils.cpp
	g++ clutils.cpp -c $(INC)

glutils4.o: glutils4.cpp
	g++ glutils4.cpp -c $(INC)

fluid.o: fluid.cpp
	g++ fluid.cpp -c $(INC)

run:
	clear
	make
	./main

clean:
	rm -f *.o
	rm -f main

rebuild:
	clear
	make clean
	make run

