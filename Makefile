CC = gcc -ansi -pedantic
CFLAGS = -Wall
EXE = main
LIBRARIES = enemies.o level.o map.o people.o pritn_on_screen.o

all : $(EXE)

.PHONY : clean
clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o $(LIBRARIES)
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o $(LIBRARIES)


run:
	./main

