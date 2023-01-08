CC=mpic++
CFLAGS= -Wall
DEPS=Worker.h Coordinator.h Process.h Leader.h
OBJ=tema3.o Worker.o Coordinator.o Process.o Leader.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ)
	$(CC) -o tema3 $^ $(CFLAGS)
	rm -f *.o

.PHONY: clean

clean:
	rm -rf tema3
